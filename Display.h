#ifndef DISPLAY
#define DISPLAY

#include "Object.h"

class Display : public Object {
public:
    Display(Object* parent, string name="display");
    void handlePrint(vector<string>);
};

#endif
