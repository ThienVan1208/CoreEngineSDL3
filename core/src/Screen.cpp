#include "../include/Screen.h"

Screen::Screen(float w, float h){
    SCREEN_WIDTH = w;
    SCREEN_HEIGHT = h;
}

float Screen::GetWidth(){
    return SCREEN_WIDTH;
}

float Screen::GetHeight(){
    return SCREEN_HEIGHT;
}