#ifndef __ERROR_H_
#define __ERROR_H_
#include <iostream>
#include <string>

#define PARSE_ERROR 1
#define FOPEN_ERROR 2
#define MEM_ERROR 3
#define GIT_ERROR 4

enum class serverity
{
	INFO,
	WARNING,
	ERROR,
	FATAL
};

void alert(std::string msg, serverity s, int error_code = 0)
{
	switch (s)
	{
	case serverity::INFO:
		std::cout << "[INFO]: " << msg << std::endl;
		break;
	case serverity::WARNING:
		std::clog << "[WARNING]: " << msg << std::endl;
		break;
	case serverity::ERROR:
		std::cerr << "[ERROR]: " << msg << std::endl;
		break;
	case serverity::FATAL:
		std::cerr << "[FATAL ERROR]: " << msg << std::endl;
		if (error_code == 0)
			exit(-1);
		else
			exit(error_code);
		break;
	}
}
#endif 
