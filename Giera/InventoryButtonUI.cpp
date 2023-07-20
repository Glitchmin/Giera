#include "InventoryButtonUI.h"

using std::min;
using std::max;

InventoryButtonUI::InventoryButtonUI(Rect<fr_pos_t> relativePosRect, weak_ptr<AbstractItem> item, UIElement* parent, double relativeEdgeThickness, shared_ptr<InventoryInputHandler> inventoryInputHandler)
: ButtonUI(relativePosRect,TextureLoader::getTexturePtr(item.lock()->getFilePath()),parent,relativeEdgeThickness), inventoryInputHandler (inventoryInputHandler)
{
}

void InventoryButtonUI::handleMouseInput(MouseEventTypes mouseEventType, pair<int, int> pos, Time timeDiff)
{
	if (pxRealPosRect.isPointInside(pos.first, pos.second)) {
		edgeTransparency += timeDiff.getTimeMs();
		edgeTransparency = min(edgeTransparency, maxEdgeTransparency);
		if (mouseEventType == MouseEventTypes::PRESS_LEFT) {
			inventoryInputHandler->setSelectedItem(item.lock());
		}
	}
	else {
		edgeTransparency -= timeDiff.getTimeMs();
		edgeTransparency = max(edgeTransparency, 0);
	}
}
