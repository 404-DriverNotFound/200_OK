#include "Server.hpp"
#include "ServerManager.hpp" // NOTE 상호참조 문제를 해결하기 위해서!
#include "Response.hpp"

void		Server::executeAutoindex(Connection& connection, std::string uri_copy)
{
	connection.SetResponse(new Response(&connection, 200, ft::makeAutoindexHTML(uri_copy)));
	Response *response = connection.GetResponse();
	response->SetHeaders("Server", "webserv");
	response->SetHeaders("Content-Type", "text/html");
	response->SetHeaders("Connection", "keep-alive");
	response->SetHeaders("Date", ft::getCurrentTime().c_str());
	response->SetHeaders("Content-Length", ft::itos(connection.GetResponse()->GetBody().length()));

	connection.SetStatus(Connection::SEND_READY);
}

void		Server::executeGet(Connection& connection, std::string target_uri)
{
	if (ft::access(target_uri) == false)
	{
		throw 404;
	}
	else
	{
		create_Response_200(connection, target_uri, ANY_FILE);
		return ;
	}
}

void		Server::executeHead(Connection& connection, std::string target_uri)
{
	executeGet(connection, target_uri);
	std::string null_str;
	connection.GetResponse()->SetBody(null_str);
	Response *response = connection.GetResponse();

	std::map<std::string, std::string>::iterator it = response->GetHeaders().begin();
	std::map<std::string, std::string> temp;
	while (it != response->GetHeaders().end())
	{
		if (it->first == "Date")
			temp["Date"] = it->second;
		else if (it->first == "Server")
			temp["Server"] = it->second;
		else if (it->first == "Content-Length")
			temp["Content-Length"] = it->second;
		it++;
	}
	response->CopyHeaders(temp);
	
}

void		Server::executePost(Connection& connection, const Request& request)
{
	// int fd = open(target_uri.c_str(), O_WRONLY | O_TRUNC | O_CREAT, 0755);
	// int ret = write(fd, response->GetBody().c_str(), response->GetBody().length());
	// close (fd); // REVIEW POST에서 request가 들어오던가? 왜 이걸 내가 파일에 저장해야하는지 모르겠어. 존재하는 .bla 파일을 이용하는거아닌가?
	// cout << "ret: " << ret << std::endl;
	
	connection.SetResponse(new Response(&connection, 200, request.GetBody()));
}

void		Server::executePut(Connection& connection, const Request& request, std::string target_uri)
{
	// NOTE 우선은 파일만 Put에 들어온다고 가정하자.
	bool file_exist = 0;
	errno = 0;
	if (target_uri.back() == '/') // NOTE 무조껀 파일경로로 open 하도록 함. 폴더 경로로 open하면, 동작이 조금 다르다.
		target_uri.pop_back();
	// cout << "pop_target_uri: target_uri" << target_uri << std::endl;
	int open_fd = open(target_uri.c_str(), O_WRONLY | O_TRUNC);
	// cout << "first open_fd: " << open_fd << std::endl;

	if (open_fd > 2)
	{
		file_exist = true;
		// perror("whatwhat??:");
		if (errno == 0) // NOTE 파일를 open으로 열었음
		{
			if (connection.GetResponse() != NULL)
			{
				delete connection.GetResponse();
				connection.SetResponse(NULL);
			}
			Response *temp2  = new Response(&connection, 200, request.GetBody());
			// printf("temp2 %p \n", temp2);
			connection.SetResponse(temp2);
			write(open_fd, connection.GetResponse()->GetBody().c_str(), connection.GetResponse()->GetBody().length());
		}
		close(open_fd);
	}
	else
	{
		if (errno == EISDIR) // NOTE 폴더를 open으로 열었음
		{
			connection.SetResponse(new Response(&connection, 204));
			std::string temp;
			connection.GetResponse()->SetBody(temp);
		}
		else if (errno == ENOENT)
		{
			if (connection.GetResponse() != NULL)
			{
				delete connection.GetResponse();
				connection.SetResponse(NULL);
			}
			connection.SetResponse(new Response(&connection, 201, request.GetBody()));	
			int open_fd2 = open(target_uri.c_str(), O_WRONLY | O_CREAT, 0755);
			// cout << "second open_fd: " << open_fd2 << std::endl;

			write(open_fd2, connection.GetResponse()->GetBody().c_str(), connection.GetResponse()->GetBody().length());
			close(open_fd2);

		}
	}
	Response *response = connection.GetResponse();
	// cout << "Response %p" << response << std::endl;
	response->SetHeaders("Date", ft::getCurrentTime().c_str());
	response->SetHeaders("Server", "webserv");
	if (errno == 0)
	{
		response->SetHeaders("Content-Length", ft::itos(response->GetBody().length()));
		response->SetHeaders("Content-Type", "text/html");
		response->SetHeaders("Content-Language", "en-US");
		if (file_exist == false)
		{
			response->SetHeaders("Location", target_uri);
		}
	}
	errno = 0; // NOTE 초기화!
}

