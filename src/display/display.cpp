#include "display.h"
#include <raylib.h>

Display::Display(int width, int height, const char* title) {
  InitWindow(width, height, title);
  SetTargetFPS(60);
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
}

Display::~Display() {
  CloseWindow();
}

void Display::Clear(Color color) {
}

void Display::BeginDraw() {
}

void Display::EndDraw() {
}

bool Display::ShouldClose() const {
}

