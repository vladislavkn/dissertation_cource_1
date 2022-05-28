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

void Safe::handleSetSafeDimentions(vector<string> args) {
    if(args.size() != 2) return;
    n = stoi(args[0]);
    m = stoi(args[1]);
};

void Safe::handleSelectBox(vector<string> args) {
    if(args.size() != 1) return;
    open_box_number = stoi(args[0]);
    safe_state = WaitingClientKey;
    cout << "Selected box: " <<open_box_number << endl;
}

void Safe::handleOpenBox(vector<string> args) {
    vector<string> openBoxArgs = {to_string(open_box_number), to_string(SafeBox::SafeBoxState::Open)};
    emit_signal(TOSIGNAL(Safe::signalSetBoxState), openBoxArgs);
}
void Safe::handleCloseBox(vector<string> args) {
    if(args.size() != 1) return;
    vector<string> closeBoxArgs = {args[0], to_string(SafeBox::SafeBoxState::Closed)};
    emit_signal(TOSIGNAL(Safe::signalSetBoxState), closeBoxArgs);
}

void Safe::handleAddBox(vector<string> args) {
    if(args.size() != 1) return;
    SafeBox* safeBox = new SafeBox(this, stoi(args[0]), "Box" + args[0]);
    set_connection(TOSIGNAL(Safe::signalSetBoxState), safeBox, TOHANDLER(SafeBox::handleSetBoxState));
    safeBox->set_readiness(1);
}

void Safe::handleSetSafeState(vector<string> args) {
    if(args.size() != 1) return;
    safe_state = (SafeState) stoi(args[0]);
}

void Safe::handleResetSafe(vector<string>) {
    open_box_number = 0;
    safe_state = Ready;
}

void Safe::signalSetBoxState(vector<string>&) {}
