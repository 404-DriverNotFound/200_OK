#include "Server.hpp"
#include "ServerManager.hpp" // NOTE 상호참조 문제를 해결하기 위해서!
#include "Response.hpp"

void		Server::run(void)
{
	std::map<int, Connection>::iterator	it = mConnections.begin();
	while (it != mConnections.end())
	{
		std::map<int, Connection>::iterator	it2 = it++;
		int fd = this->mSocket;
		if (it2->second.getSocket() == fd)
		{
			continue ;
		}
		try
		{
			if (hasSendWork(it2->second))
			{
				runSend(it2->second);
				struct timeval	time;
				gettimeofday(&time, NULL);
				it2->second.set_m_last_reqeust_at(time);
			 	continue ; // FIXME 어떻게 처리할지...
			}
			if (hasExecuteWork(it2->second))
			{
				runExecute(it2->second);
				continue ;
			}
			if (hasRequest(it2->second))
			{
				if (it2->second.get_m_request() == NULL)
				{
					it2->second.set_m_request(new Request());
				}
				runRecvAndSolve(it2->second);
			}
		}
		catch (const std::exception& e)
		{
			closeConnection(it2->second.getSocket());
		}
	}
	if (hasNewConnection())
	{
		if (mConnections.size() >= (INIT_FD_MAX / mManager->GetServers().size()))
		{
			int fd = getUnuseConnectionFd();
			if (fd == -1)
				return ;
			closeConnection(fd);
		}
		
		if (false == acceptNewConnection())
		{
			// cout << "accpetNewconnection(): Fail" << std::endl;
			// reportCreateNewConnectionLog();
		}
		else
		{
			// cout << "accpetNewconnection(): Success" << std::endl;
		}
	}
	// cout << this->mport << "'s connection_size: "<< mConnections.size() << std::endl; 
}

bool		Server::hasSendWork(Connection& connection)
{
	// value = connection.get_m_request()->GetPhase();
	// TODO COMPLETE로 가정하였으나, 실제로는 request의 진행상황에 따라서 { READY, ON_HEADER, ON_BODY, COMPLETE }; 단계로 나뉨.
	if (connection.get_m_request() == NULL)
		return (false);
	
	if (connection.GetStatus() == Connection::SEND_READY || connection.GetStatus() == Connection::SEND_ING)
	{
		if (FD_ISSET(connection.getSocket(), &(this->mManager->GetWriteCopyFds())))
		{
			// closeConnection(connection.getSocket());
			return (true);
		}
		return (false);
	}
	else
	{
		return (false);
	}
}

bool		Server::runSend(Connection& connection)
{
	static int send_number = 0;
	Request *request = connection.get_m_request();
	Response *response = connection.get_m_response();

	if (connection.GetStatus() == Connection::SEND_READY)
	{
		response->set_m_response(response->makeResponse()); // NOTE 보낼 response 만들어서, 앞으로 사용할 변수에 저장해서, 이 변수에서 뽑아내서 전송할꺼임!
		errno = 0;

		// int count = 1000000;
		// int snd_buf= count * 1, rcv_buf= count * 3;

		// NOTE  최적화1. 수신 버퍼의 크기 조절하기
		// state=setsockopt(connection.getSocket(), SOL_SOCKET, SO_RCVBUF, (void*)&rcv_buf, sizeof(rcv_buf)); // RECV buffer 늘리기
		// state = setsockopt(connection.getSocket(), SOL_SOCKET, SO_SNDBUF, (void*)&snd_buf, sizeof(snd_buf)); // SEND buffer 늘리기
		// cout << "state: " << state << std::endl;

		// NOTE  최적화1. Nagle 알고리즘 해제하기
		// int opt_val = true;
		// state = setsockopt(connection.getSocket(), IPPROTO_TCP, TCP_NODELAY, (void *)&opt_val, sizeof(opt_val));
		// cout << "state: " << state << std::endl;

		// perror("what?:");
		errno = 0;

		connection.SetStatus(Connection::SEND_ING);
		return (false);

	}
	else if(connection.GetStatus() == Connection::SEND_ING)
	{
		errno = 0;
		// perror("what?:");
		ssize_t	count = write(connection.getSocket(), response->get_m_response().c_str(), response->get_m_response().length());
		if (count <= 0)
		{
			throw IOError();
		}
		std::size_t	write_size = count;
		if (write_size != response->get_m_response().length())
		{
			// cout << "write_size: " << write_size << std::endl;
			response->set_m_response(response->get_m_response().substr(write_size));
			// cout << "now_length: " << response->get_m_response().length() << std::endl;
			return (false);
		}
	}

	int	statusCode = response->get_m_status_code();
	if (statusCode >= 200 && statusCode < 300)
	{
		std::cout << GRN;
	}
	else
	{
		std::cout << RED;
	}
	std::cout << "[" << ft::getHTTPTimeFormat(request->GetStartTime().tv_sec) << "][access][" << connection.get_m_client_ip() << ":" << connection.get_m_client_port() << "]";
	std::cout << "[" << request->GetMethod() << "][" << response->get_m_status_code() << " " << response->m_status_map[statusCode] << "]" NC << std::endl;

	// request->ShowMessage(); // ANCHOR request message debugging 용
	// response->ShowMessage(); // ANCHOR response message debugging 용

	send_number++;
	std::cout << "send_nubmer: " << send_number << std::endl;

	// ANCHOR 작업중
	delete request;
	connection.set_m_request(NULL);
	delete response;
	connection.set_m_response(NULL);
	connection.SetStatus(Connection::REQUEST_READY);
	mManager->ClrWriteFds(connection.getSocket());
	mManager->ClrWriteCopyFds(connection.getSocket());
	// FD_CLR(connection.getSocket(), &(this->mManager->GetWriteFds()));
	// FD_CLR(connection.getSocket(), &(this->mManager->GetWriteCopyFds()));
	// closeConnection(connection.getSocket());
	// ANCHOR 작업중

	return (true);
}

