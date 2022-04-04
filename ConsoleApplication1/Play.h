#pragma once

#include "Http.h"
#include "Json.h"
#include <string>

class Play
{

public:
	Play();

	std::string transcribe(std::string Text);

	void login(const std::string& email, const std::string& password);

private:
	const std::string login_url_;
	Http http_;
	std::string localId_;
};

