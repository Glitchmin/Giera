#pragma once
#include "ItemTypes.h"
#include "NormalItems.h"
#include "BaseItems.h"
#include <memory>
#include <vector>
using std::shared_ptr;
using std::vector;

class BaseItemHandler
{
public:
	static void initialize();
	template <class T> static shared_ptr<T> generate(ItemTypes itemType, int itemSubtype);
private:
	static inline vector <vector <shared_ptr<AbstractBaseItem>>> baseItems;
};

template <class T> shared_ptr<T> BaseItemHandler::generate(ItemTypes itemType, int itemSubtype) {
	BaseItemHandler::initialize();
	auto tmp = (baseItems[(int)itemType][itemSubtype]->generate());
	 return std::dynamic_pointer_cast<T>(tmp);
}