void		Server::executeDelete(Connection& connection, const Request& request, std::string target_uri)
{
	bool file_exist = 0;
	int open_fd = open(target_uri.c_str(), O_RDWR);
	int temp = errno;
	if (open_fd > 0)
	{
		file_exist = true;
		if (errno == 0) // NOTE 파일를 open으로 열었음
		{
			int status_code = 200;
			connection.SetResponse(new Response(&connection, status_code, request.GetBody()));
			std::string errorpage_body = Response::MakeStatusPage(status_code, connection.GetRequest()->GetMethod());
			connection.GetResponse()->SetBody(errorpage_body);
			connection.GetResponse()->SetHeaders("Content-Length", ft::itos(errorpage_body.length()));
			unlink(target_uri.c_str());
			temp = errno;
		}
		else if (errno == EISDIR) // NOTE 폴더를 open으로 열었음
		{
			int status_code = 204;
			connection.SetResponse(new Response(&connection, status_code));
			rmdir(target_uri.c_str());
			temp = errno;
		}
		close(open_fd);
		temp = errno;
	}
	else // ENOENT 2
	{
		int status_code = 204;
		connection.SetResponse(new Response(&connection, status_code));
		int open_fd = open(target_uri.c_str(), O_WRONLY | O_CREAT, 0755);
		write(open_fd, connection.GetResponse()->GetBody().c_str(), connection.GetResponse()->GetBody().length());
		close(open_fd);
	}
	Response *response = connection.GetResponse();

	response->SetHeaders("Date", ft::getCurrentTime().c_str());
	response->SetHeaders("Server", "webserv");
	if (errno == 0 && file_exist == true)
	{
		response->SetHeaders("Content-Length", ft::itos(response->GetBody().length()));
		response->SetHeaders("Content-Type", "text/html");
		response->SetHeaders("Content-Language", "en-US");
	}
	errno = 0; // NOTE 초기화!
}

void		Server::executeOptions(Connection& connection, std::string target_uri, config_iterator config_it)
{
	int fd = open(target_uri.c_str(), O_RDONLY);
	if (fd < 0)
		throw 404;
	else
	{
		connection.SetResponse(new Response(&connection, 200));
		Response *response = connection.GetResponse();
		response->SetHeaders("Date", ft::getCurrentTime().c_str());
		response->SetHeaders("Server", "webserv");
		std::string value;
		for (size_t i = 0; i < config_it.locationPath->mMethods.size(); i++)
		{
			value += config_it.locationPath->mMethods[i];
			if (i != config_it.locationPath->mMethods.size() - 1)
				value += " ";
		}
		response->SetHeaders("Allow", value);
		close(fd);
	}
}

void		Server::aexecuteTrace(Connection& connection)
{
	connection.GetSocket();
}

