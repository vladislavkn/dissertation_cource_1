#include "Safe.h"
    
Safe::Safe(Object* parent, string name) : Object(parent, name) {}
    
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

void Safe::handleSetSafeDimentions(vector<string> args) {
    n = stoi(args[0]);
    m = stoi(args[1]);
};

void Safe::handleSelectBox(vector<string> args) {
    open_box_number = stoi(args[0]);
    safe_state = WaitingClientKey;
}

void Safe::handleOpenBox(vector<string> args) {
    emit_signal(TOSIGNAL(Safe::signalSetBoxState), {to_string(open_box_number), to_string(SafeBox::SafeBoxState::Open)});
}
void Safe::handleCloseBox(vector<string> args) {
        emit_signal(TOSIGNAL(Safe::signalSetBoxState), {args[0], to_string(SafeBox::SafeBoxState::Closed)});
}

void Safe::handleAddBox(vector<string> args) {
    SafeBox* safeBox = new SafeBox(this, stoi(args[0]), "Box" + args[0]);
    set_connection(TOSIGNAL(Safe::signalSetBoxState), safeBox, TOHANDLER(SafeBox::handleSetBoxState));
    safeBox->set_readiness(1);
}

void Safe::handleSetSafeState(vector<string> args) {
    safe_state = (SafeState) stoi(args[0]);
}

void Safe::handleResetSafe(vector<string>) {
    open_box_number = 0;
    safe_state = Ready;
}

void Safe::signalSetBoxState(vector<string>&) {}
