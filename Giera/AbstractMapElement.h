#pragma once
#include <string>
#include <sstream>
#include "GameTime.h"
#include "TextureLoader.h"

class AbstractMapElement
{
protected:
	static inline std::string txFolderPath = "../../save_files/tx/";
	static inline int tilesPerMeter = 1;
	unsigned int elementID;
	Time updateDelay;
	int statesNumber;
	shared_ptr<Texture> texture;
	double width; //in map tiles
	double height; //in map tiles
public:
	AbstractMapElement();
	AbstractMapElement(unsigned int framesNumber, Time refreshTime);
	virtual std::string getFilePath()=0;
	friend class MapElementsFileHandler;
	friend istream& operator >> (istream& is, AbstractMapElement& t);
	static int getTilesPerMeter();
	static void setTilesPerMeter(int tilesPerMeter);
    double getWidth() const;
    double getHeight() const;

};

