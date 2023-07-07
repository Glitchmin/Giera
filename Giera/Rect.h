#pragma once
#include <array>
#include <optional>
using std::array;
using std::optional;

template <class T> class Rect {
protected:
	array <T, 4> rectangle;
public:

public:
    array<T,4> getRectangle() const;
    void setRectangle(array<T,4> rectangle);
    
    array<T, 2> getPos() const;
    void setPos(optional<T> x, optional<T> y);
    
    array<T, 2> getSize() const;
    void setSize(optional<T> x, optional<T> y);
};

template<class T> 
inline array<T, 4> Rect<T>::getRectangle() const
{
    return rectangle;
}

template<class T> 
inline void Rect<T>::setRectangle(array<T, 4> rectangle)
{
    this->rectangle = rectangle;
}

template<class T>
inline array<T, 2> Rect<T>::getPos() const
{
    return array<T, 2>(rectangle[0], rectangle[1]);
}

template<class T>
inline void Rect<T>::setPos(optional<T> x, optional<T> y)
{
    if (x) {
        rectangle[0] = *x;
    }
    if (y) {
        rectangle[1] = *y;
    }
}

template<class T> 
inline array<T, 2> Rect<T>::getSize() const
{
    return array<T, 2>(rectangle[2], rectangle[3]);
}

template<class T>
inline void Rect<T>::setSize(optional<T> x, optional<T> y)
{
    if (x) {
        rectangle[2] = *x;
    }
    if (y) {
        rectangle[3] = *y;
    }
}
