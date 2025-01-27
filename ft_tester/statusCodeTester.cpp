#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define NC "\e[0m"
#define RED "\e[0;31m"
#define GRN "\e[0;32m"
#define REDB "\e[41m"
#define GRNB "\e[42m"

void	test403(const struct sockaddr_in& sockAddr)
{
	std::cout << std::endl << "press enter to continue" << std::endl;
	std::string	tmp;
	std::getline(std::cin, tmp);
	system("clear");
	{
		int	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (connect(clientSocket, (struct sockaddr*)&sockAddr, sizeof(sockAddr)) < 0)
		{
			throw std::exception();
		}
		std::cout << GRNB << "TEST 403 STATUS CODE" << NC << std::endl;
		std::cout << "403 Forbidden" << std::endl;
		std::cout << "--------------------------" << std::endl;
		std::string	message;
		std::string	method("GET");
		std::string	uri("/ft_tester/");
		std::string	version("HTTP/1.1");
		message += method + " " + uri + " " + version + "\r\n";
		message += "Authorization: Basic sdafasdf\r\n";
		message += "\r\n";
		std::cout << message << std::endl;
		std::cout << "--------------------------" << std::endl;
		write(clientSocket, message.c_str(), message.length());
		char	buf[14] = {0,};
		ssize_t	ret = read(clientSocket, buf, 13);
		std::string	returnStatusCode = std::string(buf).substr(9, 3);
		std::cout << GRN << "expected 403" << " returned " << returnStatusCode << NC << std::endl;
		std::cout << "--------------------------" << std::endl;
		if (returnStatusCode.compare("403") != 0)
		{
			throw std::exception();
		}
		close(clientSocket);
	}
}

void	test401(const struct sockaddr_in& sockAddr)
{
	std::cout << std::endl << "press enter to continue" << std::endl;
	std::string	tmp;
	std::getline(std::cin, tmp);
	system("clear");
	{
		int	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (connect(clientSocket, (struct sockaddr*)&sockAddr, sizeof(sockAddr)) < 0)
		{
			throw std::exception();
		}
		std::cout << GRNB << "TEST 401 STATUS CODE" << NC << std::endl;
		std::cout << "401 Unauthorized" << std::endl;
		std::cout << "--------------------------" << std::endl;
		std::string	message;
		std::string	method("GET");
		std::string	uri("/ft_tester/");
		std::string	version("HTTP/1.1");
		message += method + " " + uri + " " + version + "\r\n";
		message += "WWW-Authenticate: Basic realm=\"Access to staging site\"\r\n";
		message += "\r\n";
		std::cout << message << std::endl;
		std::cout << "--------------------------" << std::endl;
		write(clientSocket, message.c_str(), message.length());

		char	buf[14] = {0,};
		ssize_t	ret = read(clientSocket, buf, 13);
		std::string	returnStatusCode = std::string(buf).substr(9, 3);
		std::cout << GRN << "expected 401" << " returned " << returnStatusCode << NC << std::endl;
		std::cout << "--------------------------" << std::endl;
		if (returnStatusCode.compare("401") != 0)
		{
			throw std::exception();
		}
		close(clientSocket);
	}
}

void	test408(const struct sockaddr_in& sockAddr)
{
	std::cout << std::endl << "press enter to continue" << std::endl;
	std::string	tmp;
	std::getline(std::cin, tmp);
	system("clear");
	{
		int	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (connect(clientSocket, (struct sockaddr*)&sockAddr, sizeof(sockAddr)) < 0)
		{
			throw std::exception();
		}
		std::cout << GRNB << "TEST 408 STATUS CODE" << NC << std::endl;
		std::cout << "We will do nothing!!" << std::endl;
		std::cout << "--------------------------" << std::endl;
		char	buf[14] = {0,};
		ssize_t	ret = read(clientSocket, buf, 13);
		std::string	returnStatusCode = std::string(buf).substr(9, 3);
		std::cout << GRN << "expected 408" << " returned " << returnStatusCode << NC << std::endl;
		std::cout << "--------------------------" << std::endl;
		if (returnStatusCode.compare("408") != 0)
		{
			throw std::exception();
		}
		close(clientSocket);
	}
}

