#include "Display.h"

Display::Display(Object* parent, string name) : Object(parent, name) {}

void Display::handlePrint(vector<string> args) {
    if(args.size() == 0) return;
    cout << endl << args[0];
}
