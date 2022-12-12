#pragma once
#include "AbstractMapElement.h"
class Foreground :
    public AbstractMapElement
{
private:
    static std::string fgFolderPath;
    static int fgCounter;
public:
    std::string getFileName() override;
    Foreground();
    virtual void draw(Texture& TextureToDrawOn, double& pixelToMeterRatio) override;
};