void	test405(const struct sockaddr_in& sockAddr)
{
	std::cout << std::endl << "press enter to continue" << std::endl;
	std::string	tmp;
	std::getline(std::cin, tmp);
	system("clear");
	{
		int	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (connect(clientSocket, (struct sockaddr*)&sockAddr, sizeof(sockAddr)) < 0)
		{
			throw std::exception();
		}
		std::cout << GRNB << "TEST 405 STATUS CODE" << NC << std::endl;
		std::cout << "When method is POST" << std::endl;
		std::cout << "--------------------------" << std::endl;
		std::string	message;
		std::string	method("POST");
		std::string	uri("/put_test");
		std::string	version("HTTP/1.1");
		message += method + " " + uri + " " + version + "\r\n";
		message += "Content-Length: 0\r\n";
		message += "\r\n";
		std::cout << message << std::endl;
		std::cout << "--------------------------" << std::endl;
		write(clientSocket, message.c_str(), message.length());

		char	buf[14] = {0,};
		ssize_t	ret = read(clientSocket, buf, 13);
		std::string	returnStatusCode = std::string(buf).substr(9, 3);
		std::cout << GRN << "expected 405" << " returned " << returnStatusCode << NC << std::endl;
		std::cout << "--------------------------" << std::endl;
		if (returnStatusCode.compare("405") != 0)
		{
			throw std::exception();
		}
		close(clientSocket);
	}
	{
		int	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (connect(clientSocket, (struct sockaddr*)&sockAddr, sizeof(sockAddr)) < 0)
		{
			throw std::exception();
		}
		std::cout << GRNB << "TEST 405 STATUS CODE" << NC << std::endl;
		std::cout << "When method is DELETE" << std::endl;
		std::cout << "--------------------------" << std::endl;
		std::string	message;
		std::string	method("DELETE");
		std::string	uri("/put_test");
		std::string	version("HTTP/1.1");
		message += method + " " + uri + " " + version + "\r\n";
		message += "HeaderField: hellow~\r\n";
		message += "\r\n";
		std::cout << message << std::endl;
		std::cout << "--------------------------" << std::endl;
		write(clientSocket, message.c_str(), message.length());

		char	buf[14] = {0,};
		ssize_t	ret = read(clientSocket, buf, 13);
		std::string	returnStatusCode = std::string(buf).substr(9, 3);
		std::cout << GRN << "expected 405" << " returned " << returnStatusCode << NC << std::endl;
		std::cout << "--------------------------" << std::endl;
		if (returnStatusCode.compare("405") != 0)
		{
			throw std::exception();
		}
		close(clientSocket);
	}
}

void	test301(const struct sockaddr_in& sockAddr)
{
	std::cout << std::endl << "press enter to continue" << std::endl;
	std::string	tmp;
	std::getline(std::cin, tmp);
	system("clear");
	{
		int	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (connect(clientSocket, (struct sockaddr*)&sockAddr, sizeof(sockAddr)) < 0)
		{
			throw std::exception();
		}
		std::cout << GRNB << "TEST 301 STATUS CODE" << NC << std::endl;
		std::cout << "When request uri is file but, is folder." << std::endl;
		std::cout << "--------------------------" << std::endl;
		std::string	message;
		std::string	method("GET");
		std::string	uri("/put_test");
		std::string	version("HTTP/1.1");
		message += method + " " + uri + " " + version + "\r\n";
		message += "HeaderField: hellow~\r\n";
		message += "\r\n";
		std::cout << message << std::endl;
		std::cout << "--------------------------" << std::endl;
		write(clientSocket, message.c_str(), message.length());

		char	buf[14] = {0,};
		ssize_t	ret = read(clientSocket, buf, 13);
		std::string	returnStatusCode = std::string(buf).substr(9, 3);
		std::cout << GRN << "expected 301" << " returned " << returnStatusCode << NC << std::endl;
		std::cout << "--------------------------" << std::endl;
		if (returnStatusCode.compare("301") != 0)
		{
			throw std::exception();
		}
		close(clientSocket);
	}
}

