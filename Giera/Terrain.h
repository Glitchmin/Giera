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

    friend istream& operator >> (istream& is, Terrain& t);
};

