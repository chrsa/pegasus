#include "MP3Player.h"
#include <Windows.h>

void MP3Player::play()
{
	mciSendStringA("open teste.mp3 type mpegvideo alias myaudiofileplayht", NULL, 0, 0);
	mciSendStringA("play myaudiofileplayht", NULL, 0, 0);
}

void MP3Player::stop()
{
	mciSendStringA("stop myaudiofileplayht", nullptr, 0, 0);
	mciSendStringA("close myaudiofileplayht", nullptr, 0, 0);
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