void	test413(const struct sockaddr_in& sockAddr)
{
	std::cout << std::endl << "press enter to continue" << std::endl;
	std::string	tmp;
	std::getline(std::cin, tmp);
	system("clear");
	{
		int	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (connect(clientSocket, (struct sockaddr*)&sockAddr, sizeof(sockAddr)) < 0)
		{
			throw std::exception();
		}
		std::cout << GRNB << "TEST 413 STATUS CODE" << NC << std::endl;
		std::cout << "When Content-Length 10 but, body 11" << std::endl;
		std::cout << "--------------------------" << std::endl;
		std::string	message;
		std::string	method("PUT");
		std::string	uri("/put_test/temp2");
		std::string	version("HTTP/1.1");
		message += method + " " + uri + " " + version + "\r\n";
		message += "Content-Length: 10\r\n";
		message += "\r\n";
		message += "kkkkkkkkkkk";
		std::cout << message << std::endl;
		std::cout << "--------------------------" << std::endl;
		write(clientSocket, message.c_str(), message.length());

		char	buf[14] = {0,};
		ssize_t	ret = read(clientSocket, buf, 13);
		std::string	returnStatusCode = std::string(buf).substr(9, 3);
		std::cout << GRN << "expected 413" << " returned " << returnStatusCode << NC << std::endl;
		std::cout << "--------------------------" << std::endl;
		if (returnStatusCode.compare("413") != 0)
		{
			throw std::exception();
		}
		close(clientSocket);
	}
	std::cout << std::endl;
	{
		int	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (connect(clientSocket, (struct sockaddr*)&sockAddr, sizeof(sockAddr)) < 0)
		{
			throw std::exception();
		}
		std::cout << GRNB << "TEST 413 STATUS CODE" << NC << std::endl;
		std::cout << "When chunked," << std::endl;
		std::cout << "--------------------------" << std::endl;
		std::string	message;
		std::string	method("PUT");
		std::string	uri("/put_test/414test2");
		std::string	version("HTTP/1.1");
		message += method + " " + uri + " " + version + "\r\n";
		message += "Transfer-Encoding: chunked\r\n";
		message += "\r\n";
		message += "2\r\n";
		message += "kk\r\n";
		message += "3\r\n";
		message += "kkkk\r\n";
		message += "0\r\n";
		message += "\r\n";
		std::cout << message << std::endl;
		std::cout << "--------------------------" << std::endl;
		write(clientSocket, message.c_str(), message.length());

		char	buf[14] = {0,};
		ssize_t	ret = read(clientSocket, buf, 13);
		std::string	returnStatusCode = std::string(buf).substr(9, 3);
		std::cout << GRN << "expected 413" << " returned " << returnStatusCode << NC << std::endl;
		std::cout << "--------------------------" << std::endl;
		if (returnStatusCode.compare("413") != 0)
		{
			throw std::exception();
		}
		close(clientSocket);
	}
	std::cout << std::endl;
}

