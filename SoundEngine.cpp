#include "SoundEngine.h"

static irrklang::ISoundEngine* soundEngine;

SoundEngine::SoundEngine()
{
    if(soundEngine == nullptr)
    {
        soundEngine = irrklang::createIrrKlangDevice();
    }
}

SoundEngine::~SoundEngine()
{
    delete soundEngine;
}

void SoundEngine::Play(std::string fileLocation, bool isLooping) const
{
    soundEngine->play2D(fileLocation.c_str(), isLooping);
}
