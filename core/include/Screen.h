#ifndef SCREEN_H
#define SCREEN_H

class Screen{
private:
    static float SCREEN_WIDTH;
    static float SCREEN_HEIGHT;
public:
    Screen(float w, float h);
    static float GetWidth();
    static float GetHeight();
};

#endif