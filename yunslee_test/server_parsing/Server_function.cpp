#include "../all_header.hpp"
#include "./server.hpp"
#include "../gnl/get_next_line_bonus.hpp"

typedef int FD;

int error_return(char const * str)
{
	cout << str << endl;
	exit(EXIT_FAILURE);
	return (-1);
}

void setPath(std::vector<std::string> &vec, const std::string& str)
{
	size_t before = 0; //string처음부터 검사
	size_t after = 0;
	std::string substr;
	while((after = str.find("/", before)) != std::string::npos)  //from을 찾을 수 없을 때까지
	{
		substr = str.substr(before, after - before);
		cout << "   str: " << str << endl;
		cout << "substr: " << substr << endl;
		if (substr.size() != 0)
			vec.push_back(substr);
		before = after + 1;
	}

	// usr/yunslee/index.html 마지막을 넣어주기 위해
	if (before < str.size())
	{
		substr = str.substr(before, after - 1);
		vec.push_back(substr);
		before = after + 1;
	}
}

const int eDirective_num = 12;
enum eDirective
{
	ROOT, SERVER_NAME, PORT, ERROR_PAGE, INDEX,
	HEAD_LENGTH, BODY_LENGTH, AUTOINDEX, TIMEOUT, AUTH,
	LOCATION, METHOD
};

std::string server_directives[12] = {"\troot ", "\tserver_name ", "\tport ", "\terro_page ", "\tindex ",
									"\thead_length ", "\tbody_length ", "\tautoindex ", "\ttimeout ", "\tauth "
										, "\tlocation ", "\tmethod "};
std::string location_directives[12] = {"\t\troot ", "\t\tserver_name ", "\t\tport ", "\t\terro_page ", "\t\tindex ",
									"\t\thead_length ", "\t\tbody_length ", "\t\tautoindex ", "\t\ttimeout ", "\t\tauth "
										, "\t\tlocation ", "\t\tmethod "};

void outdentTab(std::string &str)
{
	if (str.size() < 2 || str[0] != '\t')
		return ;
	std::string temp;
	temp.clear();
	for (size_t i = 1; i < str.size(); i++)
	{
		temp.append(1, str[i]);
	}
	str = temp;
	return ;
}

int setDefaultServer(Config &config, std::string &oneline)
{
	
}

int registerDefaultServer(Server &servers, Config &config)
{
	
}

bool isValidLocationBlock(std::string &oneline)
{
	
}

int parsingServerBlock(Server &servers, std::vector<std::string> &gnl)
{
	std::string oneline;
	int flag_server = 0;
	int i = 0;
	while (i < gnl.size())
	{
		if (gnl[i][0] != '\t' && gnl[i].compare("server") == 0) // server block
		{
			Config config; //default webserv 생성
			i++; // 다음 줄로 이동
			oneline = gnl[i];
			while (oneline[0] == '\t' && (oneline[0] != '\n' && oneline[1] != '\n')
					&& i < gnl.size())
			{
				outdentTab(oneline);
				// default 웹서버 탄생
				setDefaultServer(config, oneline);
				i++;
			}
			if (i >= gnl.size())
			{
				// default 서버하나 생성
			}
			else if (oneline[0] == '\n')
			{
				// 다음 서버를 찾으려 가자
			}
			else if (oneline[0] == '\t' && oneline[1] == '\n')
			{
				i++;
				if (i < gnl.size())
					oneline = gnl[i];
				else
				{
					registerDefaultServer(servers, config);
					return (1);
				}
				while (oneline[0] == '\t' && i < gnl.size())
				{
					outdentTab(oneline);
					// outdentTab(oneline);
					if (isValidLocationBlock(oneline) == true)
					{
						Config config2;
						i++;
						if (i < gnl.size())
							oneline = gnl[i];
						else
						{
							registerDefaultServer(servers, config);
							config2 = config;
							registerDefaultServer(servers, config2);
							return (1);
						}
						while (oneline[0] == '\t' && oneline[1] == '\t')
						{
							outdentTab(oneline);
							outdentTab(oneline);
							/* code */
							i++;
							if (i < gnl.size())
								oneline = gnl[i];
							else
							{
								registerDefaultServer(servers, config);
								config2 = config;
								registerDefaultServer(servers, config2);
								return (1);
							}
						}
					}
					else
					{
						//에러처리
						// return ;
					}
					i++;
					if (i < gnl.size())
						oneline = gnl[i];
					else
						return (1);
				}
				
			}
			else if (oneline[0] != '\t')
			{
				// error 처리
				break ; 
			}
			else
			{
				// error 처리
				break ; 
			}
			//default webserv 등록
			registerDefaultServer(servers, config);
		}
		i++;
	}
	return (-1);
}

int main()
{
	Server server;
	FD server_config;
	char buffer[BUFSIZ];
	server_config = open("./server_config", O_RDONLY, 0755);
	if (server_config < 0)
		return (error_return("fd open error"));
	int read_size;
	std::vector<std::string> gnl;
	char *line;
	while (get_next_line(server_config, &line) > 0)
	{
		gnl.push_back(line);
		free(line);
	}
	gnl.push_back(line);
	free(line);
	
	for (size_t i = 0; i < gnl.size(); i++)
	{
		cout << gnl[i] << endl;
	}
	cout << gnl.size() << endl;

	if (-1 == parsingServerBlock(server, gnl))
		return (error_return("no server block"));

	close(server_config);
	return (1);
}
