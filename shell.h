#ifndef __SHELL_H__
#define __SHELL_H__
#define _CRT_SECURE_NO_WARNINGS

#include "json.h"
#include <iostream>
#include <string>
#include <stdexcept>

#define BLK "\x1b[30m"
#define RED "\x1b[31m"
#define GRN "\x1b[32m"
#define YEL "\x1b[33m"
#define BLU "\x1b[34m"
#define MAG "\x1b[35m"
#define CYN "\x1b[36m"
#define WHT "\x1b[37m"
#define NRM "\x1b[0m"
#define TAB "\t"
#define BLD "\x1b[1m"

struct rule
{
	std::vector<int> born;
	std::vector<int> survive;
};
struct protocol_data
{
	std::string version;
	int format;
	std::string conf;
	rule r;
	std::string url;
	std::string md_file;
};
struct path_t
{
	std::string path;
	std::string file;
};

class life
{
public:
	protocol_data protocol;
	std::vector<path_t> still;
	std::vector<std::string> fbuf;
};

class shell
{
	int loaded, not_loaded;

public:
	shell();
	int parse(const std::string& cmd, life& l); //parse given command
	int call(const std::string& cmd); //call system()
	int sh(std::string& version, life& l);
};

class shlog
{
public:
	static const int PARSE_ERROR = 1;
	static const int FOPEN_ERROR = 2;
	static const int MEM_ERROR = 3;
	static const int GIT_ERROR = 4;

	enum class serverity
	{
		INFO,
		WARNING,
		ERROR,
		FATAL
	};
	void alert(std::string msg, serverity s, int error_code = 0); //logs data
};

class file
{
	FILE* handle;
	std::string filename;

public:
	file(); //defualt constructor
	file(const file& rhs); //copy constructer
	file(const std::string& fname); //open file
	int open(const std::string& fname); //open file
	std::string readall(); //read all contents of file
	bool is_open(); //check to see if file is open
	void close(); //close file
	~file(); //close file
};

class markdown
{
	std::string mdbuf;

public:

	inline std::string genHTMLflag(const std::string& key, const std::string& value) //generatr html flags
	{
		return (std::string)" " + key + "=" + "\"" + value + "\"";
	}

	inline std::string genHTMLimg(const std::string& url, const std::string& alt, int width) //generate image tag
	{
		return (std::string)"<img" +
			genHTMLflag("src", url) +
			genHTMLflag("alt", alt) +
			genHTMLflag("width", std::to_string(width) + "px") +
			genHTMLflag("height", "auto") +
			">";
	}

	inline std::string dump() { return this->mdbuf; } //dump md contents
};

protocol_data getprotocol(const nlohmann::json& j); //get the protocol data from a json class

int read_all(const std::string& name, std::string& buf);

#endif