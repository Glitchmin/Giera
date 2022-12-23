#pragma once
#include <string>
#include <sstream>
#include "Time.h"
#include "Sprite.h"
class AbstractMapElement: public Sprite
{
protected:
	static inline std::string txFolderPath = "../../save_files/tx/";
	static inline int tilesPerMeter = 1;
	unsigned int elementID;
	unsigned int framesNumber;
	Time refreshTime;
public:
	virtual std::string getFileName()=0;
	virtual void draw(Texture& textureToDrawOn, const double& pixelToMeterRatio, const Position& posOnMap) override;
	AbstractMapElement();
	AbstractMapElement(unsigned int framesNumber, Time refreshTime);
	friend class MapElementsFileHandler;
	friend istream& operator >> (istream& is, AbstractMapElement& t);
	static int getTilesPerMeter();
	static void setTilesPerMeter(int tilesPerMeter);

};

