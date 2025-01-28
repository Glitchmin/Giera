#include "UIElement.h"
#include "TextureLoader.h"
#include <SDL.h>

/**
 * Constructs a UIElement using fractional position relative to the parent.
 *
 * This constructor calculates the actual pixel position and size by multiplying the fractional
 * coordinates by the parent's pixel-based dimensions. Then it initializes an internal texture
 * for rendering this UIElement.
 */
UIElement::UIElement(Rect<fr_pos_t> frRelPosRect,
    shared_ptr<Texture> image,
    UIElement* parent,
    SDL_Color bgColor,
    ImageResizeTypes imageResizePolicy,
    VerticalAlignmentTypes vImageAlign,
    HorizontalAlignmentTypes hImageAlign)
    : image(image), parent(parent),
    pxRealPosRect({ (int)(frRelPosRect.x * parent->pxRealPosRect.w + parent->pxRealPosRect.x),
                    (int)(frRelPosRect.y * parent->pxRealPosRect.h + parent->pxRealPosRect.y),
                    (int)(frRelPosRect.w * parent->pxRealPosRect.w),
                    (int)(frRelPosRect.h * parent->pxRealPosRect.h) }),
    bgColor(bgColor),
    imageResizePolicy(imageResizePolicy),
    vImageAlign(vImageAlign),
    hImageAlign(hImageAlign)
{
    texture = TextureLoader::makeUniColorTexture(pxRealPosRect.w, pxRealPosRect.h, { 0,0,0,0 });
}

/**
 * Constructs a UIElement using absolute pixel position.
 *
 This constructor has no parent field and is used only for WindowUI object currently.
 */
UIElement::UIElement(Rect<px_pos_t> pxRealPosRect,
    shared_ptr<Texture> image,
    SDL_Color bgColor)
    : parent(nullptr), image(image), pxRealPosRect(pxRealPosRect), bgColor(bgColor)
{
}

/**
 * Draws the contents inside this UI element.
 *
 * By default, this calls drawImage(), which renders any assigned image.
 * Derived classes can override this to add additional drawing logic.
 */
void UIElement::drawInside() {
    drawImage();
}

/**
 * Draws the image onto this UI element, respecting the current resizing policy.
 *
 * Depending on the ImageResizeTypes policy, the image may be stretched or resized
 * while maintaining its aspect ratio. The alignment parameters determine how the
 * image is positioned within the element's bounds.
 */
void UIElement::drawImage() {
    if (image == nullptr) {
        return;
    }
    if (imageResizePolicy == ImageResizeTypes::STRETCH) {
        image->draw(*texture, std::nullopt, std::nullopt);
        return;
    }
    int imSizeX = image->getSize().first;
    int imSizeY = image->getSize().second;
    if (imageResizePolicy == ImageResizeTypes::KEEP_ASPECT_RATIO) {
        double aspectRatio = ((double)imSizeX) / (double)imSizeY;
        double resizeRatio = std::min((double)pxRealPosRect.w / (double)imSizeX,
            (double)pxRealPosRect.h / (double)imSizeY);
        imSizeX = static_cast<int>(imSizeX * resizeRatio);
        imSizeY = static_cast<int>(imSizeY * resizeRatio);
    }
    int x = 0;
    int y = 0;

    if (hImageAlign == HorizontalAlignmentTypes::CENTER) {
        x += (pxRealPosRect.w - imSizeX) / 2;
    }
    if (hImageAlign == HorizontalAlignmentTypes::RIGHT) {
        x += pxRealPosRect.w - imSizeX;
    }

    if (vImageAlign == VerticalAlignmentTypes::CENTER) {
        y += (pxRealPosRect.h - imSizeY) / 2;
    }
    if (vImageAlign == VerticalAlignmentTypes::BOTTOM) {
        y += pxRealPosRect.h - imSizeY;
    }
    image->draw(*texture, std::nullopt, SDL_Rect{ x,y,imSizeX,imSizeY });
}

/**
 * Renders the UI element onto the provided texture.
 *
 * This function checks if the element needs updating; if so, it draws the background,
 * calls drawInside() for custom drawing, then renders all child elements into its
 * internal texture. Finally, it draws itself onto the provided texture.
 */
void UIElement::render(shared_ptr<Texture>& textureToDrawOn)
{
    if (updateNeeded) {
        updateNeeded = false;
        insertBackground();
        drawInside();
        for (auto& child : children) {
            child->render(texture);
        }
    }
    SDL_Rect dstRect = getPixelRelativePosRect().turnToSDL_Rect();
    texture->draw(*textureToDrawOn, std::nullopt, dstRect);
}

/**
 * Handles mouse input by first passing the event to all child elements,
 * allowing them to handle the event themselves.
 *
 * This default implementation does not perform any specific action
 * for this UI element itself.
 */
bool UIElement::handleMouseInput(MouseEventTypes mouseEventType, std::pair<int, int> pos, Time timeDiff)
{
    bool ans = false;
    for (auto& child : children) {
        ans = ans || child->handleMouseInput(mouseEventType, pos, timeDiff);
    }
    return ans;
}

/**
 * Handles various SDL events, including touch events.
 *
 * This function refreshes fingerPositions to reflect active touches
 * and then passes the event down to children for further handling.
 */
