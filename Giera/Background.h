#pragma once
#include "AbstractMapElement.h"
class Background :
    public AbstractMapElement
{
private:
    static std::string bgFolderPath;
    static int bgCounter;
public:
    std::string getFileName() override;
    Background();
    Background(unsigned int framesNumber, Time refreshTime);
    virtual void draw(Texture& TextureToDrawOn,const double& pixelToMeterRatio,const Position& posOnMap) override;
    
};