void	test411(const struct sockaddr_in& sockAddr)
{
	std::cout << std::endl << "press enter to continue" << std::endl;
	std::string	tmp;
	std::getline(std::cin, tmp);
	system("clear");
	{
		int	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (connect(clientSocket, (struct sockaddr*)&sockAddr, sizeof(sockAddr)) < 0)
		{
			throw std::exception();
		}
		std::cout << GRNB << "TEST 411 STATUS CODE" << NC << std::endl;
		std::cout << "When existing Content-Length" << std::endl;
		std::cout << "--------------------------" << std::endl;
		std::string	message;
		std::string	method("PUT");
		std::string	uri("/put_test/temp2");
		std::string	version("HTTP/1.1");
		message += method + " " + uri + " " + version + "\r\n";
		message += "Content-Length: 10\r\n";
		message += "\r\n";
		message += "kkkkkkkkkk";
		std::cout << message << std::endl;
		std::cout << "--------------------------" << std::endl;
		write(clientSocket, message.c_str(), message.length());

		char	buf[14] = {0, };
		ssize_t	ret = read(clientSocket, buf, 13);
		std::string	returnStatusCode = std::string(buf).substr(9, 3);
		std::cout << GRN << "expected 200, 201" << " returned " << returnStatusCode << NC << std::endl;
		std::cout << "--------------------------" << std::endl;
		if (!(returnStatusCode.compare("200") == 0 || returnStatusCode.compare("201") == 0 || returnStatusCode.compare("204") == 0))
		{
			throw std::exception();
		}
		close(clientSocket);
	}
	std::cout << std::endl;
	{
		int	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (connect(clientSocket, (struct sockaddr*)&sockAddr, sizeof(sockAddr)) < 0)
		{
			throw std::exception();
		}
		std::cout << GRNB << "TEST 411 STATUS CODE" << NC << std::endl;
		std::cout << "When not existing Content-Length" << std::endl;
		std::cout << "--------------------------" << std::endl;
		std::string	message;
		std::string	method("PUT");
		std::string	uri("/put_test/414test2");
		std::string	version("HTTP/1.1");
		message += method + " " + uri + " " + version + "\r\n";
		message += "MyHeader: kkk\r\n";
		message += "\r\n";
		message += "kkkkkkkkkk";
		std::cout << message << std::endl;
		std::cout << "--------------------------" << std::endl;
		write(clientSocket, message.c_str(), message.length());

		char	buf[14] = {0, };
		ssize_t	ret = read(clientSocket, buf, 13);
		std::string	returnStatusCode = std::string(buf).substr(9, 3);
		std::cout << GRN << "expected 411" << " returned " << returnStatusCode << NC << std::endl;
		std::cout << "--------------------------" << std::endl;
		if (returnStatusCode.compare("411") != 0)
		{
			throw std::exception();
		}
		close(clientSocket);
	}
	std::cout << std::endl;
}

void	test505(const struct sockaddr_in& sockAddr)
{
	std::cout << std::endl << "press enter to continue" << std::endl;
	std::string	tmp;
	std::getline(std::cin, tmp);
	system("clear");
	{
		int	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (connect(clientSocket, (struct sockaddr*)&sockAddr, sizeof(sockAddr)) < 0)
		{
			throw std::exception();
		}
		std::cout << GRNB << "TEST 505 STATUS CODE" << NC << std::endl;
		std::cout << "When version 1.0" << std::endl;
		std::cout << "--------------------------" << std::endl;
		std::string	message;
		std::string	method("GET");
		std::string	uri("/");
		std::string	version("HTTP/1.0");
		message += method + " " + uri + " " + version + "\r\n";
		message += "\r\n";
		message += "\r\n";
		std::cout << message << std::endl;
		std::cout << "--------------------------" << std::endl;
		write(clientSocket, message.c_str(), message.length());

		char	buf[14] = {0,};
		ssize_t	ret = read(clientSocket, buf, 13);
		std::string	returnStatusCode = std::string(buf).substr(9, 3);
		std::cout << GRN << "expected 200" << " returned " << returnStatusCode << NC << std::endl;
		std::cout << "--------------------------" << std::endl;
		if (returnStatusCode.compare("200") != 0)
		{
			throw std::exception();
		}
		close(clientSocket);
	}
	std::cout << std::endl;
	{
		int	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (connect(clientSocket, (struct sockaddr*)&sockAddr, sizeof(sockAddr)) < 0)
		{
			throw std::exception();
		}
		std::cout << GRNB << "TEST 505 STATUS CODE" << NC << std::endl;
		std::cout << "When version 1.2" << std::endl;
		std::cout << "--------------------------" << std::endl;
		std::string	message;
		std::string	method("GET");
		std::string	uri("/");
		std::string	version("HTTP/1.2");
		message += method + " " + uri + " " + version + "\r\n";
		message += "\r\n";
		message += "\r\n";
		std::cout << message << std::endl;
		std::cout << "--------------------------" << std::endl;
		write(clientSocket, message.c_str(), message.length());

		char	buf[14] = {0,};
		ssize_t	ret = read(clientSocket, buf, 13);
		std::string	returnStatusCode = std::string(buf).substr(9, 3);
		std::cout << GRN << "expected 505" << " returned " << returnStatusCode << NC << std::endl;
		std::cout << "--------------------------" << std::endl;
		if (returnStatusCode.compare("505") != 0)
		{
			throw std::exception();
		}
		close(clientSocket);
	}
	std::cout << std::endl;
}

