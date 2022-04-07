#pragma once

#include "Http.h"
#include "Json.h"
#include <string>

class Play
{

public:
	Play();

	int transcribe(std::string Text, std::string &mp3);

	int login(const std::string& email, const std::string& password);

private:
	const std::string login_url_;
	Http http_;
	std::string localId_;
};

