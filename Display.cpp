#include "Display.h"

Display::Display(Object* parent, string name) : Object(parent, name) {}

void Display::handlePrint(vector<string> args) {
    cout << args[0];
}
