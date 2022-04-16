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