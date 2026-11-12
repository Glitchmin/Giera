#pragma once
#include "Texture.h"
#include "TextureLoader.h"
#include "Rect.h"
#include "MouseButtonTypes.h"
#include "HorizontalAlignmentTypes.h"
#include "VerticalAlignmentTypes.h"
#include "ImageResizeTypes.h"
#include <array>
#include <vector>
#include <SDL.h>
using std::array;
using std::vector;
using std::unique_ptr;

/**
 * @brief Pixel position type.
 */
typedef int px_pos_t;

/**
 * @brief Fractional position type, relative to the parent.
 */
typedef float fr_pos_t;

/**
 * @brief A base class representing a UI element that can contain children and draw a single image.
 */
class UIElement
{
protected:
    Rect<px_pos_t> pxRealPosRect; ///< position on screen in pixels
    UIElement* parent; ///< pointer to the parent UI element
    shared_ptr<Texture> image; ///< optional image texture for this UI element
    shared_ptr<Texture> texture; ///< internal rendering texture for this UI element
    vector<unique_ptr<UIElement>> children; ///< collection of child UI elements
    bool updateNeeded = true; ///< flag indicating if this element needs to be redrawn
    SDL_Color bgColor; ///< background color (RGBA) of this UI element
    ImageResizeTypes imageResizePolicy; ///< policy on how to resize the image
    VerticalAlignmentTypes vImageAlign; ///< vertical image alignment
    HorizontalAlignmentTypes hImageAlign; ///< horizontal image alignment

    /**
     * @brief Stores finger positions for multi-touch events.
     */
    static inline vector<std::pair<float, float>> fingerPositions = {};

public:

    /**
     * @brief Types of mouse-related events that can be handled by this UI element.
     */
    enum class MouseEventTypes {
        HOVER,           
        PRESS_LEFT,      
        RELEASE_LEFT,    
        PRESS_RIGHT,     
        RELEASE_RIGHT,   
        MOUSE_SCROLL_UP, 
        MOUSE_SCROLL_DOWN, 
        COUNT
    };

    /**
     * @brief Structure holding the result of handling an event.
     */
    struct EventHandleResult
    {
        bool mouseHandled = false; ///< Indicates if mouse event was handled
        vector<bool> touchesHandled; ///< Indicates which touches were handled
    };

    /**
     * @brief Constructs a UIElement using fractional position relative to the parent.
     * @param frRelPosRect Initial position of the top-left corner of the UI element in fractional coordinates.
     * @param image Shared pointer to a texture for the UI element's image.
     * @param parent Pointer to the parent UI element.
     * @param bgColor Background color of the UI element.
     * @param imageResizePolicy Policy for resizing the image.
     * @param vImageAlign Vertical alignment for the image.
     * @param hImageAlign Horizontal alignment for the image.
     */
    UIElement(Rect<fr_pos_t> frRelPosRect,
        shared_ptr<Texture> image,
        UIElement* parent,
        SDL_Color bgColor = { 0,0,0,0 },
        ImageResizeTypes imageResizePolicy = ImageResizeTypes::STRETCH,
        VerticalAlignmentTypes vImageAlign = VerticalAlignmentTypes::TOP,
        HorizontalAlignmentTypes hImageAlign = HorizontalAlignmentTypes::LEFT);

    /**
     * @brief Constructs a UIElement using absolute pixel position. 
     This constructor has no parent field and is used only for WindowUI object
     * @param pxRealPosRect The absolute rectangle in pixels.
     * @param image Shared pointer to a texture for the UI element's image.
     * @param bgColor Background color of the UI element.
     */
    UIElement(Rect<px_pos_t> pxRealPosRect,
        shared_ptr<Texture> image,
        SDL_Color bgColor = { 0,0,0,0 });

    /**
     * @brief Adds a child UI element to this element.
     * @param child Unique pointer to the child UI element.
     */
    virtual void addChild(unique_ptr<UIElement> child);

    /**
     * @brief Removes a child UI element from this element.
     * @param childToRemove Pointer to the child UI element to remove.
     */
    virtual void removeChild(UIElement* childToRemove);

    /**
     * @brief Gets the parent UI element.
     * @return Pointer to the parent UI element.
     */
    UIElement* getParent() const;

    /**
     * @brief Gets the collection of child UI elements.
     * @return Constant reference to the vector of child UI elements.
     */
    virtual const vector<unique_ptr<UIElement>>& getChildren();

    /**
     * @brief Clears all child UI elements from this element.
     */
    virtual void clearChildren();

    /**
     * @brief Renders this UI element onto the given texture. 
     Should be final and its behaviour should be modified using internal methods, like drawInside()
     * @param textureToDrawOn The texture onto which the element will be drawn.
     * @return void
     */
    virtual void render(shared_ptr<Texture>& textureToDrawOn); //TODO make final, all changes should now be done with drawInside

    /**
     * @brief Inserts the background before drawing.
     */
    virtual void insertBackground();

    /**
     * @brief Draws the image onto this UI element.
     */
    void drawImage();

    /**
     * @brief Draws the contents inside this UI element (excluding children).
     */
    virtual void drawInside();

    /**
     * @brief Handles mouse input events.
     * @param mouseEventType The type of mouse event.
     * @param pos The mouse position.
     * @param timeDiff Time passed since last event handling.
     * @return True if the event was handled, false otherwise.
     */
    virtual bool handleMouseInput(MouseEventTypes mouseEventType, std::pair<int, int> pos, Time timeDiff);

    /**
     * @brief Handles various SDL events, including touch events.
     * @param e The SDL event object.
     * @param timeDiff Time passed since last event handling.
     * @param screenSizeX Width of the screen for coordinate conversion.
     * @param screenSizeY Height of the screen for coordinate conversion.
     * @return A structure with information about how the event was handled.
     */
    virtual EventHandleResult handleEvent(const SDL_Event& e, Time timeDiff, int screenSizeX, int screenSizeY);

    /**
     * @brief Marks this element (and its parent) as needing to be updated/redrawn.
     */
    virtual void needsUpdate();

    /**
     * @brief Gets the underlying texture used for rendering this UI element.
     * @return Shared pointer to the texture used internally by this UI element.
     */
    shared_ptr<Texture> getTexture() const;

    /**
     * @brief Gets the rectangle of this element relative to its parent in pixels.
     * @return The rectangle representing this element's relative position in pixels.
     */
    Rect<px_pos_t> getPixelRelativePosRect() const;

    /**
     * @brief Gets the absolute rectangle of this element in pixels.
     * @return The absolute rectangle in pixels.
     */
    Rect<px_pos_t> getPixelRealPosRect() const;

    /**
     * @brief Sets the absolute position/size of this element in pixels.
     * @param pixelRealPosRect The new rectangle in pixels.
     */
    void setPixelRealPosRect(Rect<px_pos_t> pixelRealPosRect);

    /**
     * @brief Sets the background color of this UI element.
     * @param bgColor SDL_Color representing the new background color.
     */
    void setBgColor(SDL_Color bgColor);
};

/**
 * @brief Writes the mouse event type to an output stream.
 * @param os Reference to the output stream.
 * @param eventType The mouse event type.
 * @return Reference to the output stream for chaining.
 */
std::ostream& operator<<(std::ostream& os, UIElement::MouseEventTypes eventType);


