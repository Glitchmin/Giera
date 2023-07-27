#pragma once
#include "ButtonUI.h"
#include "InventoryInputHandler.h"

class EqSlotUIElement;

class InventoryButtonUI :
	public ButtonUI
{
protected:
	shared_ptr <InventoryInputHandler> inventoryInputHandler;
	optional <weak_ptr<AbstractItem>> item;
	static inline int maxItemAlpha = 255;
	int itemAlpha = maxItemAlpha;
public:
	InventoryButtonUI(Rect <fr_pos_t> relativePosRect, optional <weak_ptr<AbstractItem>> item, EqSlotUIElement* parent,
		double relativeEdgeThickness, shared_ptr <InventoryInputHandler> inventoryInputHandler);
	void resetItemAlpha();
	void handleMouseInput(MouseEventTypes mouseEventType, pair<int, int> pos, Time timeDiff) override;
	void render(shared_ptr<Texture>& textureToDrawOn) override;
	
};

