#pragma once

#include <vector>
#include <unordered_map>
#include <string>

class VoiceDataBase
{
public:
	using MapType = std::unordered_map<std::string, std::unordered_map<std::string, std::unordered_map<std::string, std::string>>>;

	VoiceDataBase();
	const MapType& Voices();

private:
	std::unordered_map<std::string, std::unordered_map<std::string, std::unordered_map<std::string, std::string>>> voices_;
};
