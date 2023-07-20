#pragma once
#include "ButtonUI.h"
#include "InventoryInputHandler.h"
class InventoryButtonUI :
	public ButtonUI
{
protected:
	shared_ptr <InventoryInputHandler> inventoryInputHandler;
	weak_ptr<AbstractItem> item;
public:
	InventoryButtonUI(Rect <fr_pos_t> relativePosRect, weak_ptr<AbstractItem> item, UIElement* parent,
		double relativeEdgeThickness, shared_ptr <InventoryInputHandler> inventoryInputHandler);
	void handleMouseInput(MouseEventTypes mouseEventType, pair<int, int> pos, Time timeDiff) override;
	
};

