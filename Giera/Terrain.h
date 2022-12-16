#pragma once
#include "AbstractMapElement.h"
class Terrain :
    public AbstractMapElement
{
private:
    static std::string trFolderPath;
    static int trCounter;
public:
    std::string getFileName() override;
    Terrain();
    virtual void draw(Texture& TextureToDrawOn, const double& pixelToMeterRatio,const Position& posOnMap) override;

    friend istream& operator >> (istream& is, Terrain& t);
};

