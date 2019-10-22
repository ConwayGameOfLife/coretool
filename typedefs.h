#ifndef __TYPEDEFS_H_
#define __TYPEDEFS_H_
#include <vector>
#include <string>

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
};
#endif