#pragma once
#include "UIElement.h"

/**
 * @brief Represents a button UI element with customizable edge transparency and thickness.
 */
class ButtonUI : public UIElement
{
protected:
    double relativeEdgeThickness;

    int edgeTransparency;

    static inline int maxEdgeTransparency = 128;

public:
    /**
     * @param relativePosRect The position and size of the button relative to its parent.
     * @param image The texture to be displayed on the button.
     * @param parent The parent UI element to which this button belongs.
     * @param relativeEdgeThickness The relative thickness of the button's edges.
     * @param imageResizePolicy The policy for resizing the button's image (default: stretch).
     * @param vImageAlign The vertical alignment of the button's image (default: top).
     * @param hImageAlign The horizontal alignment of the button's image (default: left).
     */
    ButtonUI(Rect <fr_pos_t> relativePosRect, shared_ptr<Texture> image, UIElement* parent, double relativeEdgeThickness,
        ImageResizeTypes imageResizePolicy = ImageResizeTypes::STRETCH, VerticalAlignmentTypes vImageAlign = VerticalAlignmentTypes::TOP,
        HorizontalAlignmentTypes hImageAlign = HorizontalAlignmentTypes::LEFT);

    /**
     * @brief Draws the edges of the button.
     * @param texture The texture on which the edges will be drawn.
     */
    void drawEdges(shared_ptr<Texture>& texture);

    /**
     * @brief Adjusts the transparency of the button's edges.
     * @param timeDiff The time difference since the last update.
     * @param positive If true, increases transparency; otherwise, decreases it.
     */
    void changeEdgeTransparency(Time timeDiff, bool positive);

    /**
     * @brief Handles mouse input events for the button.
     * @param mouseEventType The type of mouse event (eg. PRESS_LEFT, RELEASE_RIGHT).
     * @param pos The mouse position (x, y) in screen coordinates.
     * @param timeDiff The time difference between the current and previous input events.
     * @return True if the mouse event is handled within the button's bounds, false otherwise.
     */
    virtual bool handleMouseInput(MouseEventTypes mouseEventType, pair<int, int> pos, Time timeDiff) override;

    /**
     * @brief Renders the button and its edges onto the specified texture.
     * @param textureToDrawOn The texture where the button will be rendered.
     */
    virtual void render(shared_ptr<Texture>& textureToDrawOn) override;
};
