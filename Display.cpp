#include "Display.h"

Display::Display(Object* parent, string name) : Object(parent, name) {}

void Display::handlePrint(Triple args) {
    cout << args.first;
}
