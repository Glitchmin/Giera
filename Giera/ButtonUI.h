#pragma once
#include "UIElement.h"
class ButtonUI :
    public UIElement
{
protected:
    double relativeEdgeThickness;
    int edgeTransparency;
    static inline int maxEdgeTransparency = 128;
public:
    ButtonUI(Rect <fr_pos_t> relativePosRect, shared_ptr<Texture> image, UIElement* parent, double relativeEdgeThickness, 
        ImageResizeTypes imageResizePolicy = ImageResizeTypes::STRETCH, VerticalAlignmentTypes vImageAlign = VerticalAlignmentTypes::TOP,
        HorizontalAlignmentTypes hImageAlign = HorizontalAlignmentTypes::LEFT);
    void drawEdges(shared_ptr<Texture>& texture);
    void changeEdgeTransparency(Time timeDiff, bool positive);
    virtual void handleMouseInput(MouseEventTypes mouseEventType, pair<int, int> pos, Time timeDiff) override;
    virtual void render(shared_ptr <Texture>& textureToDrawOn) override;
};

