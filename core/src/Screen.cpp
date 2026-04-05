#include "../include/Screen.h"
float Screen::SCREEN_WIDTH = 800.0f;
float Screen::SCREEN_HEIGHT = 600.0f;
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