#pragma once

using std::string;
using std::optional;
using std::nullopt;
using std::pair;
using std::unique_ptr;


class Texture
{
public:
	Texture();
	Texture(SDL_Texture* texture);
	~Texture();
	Texture(Texture& texture) = delete;
	Texture(Texture&& texture) = delete;
    SDL_Texture* getSDLTexture() const;
    void setTexture(SDL_Texture* texture);
    static SDL_Renderer* getRenderer();
	static void setRenderer(SDL_Renderer* renderer);
	void draw(Texture& target, optional<SDL_Rect> srcRect, optional<SDL_Rect> dstRect, float angle, unique_ptr<SDL_Point>& rotationCenter);
	void draw(Texture& target, optional<SDL_Rect> srcRect, optional<SDL_Rect> dstRect);
	//srcRect - the source SDL_Rect structure or NULL for the entire texture
	//dstRect - the destination SDL_Rect structure or NULL for the entire rendering target
	void fillWithColor(SDL_Color color);

	pair<int, int> getSize();
private:
	static inline SDL_Renderer* renderer = NULL;
	SDL_Texture* texture;

};

