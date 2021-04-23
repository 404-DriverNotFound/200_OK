#include "../base_headers/Socket.hpp"
#include <string.h> // memset forbidden
#include <fstream>
#include <sstream>
/*
?? port 와 ip 로 들어온 것이 Socket 에서 무슨 역할인지 모르겠다.
Socket::Socket(uint16_t port, uint32_t ip)
{
}
*/

/* Socket 에 이런식으로 넣으면 binding, listen 작업까지 */
Socket::Socket(uint16_t port, uint32_t ip)
{
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd == -1) {
		std::cout << "Failed to create socket. errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}
	SetAddr();
	Bind(port, ip);
	Listen();
}

Socket::Socket(int fd)
{
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd == -1)
	{
		std::cout << "Failed to create socket. errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}
}

Socket::~Socket()
{
	std::cout << "~Socket()\n";
}

void					Socket::SetAddr(void)
{
	memset((char*)&sockaddr, 0, sizeof(sockaddr));
}

void					Socket::Bind(uint16_t port, uint32_t ip)
{
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons(port); // htons is necessary to convert a number to
	sockaddr.sin_addr.s_addr = ip; // localhost
	// network byte order
	if (bind(fd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0) {
		std::cout << "Failed to bind to port " << port << ". errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}
	std::cout << "finished binding\n";
}

void					Socket::Listen(void)
{
	// Start listening. Hold at most 10 connections in the queue
	if (listen(fd, 10) < 0)
	{
		std::cout << "Failed to listen on socket. errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}
	std::cout << "finished listening\n";
}

size_t					Socket::Accept(size_t connections)
{
	// Grab a connection from the queue
	socklen = sizeof(sockaddr);
	connections = accept(fd, (struct sockaddr*)&sockaddr, (socklen_t*)&socklen);
	if (connections < 0)
	{
		std::cout << "Failed to grab connection. errno: " << errno << std::endl;
		exit(EXIT_FAILURE);
	}
	std::cout << "finished accepting\n";
	return (connections);
}

int						Socket::GetFd(void)
{
	return (fd);
}