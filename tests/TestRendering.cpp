#include "TestFramework.h"
#include "Texture.h"
#include "TextureLoader.h"
#include "FontTypes.h"

TEST_CASE("Rendering", "ui font is shipped and renders text")
{
    // The font used to be git-ignored, so fresh clones rendered no text at all.
    auto text = TextureLoader::makeTextTexture((FontTypes)0, 24, "12/40", { 255, 255, 255, 255 });
    REQUIRE(text != nullptr);
    CHECK(text->getSize().first > 10);
    CHECK(text->getSize().second > 10);
}

TEST_CASE("Rendering", "textures are cached")
{
    auto a = TextureLoader::getTexturePtr("assets/tx/npc/npc0.png");
    auto b = TextureLoader::getTexturePtr("assets/tx/npc/npc0.png");
    REQUIRE(a != nullptr);
    CHECK(a == b);
    CHECK(a->getSize().first > 1);
}

TEST_CASE("Rendering", "texture copies are independent")
{
    auto a = TextureLoader::getTextureCopy("assets/tx/npc/npc0.png");
    auto b = TextureLoader::getTextureCopy("assets/tx/npc/npc0.png");
    REQUIRE(a != nullptr);
    REQUIRE(b != nullptr);
    CHECK(a != b);
    CHECK(a->getSize() == b->getSize());
}

TEST_CASE("Rendering", "missing texture gives a placeholder instead of crashing (regression)")
{
    auto tx = TextureLoader::getTexturePtr("assets/tx/does/not/exist.png");
    REQUIRE(tx != nullptr);
    CHECK(tx->getSDLTexture() != nullptr);
    CHECK_EQ(tx->getSize().first, 16);
}

TEST_CASE("Rendering", "every character texture loads")
{
    for (const char* path : { "assets/tx/npc/npc0.png", "assets/tx/npc/npc1.png",
                              "assets/tx/shadows/shadow_medium.png",
                              "assets/tx/items/shield/shield0.png", "assets/tx/items/arrow/arrow0.png",
                              "assets/tx/items/armor/armor0.png", "assets/tx/items/melee/melee0.png" }) {
        auto tx = TextureLoader::getTexturePtr(path);
        REQUIRE(tx != nullptr);
        // placeholders are 16x16; real art is larger
        CHECK(tx->getSize().first > 16 || tx->getSize().second > 16);
    }
}
