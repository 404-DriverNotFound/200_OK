#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <vector>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/time.h>
#include <fcntl.h>
#include <arpa/inet.h>

namespace ft
{
	void			testNameSpace(void);
	void*			memset(void *b, int c, size_t len);
	void			split_vector(std::vector<std::string> &vec, const std::string& str, const char *delim);
	std::string		ReplaceAll(std::string &str, const std::string& from, const std::string& to);
	std::string		ReplaceAll_modified(std::string &str, const std::string& from, const std::string& to);

	std::string		itos(int n);
	// int				atoi(const char *str); // NOTE std::atoi으로 대체가능 <iostream>

	bool access(std::string absolute_path);
	bool isFilePath(const std::string &path);
	bool isDirPath(const std::string &path);
	std::string makeAutoindexHTML(std::string root);

	std::string getHTTPTimeFormat(time_t time);
	std::string getCurrentTime();

	std::vector<int> getVector_changedFD(fd_set *fdset, size_t fdset_size);

	// 서브젝트 허용 매크로함수로 변경됨.
	u_int32_t ft_htonl(u_int32_t ip_addr);
	u_int16_t ft_htons(u_int16_t port);
	in_addr_t ft_inet_addr(const char * ip_address);

	unsigned long	stohex(const std::string &str);

	std::string		inet_ntos(struct in_addr in);


	std::string getBody_from_file(std::string uri_plus_file);
	std::string getBody_from_fd(int fd);
}

#endif