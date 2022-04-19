#include "Play.h"
#include <regex>

Play::Play()
:login_url_{"https://www.googleapis.com/identitytoolkit/v3/relyingparty/verifyPassword?key=AIzaSyDIYBezlQ0hOhLvY2Wn0nPPhiNQsXdV-Js"}
{
}

int Play::transcribe(std::string Text, std::string &mp3)
{
	std::replace(Text.begin(), Text.end(), '\r', ' ');
	std::replace(Text.begin(), Text.end(), '\n', ' ');
	std::replace(Text.begin(), Text.end(), '\t', ' ');
	std::replace(Text.begin(), Text.end(), '\v', ' ');
	std::replace(Text.begin(), Text.end(), '\b', ' ');
	std::replace(Text.begin(), Text.end(), '\f', ' ');
	std::replace(Text.begin(), Text.end(), '\a', ' ');


	std::string postData = "{\"userId\":\"" + localId_ + "\",\"ssml\":\"<speak><p>" + Text;
	postData += "</p></speak>\",\"voice\":\"" + voice_ + "\", \"narrationStyle\":\"regular\",\"globalSpeed\":\"100%\",\"globalVolume\":\"+0dB\",\"pronunciations\":[],\"platform\":\"dashboard\"}";

	int httpStatus;

	std::string fileToDownload = http_.request(httpStatus, "https://play.ht/api/transcribe", postData.c_str());
	
	if (httpStatus != 200)
	{
		return httpStatus;
	}

	if (fileToDownload.empty())
	{
		return -1;
	}

	nlohmann::json json = nlohmann::json::parse(fileToDownload);

	std::string fileUrl = json["file"].get<std::string>();

	mp3 = http_.request(httpStatus, fileUrl, "");

	return httpStatus;
}

int Play::login(const std::string &email, const std::string &password)
{
	int httpCode;

	std::string response = http_.request(httpCode, login_url_, "{\"email\":\"" + email + "\",\"password\":\"" + password + "\",\"returnSecureToken\":true}");

	if (httpCode == 200)
	{
		nlohmann::json json = nlohmann::json::parse(response);
		localId_ = json["localId"].get<std::string>();
	}

	return httpCode;
}

void Play::ConfigureVoice(const std::string& voice)
{
	voice_ = voice;
}
