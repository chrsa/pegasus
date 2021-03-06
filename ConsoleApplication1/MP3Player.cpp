#include "MP3Player.h"
#include <Windows.h>

void MP3Player::play()
{
	mciSendString(L"open teste.mp3 type mpegvideo alias myaudiofileplayht", NULL, 0, 0);
	mciSendString(L"play myaudiofileplayht", NULL, 0, 0);
}

void MP3Player::stop()
{
	mciSendString(L"stop myaudiofileplayht", nullptr, 0, 0);
	mciSendString(L"close myaudiofileplayht", nullptr, 0, 0);
}

bool MP3Player::automatic() const
{
	return automatic_;
}

void MP3Player::enableAutomatic()
{
	automatic_ = true;
}

void MP3Player::disableAutomatic()
{
	automatic_ = false;
}

MP3Player::MP3Player()
	: automatic_{true}
{

}

