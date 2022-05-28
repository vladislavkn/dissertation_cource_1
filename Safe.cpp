#include "Safe.h"
    
Safe::Safe(Object* parent, string name) : Object(parent, name) {
    class_number = 2;
}
    
SafeBox* Safe::get_box_by_number(int number) {
    for(int i = 0; i < children.size(); i++) {
        int boxNumber = ((SafeBox*) children[i])->get_box_number();
        if(boxNumber == number) return (SafeBox*) children[i];
    }
    
    return nullptr;
}

int Safe::get_open_box_number() {
    return open_box_number;
}

int Safe::getAllBoxesCount() {
    return n * m;
}

void Safe::handleSetSafeDimentions(Triple args) {
    n = stoi(args.first);
    m = stoi(args.second);
};

void Safe::handleSelectBox(Triple args) {
    open_box_number = stoi(args.first);
    safe_state = WaitingClientKey;
}

void Safe::handleOpenBox(Triple args) {
    emit_signal(TOSIGNAL(Safe::signalSetBoxState), Triple(to_string(open_box_number), to_string(SafeBox::SafeBoxState::Open)));
}
void Safe::handleCloseBox(Triple args) {
    emit_signal(TOSIGNAL(Safe::signalSetBoxState), Triple(args.first, to_string(SafeBox::SafeBoxState::Closed)));
}

void Safe::handleAddBox(Triple args) {
    SafeBox* safeBox = new SafeBox(this, stoi(args.first), "Box" + args.first);
    set_connection(TOSIGNAL(Safe::signalSetBoxState), safeBox, TOHANDLER(SafeBox::handleSetBoxState));
    safeBox->set_readiness(1);
}

void Safe::handleSetSafeState(Triple args) {
    safe_state = (SafeState) stoi(args.first);
}

void Safe::handleResetSafe(Triple) {
    open_box_number = 0;
    safe_state = Ready;
}

void Safe::signalSetBoxState(Triple&) {}
