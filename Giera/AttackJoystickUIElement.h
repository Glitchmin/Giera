#pragma once
#include "UIElement.h"
#include "Player.h"

class AttackJoystickUIElement: public UIElement
{
public:
    AttackJoystickUIElement(
        Rect<fr_pos_t> frRelPosRect,
        UIElement* parent, shared_ptr<Player> player) 
        :UIElement(frRelPosRect, nullptr,parent,bgColor={255,255,255,12}),
        player(player) {};

    virtual bool handleMouseInput(MouseEventTypes mouseEventType, std::pair<int, int> pos, Time timeDiff) override;
    virtual UIElement::EventHandleResult handleEvent(const SDL_Event& e, Time timeDiff, int screenSizeX, int screenSizeY) override;
    virtual void drawInside() override;

protected:
    shared_ptr <Texture> image = TextureLoader::makeUniColorTexture(10, 10, {255,255,255,255});
    shared_ptr <Texture> knobImage = TextureLoader::makeUniColorTexture(10, 10, { 0,0,0,255 });
    float xDeflection = 0;
    float yDeflection = 0;
    bool pressed = false;
    shared_ptr<Player> player;
};
