#pragma once

#include <string>

class Clipboard
{
public:

	std::string read();
	std::string last();
private:

	std::string lastText_;

};