void		Server::executeCGI(Connection& connection) // NOTE request는 전혀 사용되지 않음
{
	Response*	response = connection.GetResponse();

	std::string connectionFD = ft::itos(connection.GetSocket());
	if (connectionFD.size() == 0)
	{
		throw 500;
	}
	std::string	fromCGIfileName = "fromCGI" + connectionFD;

	if (connection.GetStatus() == Connection::CGI_READY)
	{
		std::string	toCGIfileName("toCGI");

		int	toCGI = open(toCGIfileName.c_str(), O_CREAT | O_TRUNC | O_RDWR, 0666);
		if (toCGI == -1)
		{
			throw 500;
		}
		// fcntl(toCGI, F_SETFL, O_NONBLOCK);
		int	fromCGI = open(fromCGIfileName.c_str(), O_CREAT | O_TRUNC | O_RDWR, 0666);
		if (fromCGI == -1)
		{
			close(toCGI);
			throw 500;
		}
		int	cnt = write(toCGI, response->GetBody().c_str(), response->GetBody().length());
		// cout << "toCGI: " << toCGI << std::endl;
		// cout << "fromCGI: " << fromCGI << std::endl;

		if (cnt < 0)
		{
			close(fromCGI);
			close(toCGI);
			throw 500;
		}
		else if (cnt == 0)
		{
			close(fromCGI);
			close(toCGI);
			throw 205;
		}
		if (lseek(toCGI, 0, SEEK_SET) == -1)
		{
			close(fromCGI);
			close(toCGI);
			throw 500;
		}

		char** envp = createCGIEnv(connection);
		if (envp == NULL)
		{
			close(fromCGI);
			close(toCGI);
			throw 500;
		}
		int pid = fork();
		if (pid < 0)
		{
			free(envp); envp = NULL;
			close(fromCGI);
			close(toCGI);
			throw 500;
		}
		else if (pid == 0)
		{
			dup2(toCGI, 0); dup2(fromCGI, 1);
			execve("ft_tester/cgi_tester", 0, envp); // NOTE execve 돌아가는 코드
			exit(0);
		}
		else
		{
			waitpid(pid, 0, 0);
			free(envp); envp = NULL;
			close(fromCGI);
			close(toCGI); //unlink(toCGIfileName.c_str());
			connection.SetStatus(Connection::CGI_ING);
			return ;
		}
	}
	else
	{
		int	fromCGI = open(fromCGIfileName.c_str(), O_RDONLY);
		// cout << "fromCGIfilename: " << fromCGIfileName << std::endl;
		struct stat	statBuf;

		if (fstat(fromCGI, &statBuf) == -1)
		{
			close(fromCGI);
			throw 500;
		}
		char *buf = (char *)malloc(sizeof(char) * (statBuf.st_size + 1));
		
		int cnt = read(fromCGI, buf, statBuf.st_size);
		// cout << "cnt: " << cnt << std::endl;
		buf[cnt] = 0;
		if (cnt <= 0)
		{
			close(fromCGI);
			throw 500;
		}

		// STUB 파싱과정 필요
		std::string fromCGI_str(buf);
		free(buf);
	
		std::size_t seek = fromCGI_str.find("\r\n\r\n", 0);
		if (seek == std::string::npos)
		{
			throw 500;
		}
		std::size_t seek_cur = 0;
		while (true)
		{
			std::size_t	seek  = fromCGI_str.find("\r\n", seek_cur);
			if (seek == std::string::npos)
			{
				throw 500;
			}
			std::string	header = fromCGI_str.substr(seek_cur, seek - seek_cur);
			if (header.empty())
			{
				seek_cur += 2;
				break ;
			}
			// std::cout << "\t|" << header << "|" << std::endl;
			// REVIEW 정보는 딱 두개 만 보내주는건가, header 넣는 부분. 값을 받아서 넣어줘야하는데, 그냥 수기로 넣어주고 있음.
			// Status: 200 OK
			// Content-Type: text/html; charset=utf-8
	
			std::size_t	found = header.find(": ");
			if (found == std::string::npos)
			{
				throw 500;
			}
			std::string	key = header.substr(0, found);
			std::locale loc;
			for(std::string::size_type i = 0; i < key.length(); i++)
			{
				key[i] = std::tolower(key[i], loc);
			}

			std::string	value = header.substr(found + 2);
			// REVIEW CGI program 에서 Chunked로 보내주기도 하나?
			if (key == "status")
			{
				std::size_t found = value.find(" ");
				std::string status_code_str = value.substr(0, found);
				int status_code = std::atoi(status_code_str.c_str());
				if (status_code != 200)
				{
					// STUB 1. CGI의 반환 header를 하나만 넣어준다면, 아래로 설정하기
					// delete (connection.GetResponse());
					// connection.SetResponse(new Response(&connection, status_code, Response::MakeStatusPage(status_code, request.GetMethod())));

					// STUB 2. content_length가 필요해서 아래 있는 함수를 이용하는 것이 좋아보임.
					this->create_Response_statuscode(connection, status_code);
					return ; // REVIEW throw 를 status_code로 던져서 윗단에서 생성시키도록 해야하나.
				}
			}
			else
			{
				// std::cout << "\t\t|" << key << "| |" << value << "|" << std::endl;
				connection.GetResponse()->SetHeaders(key, value);
			}
			seek_cur = seek + 2;
		}
		std::size_t seek_body = seek_cur;

		response->SetBody(fromCGI_str.substr(seek_body));
		response->SetHeaders("Content-Length", ft::itos(response->GetBody().length()));
		close(fromCGI); //unlink(fromCGIfileName.c_str());
		connection.SetStatus(Connection::SEND_READY);
	}
}