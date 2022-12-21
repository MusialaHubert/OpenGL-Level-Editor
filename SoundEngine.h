#pragma once
#include <irrKlang.h>
#include <string>

class SoundEngine
{
public:
    SoundEngine();
    ~SoundEngine();
    void Play(std::string fileLocation, bool isLooping) const;
    
};
