#pragma once
#include <string>
#include <sstream>
#include "Time.h"
#include "Sprite.h"
class AbstractMapElement: public Sprite
{
protected:
	static inline std::string txFolderPath = "../../tx/";
	static inline int tilesPerMeter = 1;
	unsigned int elementID;
	unsigned int framesNumber;
	Time refreshTime;
public:
	virtual std::string getFileName()=0;
	AbstractMapElement();
	AbstractMapElement(unsigned int framesNumber, Time refreshTime);
	friend class MapElementsFileHandler;
};

