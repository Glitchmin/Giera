#include "BoardRenderer.h"

void BoardRenderer::drawBoard()
{
	for (auto& it : drawablesSet) {
		it.second.lock()->draw(boardTexture, pixelsPerMeter);
	}
}