UIElement::EventHandleResult UIElement::handleEvent(const SDL_Event& e, Time timeDiff, int screenSizeX, int screenSizeY)
{
    // After handling the event, always refresh fingerPositions
    // with all active fingers across all touch devices:
    fingerPositions.clear();
    int numDevices = SDL_GetNumTouchDevices();
    for (int d = 0; d < numDevices; ++d)
    {
        SDL_TouchID touchId = SDL_GetTouchDevice(d);
        if (touchId == 0) continue;

        int nFingers = SDL_GetNumTouchFingers(touchId);
        for (int i = 0; i < nFingers; ++i)
        {
            SDL_Finger* finger = SDL_GetTouchFinger(touchId, i);
            if (finger)
            {
                float x = finger->x * screenSizeX;
                float y = finger->y * screenSizeY;
                fingerPositions.emplace_back(x, y);
            }
        }
    }

    // Pass the event down to children (if any)
    for (auto& child : children)
    {
        child->handleEvent(e, timeDiff, screenSizeX, screenSizeY);
    }

    EventHandleResult ret{ false, {} };
    return ret;
}

/**
 * Adds a new child UI element to this element and marks itself as needing update.
 *
 * Adding a child triggers a redraw to ensure the new element is properly reflected.
 */
void UIElement::addChild(unique_ptr<UIElement> child)
{
    children.push_back(std::move(child));
    needsUpdate();
}

/**
 * Removes the specified child UI element from this element and marks itself as needing update.
 *
 * The element is located by pointer comparison, and once found, it is removed from the children vector.
 */
void UIElement::removeChild(UIElement* childToRemove)
{
    for (int i = 0; i < static_cast<int>(children.size()); i++) {
        if (children[i].get() == childToRemove) {
            children.erase(children.begin() + i);
            break;
        }
    }
    needsUpdate();
}

/**
 * Fills the internal texture with the element's background color before drawing additional content.
 */
void UIElement::insertBackground()
{
    texture->fillWithColor(bgColor);
}

/**
 * Provides read-only access to the vector of child UI elements.
 *
 * This default implementation returns a reference to the internal vector.
 */
const vector<unique_ptr<UIElement>>& UIElement::getChildren()
{
    return children;
}

/**
 * Removes all child elements and marks the UI element as needing update.
 */
void UIElement::clearChildren()
{
    children.clear();
}

/**
 * Returns the parent of this element.
 *
 * If the element has no parent, this returns nullptr.
 */
UIElement* UIElement::getParent() const
{
    return parent;
}

/**
 * Signals that this UI element needs to be updated/redrawn and propagates the signal to its parent.
 *
 * This ensures changes propagate to the root element for a full redraw if necessary.
 */
void UIElement::needsUpdate()
{
    if (parent == nullptr) {
        return;
    }
    updateNeeded = true;
    parent->needsUpdate();
}

/**
 * Retrieves the internal rendering texture used by this element.
 *
 * This can be used to perform additional drawing or to inspect the drawn content.
 */
shared_ptr<Texture> UIElement::getTexture() const
{
    return texture;
}

/**
 * Calculates a rectangle in pixels relative to the parent element.
 *
 * This is useful for properly positioning the element within its parent's coordinate space.
 */
Rect<px_pos_t> UIElement::getPixelRelativePosRect() const
{
    return {
        pxRealPosRect.x - parent->pxRealPosRect.x,
        pxRealPosRect.y - parent->pxRealPosRect.y,
        pxRealPosRect.w,
        pxRealPosRect.h
    };
}

/**
 * Returns the absolute position and size of this element in pixels.
 */
Rect<px_pos_t> UIElement::getPixelRealPosRect() const
{
    return pxRealPosRect;
}

/**
 * Updates this UI element's absolute position/size and shifts child elements accordingly.
 *
 * This preserves the relative positions of child elements when the parent moves or resizes.
 */
void UIElement::setPixelRealPosRect(Rect<px_pos_t> pixelRealPosRect)
{
    px_pos_t xShift = pixelRealPosRect.x - pxRealPosRect.x;
    px_pos_t yShift = pixelRealPosRect.y - pxRealPosRect.y;
    for (auto& child : children) {
        auto posRect = child->getPixelRealPosRect();
        posRect.x += xShift;
        posRect.y += yShift;
        child->setPixelRealPosRect(posRect);
    }
    pxRealPosRect = pixelRealPosRect;
}

/**
 * Changes the background color of this UI element and will trigger an update on the next render pass.
 */
void UIElement::setBgColor(SDL_Color bgColor)
{
    this->bgColor = bgColor;
}

/**
 * Outputs a textual representation of the mouse event type to the provided stream.
 *
 * This is mainly used for logging and debugging.
 */
std::ostream& operator<<(std::ostream& os, UIElement::MouseEventTypes eventType) {
    switch (eventType) {
    case UIElement::MouseEventTypes::HOVER:
        os << "HOVER";
        break;
    case UIElement::MouseEventTypes::PRESS_LEFT:
        os << "PRESS_LEFT";
        break;
    case UIElement::MouseEventTypes::RELEASE_LEFT:
        os << "RELEASE_LEFT";
        break;
    case UIElement::MouseEventTypes::PRESS_RIGHT:
        os << "PRESS_RIGHT";
        break;
    case UIElement::MouseEventTypes::RELEASE_RIGHT:
        os << "RELEASE_RIGHT";
        break;
    case UIElement::MouseEventTypes::MOUSE_SCROLL_UP:
        os << "MOUSE_SCROLL_UP";
        break;
    case UIElement::MouseEventTypes::MOUSE_SCROLL_DOWN:
        os << "MOUSE_SCROLL_DOWN";
        break;
    case UIElement::MouseEventTypes::COUNT:
        os << "COUNT";
        break;
    default:
        os << "UNKNOWN_EVENT";
    }
    return os;
}