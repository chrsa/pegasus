#include "Play.h"
#include <regex>

Play::Play()
:login_url_{"https://www.googleapis.com/identitytoolkit/v3/relyingparty/verifyPassword?key=AIzaSyDIYBezlQ0hOhLvY2Wn0nPPhiNQsXdV-Js"}
{
}

std::string Play::transcribe(std::string Text)
{
	std::replace(Text.begin(), Text.end(), '\r', ' ');
	std::replace(Text.begin(), Text.end(), '\n', ' ');
	std::replace(Text.begin(), Text.end(), '\t', ' ');
	std::replace(Text.begin(), Text.end(), '\v', ' ');
	std::replace(Text.begin(), Text.end(), '\b', ' ');
	std::replace(Text.begin(), Text.end(), '\f', ' ');
	std::replace(Text.begin(), Text.end(), '\a', ' ');


	std::string postData = "{\"userId\":\"" + localId_ + "\",\"ssml\":\"<speak><p>" + Text + "</p></speak>\",\"voice\":\"en-US-JennyNeural\",\"narrationStyle\":\"regular\",\"globalSpeed\":\"100%\",\"globalVolume\":\"+0dB\",\"pronunciations\":[],\"platform\":\"dashboard\"}";

	std::string fileToDownload = http_.request("https://play.ht/api/transcribe", postData.c_str());
	
	if (fileToDownload.empty())
	{
		return "";
	}

	nlohmann::json json = nlohmann::json::parse(fileToDownload);

	std::string fileUrl = json["file"].get<std::string>();

	std::string mp3 = http_.request(fileUrl, "");

	return mp3;
}

void Play::login(const std::string &email, const std::string &password)
{
	std::string response = http_.request(login_url_, "{\"email\":\"" + email + "\",\"password\":\"" + password + "\",\"returnSecureToken\":true}");

	nlohmann::json json = nlohmann::json::parse(response);

	localId_ = json["localId"].get<std::string>();
}
