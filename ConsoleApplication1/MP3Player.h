#pragma once
class MP3Player
{
public:

	MP3Player();

	void play();
	void stop();

	bool automatic() const;

	void enableAutomatic();
	void disableAutomatic();

private:

	bool automatic_;
};

