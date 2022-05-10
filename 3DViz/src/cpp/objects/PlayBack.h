#pragma once

#include "AudioObject.h"

class PlayBack
{
public:

	PlayBack(string const& path);
	~PlayBack() {};

	bool Init();
	void Update();
	void Start();
	void Pause();
	void UnPause();
	bool IsPlaying();

	AudioObject* bass;
	AudioObject* drums;
	AudioObject* other;
	AudioObject* vocals;

private:

	string		folderPath;
};