void	test414(const struct sockaddr_in& sockAddr)
{
	std::cout << std::endl << "press enter to continue" << std::endl;
	std::string	tmp;
	std::getline(std::cin, tmp);
	system("clear");
	{
		int	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (connect(clientSocket, (struct sockaddr*)&sockAddr, sizeof(sockAddr)) < 0)
		{
			throw std::exception();
		}
		std::cout << GRNB << "TEST 414 STATUS CODE" << NC << std::endl;
		std::cout << "When URI length 200" << std::endl;
		std::cout << "--------------------------" << std::endl;
		std::string	message;
		std::string	method("GET");
		std::string	uri(200, '/');
		std::string	version("HTTP/1.1");
		message += method + " " + uri + " " + version + "\r\n";
		message += "\r\n";
		message += "\r\n";
		std::cout << message << std::endl;
		std::cout << "--------------------------" << std::endl;
		write(clientSocket, message.c_str(), message.length());

		char	buf[14] = {0,};
		ssize_t	ret = read(clientSocket, buf, 13);
		std::string	returnStatusCode = std::string(buf).substr(9, 3);
		std::cout << GRN << "expected 200" << " returned " << returnStatusCode << NC << std::endl;
		std::cout << "--------------------------" << std::endl;
		if (returnStatusCode.compare("200") != 0)
		{
			throw std::exception();
		}
		close(clientSocket);
	}
	std::cout << std::endl;
	{
		int	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (connect(clientSocket, (struct sockaddr*)&sockAddr, sizeof(sockAddr)) < 0)
		{
			throw std::exception();
		}
		std::cout << GRNB << "TEST 414 STATUS CODE" << NC << std::endl;
		std::cout << "When URI length 201" << std::endl;
		std::cout << "--------------------------" << std::endl;
		std::string	message;
		std::string	method("GET");
		std::string	uri(201, '/');
		std::string	version("HTTP/1.1");
		message += method + " " + uri + " " + version + "\r\n";
		message += "\r\n";
		message += "\r\n";
		std::cout << message << std::endl;
		std::cout << "--------------------------" << std::endl;
		write(clientSocket, message.c_str(), message.length());

		char	buf[14] = {0,};
		ssize_t	ret = read(clientSocket, buf, 13);
		std::string	returnStatusCode = std::string(buf).substr(9, 3);
		std::cout << GRN << "expected 414" << " returned " << returnStatusCode << NC << std::endl;
		std::cout << "--------------------------" << std::endl;
		if (returnStatusCode.compare("414") != 0)
		{
			throw std::exception();
		}
		close(clientSocket);
	}
	std::cout << std::endl;
}

int	main(int argc, char* argv[])
{
	try
	{
		if (argc < 2)
		{
			std::cerr << "example : " << argv[0] << " 127.0.0.1:8000" << std::endl;
			throw std::exception();
		}
		std::string	argument(argv[1]);
		std::size_t	found = argument.find(":");
		if (found == std::string::npos)
		{
			throw std::exception();
		}
		std::string			host = argument.substr(0, found);
		std::string			port = argument.substr(found + 1);
		struct sockaddr_in	sockAddr;
		sockAddr.sin_family = AF_INET;
		sockAddr.sin_port = htons(std::stoi(port));
		sockAddr.sin_addr.s_addr = inet_addr(host.c_str());
		test414(sockAddr);
		test505(sockAddr);
		test411(sockAddr);
		test413(sockAddr);
		test301(sockAddr);
		test405(sockAddr);
		test401(sockAddr); // Unauthorized
		test403(sockAddr); // Forbidden
		test408(sockAddr);

		// NOTE 구현되었으나 테스터에서 보여주지 않는 것
		// test205(); Reset content
		// test204(); NO content
		// test503();
		// test400();
		// test404();
		// test201();
		// test200();
		// test500();
		std::cout << "TEST OK!!!!" << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cerr << "Something wrong!" << '\n';
	}
}
