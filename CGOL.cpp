#define _CRT_SECURE_NO_WARNINGS
#pragma warning (disable: 6387)
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdio>
#include "shell.h"

using namespace nlohmann;
using namespace std; 
int main()
{
#ifndef _WIN32
	if (syscall("git pull origin master")) //try to sync files
		l.alert("unable to sync files form github. Make sure to config git", shlog::serverity::FATAL, GIT_ERROR); //error message
#endif
	shlog l;
	string buf; //buffer for "core.json" file
	//file f("core.json"); //open file core.json
	shell sh; //shell class
	protocol_data protocol; //protocol
	json core; //json class

	read_all("core.json", buf); //read all contents of core.json to buf

	try
	{
		core = json::parse(buf); //parse contents of buf
		protocol = getprotocol(core); //get the protocol of core.json
	}
	catch (...)
	{
		l.alert("unable to parse file \"core.json\"", shlog::serverity::FATAL, shlog::PARSE_ERROR); //error msg
	}
	
	l.alert("loaded core file!", shlog::serverity::INFO); 
	l.alert("loaded version: " + protocol.version + ", file format: " + protocol.conf, shlog::serverity::INFO);
	l.alert("loading still life files...", shlog::serverity::INFO);

	vector<path_t> paths;
	life lifes;

	try
	{
		vector<json> path = core["still_life"]["path"].get<vector<json>>();
		for (size_t pos = 0; pos < path.size(); pos++)
		{
			paths.push_back({ path[pos]["path"].get<string>(),  path[pos]["file"].get<string>() });
		}

		lifes = { protocol, paths };
		sh.parse("reload", lifes);
	}
	catch(...)
	{
		l.alert("cannot find file-paths to still lives", shlog::serverity::ERROR);
	}
	sh.sh(protocol.version, lifes);
	return 0;
}