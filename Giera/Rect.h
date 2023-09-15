#pragma once
#include <array>
#include <optional>
using std::array;
using std::optional;

template <class T> class Rect {
public:
	T x,y,w,h;

	Rect();
	Rect(T x, T y, T sizeX, T sizeY);

	bool isPointInside(T x, T y);

	SDL_Rect turnToSDL_Rect();
};

template<class T>
inline Rect<T>::Rect(T x, T y, T sizeX, T sizeY):
x(x),y(y),w(sizeX),h(sizeY) {}

template<class T>
inline bool Rect<T>::isPointInside(T pX, T pY)
{
	return (pX >= x && pX <= x+w && pY >= y && pY <= y+h);
}

template<class T>
inline Rect<T>::Rect()
{
	throw "default constructor";
}

template<class T>
inline SDL_Rect Rect<T>::turnToSDL_Rect()
{
	return SDL_Rect{ x,y,w,h };
}
