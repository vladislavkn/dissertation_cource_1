#include "Control.h"
#include "Server.h"
#include "Safe.h"
#include "SafeBox.h"

Control::Control(Object* parent, string name) : Object(parent, name) {}

bool Control::isAllBoxesConfigured() {
    Safe* safe = (Safe*) find_by_path("/control/safe");
    Server* server = (Server*) find_by_path("/control/server");
    int allBoxesCount = safe->getAllBoxesCount();
    unsigned long configuredBoxesCount = server->getConfiguredBoxesCount();
    
    return configuredBoxesCount == allBoxesCount;
}

void Control::handleSetSafeDimentions(vector<string> args) {
    if(args.size() != 2) return;
    emit_signal(TOSIGNAL(Control::signalSetSafeDimentions), args);
}

void Control::handleSetBoxKeys(vector<string> args) {
    if(args.size() != 3) return;
    vector<string> addBoxArgs = { args[0] };
    emit_signal(TOSIGNAL(Control::signalAddBox), addBoxArgs);
    emit_signal(TOSIGNAL(Control::signalSetBoxKeys), args);
}

void Control::handleSelectBox(vector<string> args) {
    if(args.size() != 1) return;
    Safe* safe = (Safe*) find_by_path("/control/safe");
    if(safe->get_box_by_number(stoi(args[0]))->get_box_state() == SafeBox::SafeBoxState::Closed) {
        emit_signal(TOSIGNAL(Control::signalSelectBox), args);
    } else {
        vector<string> errorArgs = {"The safe deposit box " + args[0] + " is open", "0", "3"};
        emit_signal(TOSIGNAL(Control::signalError), errorArgs);
    }
}

void Control::handleApplyClientKey(vector<string> args) {
    if(args.size() != 1) return;
    string clientKey = args[0];
    Server* server = (Server*) find_by_path("/control/server");
    Safe* safe = (Safe*) find_by_path("/control/safe");
    int openBoxNumber = safe->get_open_box_number();
    if(server->validateBoxClientkey(openBoxNumber, clientKey)) {
        vector<string> setSafeStateArgs = {to_string(Safe::SafeState::WaitingBankKey)};
        emit_signal(TOSIGNAL(Control::signaleSetSafeState), setSafeStateArgs);
    } else {
        vector<string> errorArgs = {"The client is key is incorrect", "0", "4"};
        emit_signal(TOSIGNAL(Control::signalError), errorArgs);
    }
}

void Control::handleApplyBankKey(vector<string> args) {
    if(args.size() != 1) return;
    string bankKey = args[0];
    Server* server = (Server*) find_by_path("/control/server");
    Safe* safe = (Safe*) find_by_path("/control/safe");
    int openBoxNumber = safe->get_open_box_number();
    if(server->validateBoxBankkey(openBoxNumber, bankKey)) {
        vector<string> openBoxArgs;
        emit_signal(TOSIGNAL(Control::signalOpenBox), openBoxArgs);
    } else {
        vector<string> errorArgs = {"The bank is key is incorrect", "0", "5"};
        emit_signal(TOSIGNAL(Control::signalError), errorArgs);
    }
}

void Control::handleCloseBox(vector<string> args) {
    if(args.size() != 1) return;
    emit_signal(TOSIGNAL(Control::signalCloseBox), args);
}

void Control::handleResetSafe(vector<string> args) {
    emit_signal(TOSIGNAL(Control::signalResetSafe), args);
}

void Control::signalSetSafeDimentions(vector<string> &) {}

void Control::signaleSetSafeState(vector<string>&) {}

void Control::signalResetSafe(vector<string>&) {}

void Control::signalSelectBox(vector<string>&) {};

void Control::signalOpenBox(vector<string>&) {};

void Control::signalCloseBox(vector<string>&) {};

void Control::signalSetBoxKeys(vector<string>&) {};

void Control::signalError(vector<string>&) {};

void Control::signalAddBox(vector<string> &) {};
