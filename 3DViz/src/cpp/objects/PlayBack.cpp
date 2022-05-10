#include "PlayBack.h"

PlayBack::PlayBack(string const& path)
{
	folderPath = path;
	bass = new AudioObject(folderPath + "bass.wav", BUFFER_SIZE);
	drums = new AudioObject(folderPath + "drums.wav", BUFFER_SIZE);
	vocals = new AudioObject(folderPath + "vocals.wav", BUFFER_SIZE);
	other = new AudioObject(folderPath + "other.wav", BUFFER_SIZE);
}

bool PlayBack::Init()
{
	return bass->Init() && drums->Init() && vocals->Init() && other->Init();
}

void PlayBack::Start()
{
	bass->PlaySound();
	drums->PlaySound();
	vocals->PlaySound();
	other->PlaySound();
}

void PlayBack::Pause()
{
	bass->Pause();
	drums->Pause();
	vocals->Pause();
	other->Pause();
}

void PlayBack::UnPause()
{
	bass->UnPause();
	drums->UnPause();
	vocals->UnPause();
	other->UnPause();
}

bool PlayBack::IsPlaying()
{
	return bass->IsPlaying() && drums->IsPlaying() && vocals->IsPlaying() && other->IsPlaying();
}

void PlayBack::Update()
{
	bass->Update();
	drums->Update();
	vocals->Update();
	other->Update();
}
