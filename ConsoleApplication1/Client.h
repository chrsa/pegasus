#pragma once

#include "Play.h"
#include "Clipboard.h"

class Client
{
public:
	Play& play();
	Clipboard &clipboard();

private:

	Play play_;
	Clipboard clipboard_;
};

