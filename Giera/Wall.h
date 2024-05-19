#pragma once
#include "AbstractMapElement.h"
#include "Hitbox.h"
class Wall :
    public AbstractMapElement
{
private:
    static std::string wFolderPath;
    static int wCounter;
    std::unique_ptr <Hitbox> hitbox;
public:
    Wall();
    Wall(std::unique_ptr<Hitbox> hitbox);
    std::string getFilePath() override;
    const std::unique_ptr<Hitbox>& getHitbox() const;

    friend istream& operator >> (istream& in, Wall& w);
};