bool		Server::hasExecuteWork(const Connection& connection) const
{
	if (connection.GetStatus() == Connection::CGI_READY ||
		connection.GetStatus() == Connection::CGI_ING)
	{
		return (true);
	}
	else
	{
		return (false);
	}
}

bool		Server::runExecute(Connection& connection)
{
	// TODO cgi 여부에 따라 걸러주는 로직이 있어야함 cgi 아니면 send ready
	try
	{
		if (connection.get_m_request()->GetURItype() == Request::CGI_PROGRAM)
		{
			executeCGI(connection);
			return (true);
		}
		else
		{
			connection.SetStatus(Connection::SEND_READY);
			return (false);
		}
	}
	catch (int status_code)
	{
		// std::cout << "runExecute catch: " << status_code << std::endl;
		create_Response_statuscode(connection, status_code);
		connection.SetStatus(Connection::SEND_READY);
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	return (false);
}

bool		Server::hasRequest(const Connection& connection)
{
	if (FD_ISSET(connection.getSocket(), &(this->mManager->GetReadCopyFds()))) // REVIEW	request의 phase도 함께 확인해야할 수도 있을 듯
	{
		// std::cout << "client(" << connection.getSocket() << ") : has request" << std::endl;
		return (true);
	}
	else
	{
		return (false);
	}
}

bool		Server::runRecvAndSolve(Connection& connection)
{
	try
	{
		recvRequest(connection);
	}
	catch (int status_code)
	{
		// std::cout << "runRecvAndSolve catch: " << status_code << std::endl;
		create_Response_statuscode(connection, status_code);
		connection.SetStatus(Connection::SEND_READY);
		return (true);
	}
	catch (const Server::IOError& e)
	{
		throw (e);
	}
	catch (const std::exception& e)
	{
		// ft::log(ServerManager::log_fd, std::string("[Failed][Request] Failed to create request because ") + e.what());
		// createResponse(connection, 50001);
	}

	try
	{
		Request& request = *connection.get_m_request();
		if (request.GetPhase() == Request::COMPLETE)
		{
		// 	writeCreateNewRequestLog(request);
		// 	connection.set_m_status(Connection::ON_EXECUTE); //REVIEW 이게 맞나?
			solveRequest(connection, *connection.get_m_request());
			return (true);
		}
		return (false);
	}
	catch (int status_code)
	{
		// std::cout << "runRecvAndSolve catch: " << status_code << std::endl;
		create_Response_statuscode(connection, status_code);
		connection.SetStatus(Connection::SEND_READY);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	return (false);
}

bool		Server::hasNewConnection()
{
	if (FD_ISSET(this->mSocket, &(this->mManager->GetReadCopyFds())))
	{
		// cout << "this->msocket: " << this->msocket << std::endl;
		return (true);
	}
	else
	{
		return (false);
	}
}

bool		Server::acceptNewConnection()
{
	sockaddr_in	sockaddr;
	socklen_t	socketlen = sizeof(struct sockaddr);
	int			client_socket = accept(this->mSocket, reinterpret_cast<struct sockaddr*>(&sockaddr), reinterpret_cast<socklen_t*>(&socketlen));
	if (client_socket == -1)
	{
		// std::cerr << "Could not create socket." << std::endl;
		return (false);
	}
	fcntl(client_socket, F_SETFL, O_NONBLOCK);
	mManager->SetReadFds(client_socket);
	mManager->SetReadCopyFds(client_socket);
	// FD_SET(client_socket, &(this->mManager->GetReadFds()));
	// FD_SET(client_socket, &(this->mManager->GetReadCopyFds()));
	// FD_SET(client_socket, &(this->mManager->GetWriteFds()));
	// FD_SET(client_socket, &(this->mManager->GetWriteCopyFds()));
	this->mConnections[client_socket] = Connection(client_socket, ft::inet_ntos(sockaddr.sin_addr), this->mPort);
	std::cerr << GRNB "[" << ft::getCurrentTime() << "][connection]" << "[ESTABLISHED]" << "[" << client_socket << "]" << NC << std::endl;
	// close(client_socket); // NOTE 이제 keep-alive로 관리
	return (true);
}
