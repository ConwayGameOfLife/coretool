#define _CRT_SECURE_NO_WARNINGS
#pragma warning (disable: 6387)
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdio>
#include "json.h"
#include "errors.h"
#include "typedefs.h"



using namespace nlohmann;
using namespace std; 

string read_all(string filename)
{
	FILE* fp = fopen(filename.c_str(), "r+");
	string buf;
	if (fp == 0)
		throw invalid_argument("unable to open file");

	fseek(fp, 0, SEEK_END);
	size_t size = ftell(fp);
	buf.resize(size + 10);
	rewind(fp);

	fread((void*)buf.data(), 1, size, fp);
	if (buf.data() == nullptr)
		alert("runtime memeory error", serverity::FATAL, 3);
	fclose(fp);

	return buf;
}


int main()
{
	/*
	int ret = system("git pull origin master"); //try to sync files
	if (ret != 0)
		alert("unable to sync files form github. Make sure to config git", serverity::FATAL, GIT_ERROR);*/

	string buf; //buffer for "core.json" file
	try
	{
		buf = read_all("core.json"); //try to read file
	}
	catch (exception & e)
	{
		alert("unable to open file \"core.json\"", serverity::FATAL, FOPEN_ERROR); //error handling
	}

	fstream handle("core.json"); //c++ stream for json
	if(!handle.is_open()) //error handling
		alert("unable to open file \"core.json\"", serverity::FATAL, FOPEN_ERROR);

	json j;
	protocol_data protocol;
	try
	{
		stringstream ss(buf);
		ss >> j;
		protocol.version = j["protocol"]["version"].get<string>();
		protocol.format = j["protocol"]["format"].get<int>();
		protocol.conf = j["protocol"]["conf"].get<string>();
		protocol.r.born = j["protocol"]["rule"]["born"].get<vector<int>>();
		protocol.r.survive = j["protocol"]["rule"]["survive"].get<vector<int>>();
	}
	catch (exception & e)
	{
		alert((string)"parse error con \"core.json\": " + e.what(), serverity::FATAL, PARSE_ERROR);
	}

	alert("loaded core file!", serverity::INFO);
	alert("loaded version: " + protocol.version + ", file format: " + protocol.conf, serverity::INFO);
	
	alert("loading still life files...", serverity::INFO);
	vector<string> path;
	vector<string> fbuf;
	int loaded = 0;
	int not_loaded = 0;

	try
	{
		path = j["still_life"]["path"].get<vector<string>>();
		for (size_t pos = 0; pos < path.size(); pos++)
		{
			try
			{
				fbuf.push_back(read_all(path[pos]));
				loaded++;

			}
			catch (exception & e)
			{
				alert("unable to load file \"" + path[pos] + "\"", serverity::WARNING);
				not_loaded++;
			}
		}
	}
	catch(exception & e)
	{
		alert("cannot find file-paths to still lives", serverity::ERROR);
	}

	alert(to_string(loaded) + " files loaded, " + to_string(not_loaded) + " files not loaded", serverity::INFO);

	return 0;
}