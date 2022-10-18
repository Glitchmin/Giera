#pragma once
#include "ItemTypes.h"
#include "NormalItems.h"
#include "BaseItems.h"
#include "BaseItemFileHandler.h"
#include <memory>
#include <vector>
using std::shared_ptr;
using std::vector;


class BaseItemHandler
{
public:
	BaseItemHandler();
private:
	vector <vector <shared_ptr<AbstractBaseItem>>> baseItems;
};

