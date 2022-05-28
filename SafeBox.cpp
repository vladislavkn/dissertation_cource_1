#include "SafeBox.h"
    
SafeBox::SafeBox(Object* parent, int box_number, string name) : Object(parent, name) {
    this->box_number = box_number;
}

SafeBox::SafeBoxState SafeBox::get_box_state() {
    return state;
}

int SafeBox::get_box_number() {
    return box_number;
}

void SafeBox::handleSetBoxState(Triple args) {
    int box_number = stoi(args.first);
    if(this->box_number == box_number) {
        state = (SafeBoxState) stoi(args.second);
    }
}

