#pragma once
#include "AbstractMapElement.h"
#include "AbstractGeometryFigure.h"
#include <memory>
class Wall :
    public AbstractMapElement
{
private:
    static std::string wFolderPath;
    static int wCounter;
    std::unique_ptr<AbstractGeometryFigure> hitbox;
public:
    Wall();
    Wall(std::unique_ptr<AbstractGeometryFigure>&);
    std::string getFileName() override;
    const std::unique_ptr<AbstractGeometryFigure>& getHitbox() const;
    virtual void draw(Texture& TextureToDrawOn, double& pixelToMeterRatio,const Position& posOnMap) override;

    friend class MapElementsFileHandler;
};

