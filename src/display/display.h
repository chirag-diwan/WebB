#pragma once
#include <raylib.h>

class Display {
public:
    Display(int width, int height, const char* title) ;

    ~Display() ;

    void DrawCommand();
};
