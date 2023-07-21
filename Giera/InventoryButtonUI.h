#pragma once
#include "ButtonUI.h"
#include "InventoryInputHandler.h"

class EqSlotUIElement;

class InventoryButtonUI :
	public ButtonUI
{
protected:
	shared_ptr <InventoryInputHandler> inventoryInputHandler;
	weak_ptr<AbstractItem> item;
	int itemAlpha = 255;
public:
	InventoryButtonUI(Rect <fr_pos_t> relativePosRect, weak_ptr<AbstractItem> item, EqSlotUIElement* parent,
		double relativeEdgeThickness, shared_ptr <InventoryInputHandler> inventoryInputHandler);
	void handleMouseInput(MouseEventTypes mouseEventType, pair<int, int> pos, Time timeDiff) override;
	void render(shared_ptr<Texture>& textureToDrawOn) override;
	
};

