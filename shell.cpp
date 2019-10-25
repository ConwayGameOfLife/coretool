#include "shell.h"
#include <iomanip>

shell::shell() { loaded = not_loaded = 0; }
int shell::parse(const std::string& cmd, life& l)
{
	shlog sl;
	if (cmd == "reload")
	{
		l.fbuf.clear();
		this->not_loaded = this->loaded = 0;

		for (size_t pos = 0; pos < l.still.size(); pos++)
		{
			std::string tmp;
			if (read_all(l.still[pos].file, tmp))
			{
				sl.alert("unable to load file \"" + l.still[pos].file + "\"", shlog::serverity::WARNING);
				this->not_loaded++;
			}
			else
			{
				sl.alert("loaded file \"" + l.still[pos].file + "\"", shlog::serverity::INFO);
				this->loaded++;
			}
		}
		sl.alert(std::to_string(this->loaded) + " files loaded, " + std::to_string(this->not_loaded) + " files not loaded", shlog::serverity::INFO);
	}
	else if (cmd == "help")
	{
		std::cout << "help manual (it sort of like MC commands):" << std::endl
			<< TAB << BLD << GRN << "help" << NRM << ": displays the helpful message" << std::endl
			<< TAB << BLD << GRN << "reload" << NRM << ": reloads all life json files" << std::endl
			<< TAB << BLD << GRN << "exit" << NRM << ": if you want to leave this great app, use this!" << std::endl
			<< TAB << BLD << GRN << "sync" << NRM << ": syncs files from link" << std::endl;;
	}
	else if (cmd == "");
	else if (cmd == "exit");
	else
		std::cout << CYN << "CISsh: " << NRM << "invalid command\n";
	return 0;
}

int shell::call(const std::string& cmd)
{
	if (system(cmd.c_str()))
		return 1;
	else
		return 0;
}

int shell::sh(std::string& version, life& l)
{
	shlog sl;
	std::cout << GRN << "CIS isn't special! " << NRM << "(" << version << ")" << std::endl;
	std::string cmd;
	while (cmd != "exit")
	{
		std::cout << CYN << "CIS shell: " << GRN << version << NRM << "$ ";
		std::getline(std::cin, cmd);
		this->parse(cmd, l);
	}
	return 0;
}

void shlog::alert(std::string msg, serverity s, int error_code)
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

file::file()
{
	this->handle = nullptr;
}

file::file(const file& rhs)
{
	if (rhs.handle == nullptr)
		this->handle = nullptr;
	else
	{
		this->handle = fopen(rhs.filename.c_str(), "r+");
		this->filename = rhs.filename;
	}
}

file::file(const std::string& fname)
{
	this->open(fname);
}

int file::open(const std::string& fname)
{
	this->~file();
	this->handle = fopen(fname.c_str(), "r+");
	this->filename = fname;
	if (handle != nullptr)
		return 0;
	else
		return 1;
}

std::string file::readall()
{
	if (this->handle == nullptr)
		throw std::logic_error("handle is null");

	std::string buf;
	size_t size;

	fseek(this->handle, 0, SEEK_END);
	size = ftell(this->handle);
	buf.resize(size + 10);
	rewind(this->handle);

	fread((void*)buf.data(), 1, size, this->handle);
	return buf;
}

bool file::is_open()
{
	if (this->handle == nullptr)
		return 0;
	else
		return 1;
}

void file::close()
{
	if (this->handle == nullptr)
		return;
	else
		fclose(this->handle);
}

file::~file()
{
	this->close();
}

protocol_data getprotocol(const nlohmann::json& j)
{
	shlog l;
	protocol_data protocol;
	try
	{
		protocol.version = j["protocol"]["version"].get<std::string>();
		protocol.format = j["protocol"]["format"].get<int>();
		protocol.conf = j["protocol"]["conf"].get<std::string>();
		protocol.r.born = j["protocol"]["rule"]["born"].get<std::vector<int>>();
		protocol.r.survive = j["protocol"]["rule"]["survive"].get<std::vector<int>>();
		protocol.md_file = j["protocol"]["md"].get<std::string>();
		protocol.url = j["protocol"]["url"].get<std::string>();
	}
	catch (std::exception & e)
	{
		l.alert((std::string)"parse error on \"core.json\": " + e.what(), shlog::serverity::FATAL, shlog::PARSE_ERROR);
	}
	return protocol;
}

int read_all(const std::string& name, std::string& buf)
{
	file f(name);
	if (!f.is_open())
		return 1;
	else
		buf = f.readall();
	return 0;
}