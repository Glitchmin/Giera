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
using std::array;
using std::vector;
using std::unique_ptr;

typedef int px_pos_t; //pixel position
typedef float fr_pos_t; //proportional position using fraction, relative to the parent
class UIElement
{
protected:
	Rect <px_pos_t> pxRealPosRect; //position on screen in pixels
	UIElement* parent;
	shared_ptr <Texture> image;
	shared_ptr <Texture> texture;
	vector<unique_ptr<UIElement>> children;
	bool updateNeeded = 1;
	SDL_Color bgColor;
	ImageResizeTypes imageResizePolicy;
	VerticalAlignmentTypes vImageAlign;
	HorizontalAlignmentTypes hImageAlign;

public:
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

	UIElement(Rect <fr_pos_t> frRelPosRect, shared_ptr<Texture> image, UIElement* parent, SDL_Color bgColor = { 0,0,0,0 }, 
		ImageResizeTypes imageResizePolicy = ImageResizeTypes::STRETCH, VerticalAlignmentTypes vImageAlign = VerticalAlignmentTypes::TOP,
		HorizontalAlignmentTypes hImageAlign = HorizontalAlignmentTypes::LEFT);
	UIElement(Rect <px_pos_t> pxRealPosRect, shared_ptr<Texture> image, SDL_Color bgColor = { 0,0,0,0 });
	virtual void addChild(unique_ptr<UIElement> child);
	virtual void removeChild(UIElement* childToRemove);
	virtual void insertBackground();
	virtual void render(shared_ptr <Texture>& textureToDrawOn);
	virtual bool handleMouseInput(MouseEventTypes mouseEventType, pair<int,int> pos, Time timeDiff);
	void drawImage();

    UIElement* getParent() const;
	virtual const vector<unique_ptr<UIElement>>& getChildren();
	virtual void clearChildren();

	virtual void needsUpdate();
    shared_ptr<Texture> getTexture() const;

	Rect<px_pos_t> getPixelRelativePosRect() const;

	Rect<px_pos_t> getPixelRealPosRect() const;
	void setPixelRealPosRect(Rect<px_pos_t> pixelRealPosRect);

    void setBgColor(SDL_Color bgColor);

};

