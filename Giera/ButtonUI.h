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
    ButtonUI(Rect <rel_pos_t> relativePosRect, shared_ptr<Texture> texture, UIElement* parent, double relativeEdgeThickness);
    void fillEdgesTexture();
    virtual bool handleMouseInput(MouseButtonTypes mouseButtonType, pair<int, int> pos) override;
    virtual void render(shared_ptr <Texture>& textureToDrawOn) override;
};

