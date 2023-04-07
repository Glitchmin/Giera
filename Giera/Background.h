#pragma once
#include "AbstractMapElement.h"
class Background :
    public AbstractMapElement
{
private:
    static std::string bgFolderPath;
    static int bgCounter;
public:
    std::string getFilePath() override;
    Background();
    Background(unsigned int framesNumber, Time refreshTime);

    friend istream& operator >> (istream& in, Background& bg);
};

