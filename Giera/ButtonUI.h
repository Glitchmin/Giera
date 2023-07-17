#pragma once
#include "UIElement.h"
class ButtonUI :
    public UIElement
{
protected:
    shared_ptr <Texture> edgesTexture;
    double relativeEdgeThickness;
    int edgeTransparency;
    static inline int maxEdgeTransparency = 128;
public:
    ButtonUI(Rect <fr_pos_t> relativePosRect, shared_ptr<Texture> texture, UIElement* parent,
        double relativeEdgeThickness);
    void drawEdges(shared_ptr<Texture>& texture);
    virtual void handleMouseInput(MouseEventTypes mouseEventType, pair<int, int> pos, Time timeDiff);
    virtual void render(shared_ptr <Texture>& textureToDrawOn) override;
};

