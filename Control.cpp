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

void Control::handleSetSafeDimentions(Triple args) {
    emit_signal(TOSIGNAL(Control::signalSetSafeDimentions), args);
}

void Control::handleSetBoxKeys(Triple args) {
    emit_signal(TOSIGNAL(Control::signalAddBox), Triple(args.first));
    emit_signal(TOSIGNAL(Control::signalSetBoxKeys), args);
}

void Control::handleSelectBox(Triple args) {
    Safe* safe = (Safe*) find_by_path("/control/safe");
    if(safe->get_box_by_number(stoi(args.first))->get_box_state() == SafeBox::SafeBoxState::Closed) {
        emit_signal(TOSIGNAL(Control::signalSelectBox), args);
    } else {
        emit_signal(TOSIGNAL(Control::signalError), Triple("The safe deposit box " + args.first + " is open", "0", "3"));
    }
}

void Control::handleApplyClientKey(Triple args) {
    string clientKey = args.first;
    Server* server = (Server*) find_by_path("/control/server");
    Safe* safe = (Safe*) find_by_path("/control/safe");
    int openBoxNumber = safe->get_open_box_number();
    if(server->validateBoxClientkey(openBoxNumber, clientKey)) {
        emit_signal(TOSIGNAL(Control::signaleSetSafeState), Triple(to_string(Safe::SafeState::WaitingBankKey)));
    } else {
        emit_signal(TOSIGNAL(Control::signalError), Triple("The client is key is incorrect", "0", "4"));
    }
}

void Control::handleApplyBankKey(Triple args) {
    string bankKey = args.first;
    Server* server = (Server*) find_by_path("/control/server");
    Safe* safe = (Safe*) find_by_path("/control/safe");
    int openBoxNumber = safe->get_open_box_number();
    if(server->validateBoxBankkey(openBoxNumber, bankKey)) {
        emit_signal(TOSIGNAL(Control::signalOpenBox), Triple());
    } else {
        emit_signal(TOSIGNAL(Control::signalError), Triple("The bank is key is incorrect", "0", "5"));
    }
}

void Control::handleCloseBox(Triple args) {
    emit_signal(TOSIGNAL(Control::signalCloseBox), args);
}

void Control::handleResetSafe(Triple args) {
    emit_signal(TOSIGNAL(Control::signalResetSafe), args);
}

void Control::signalSetSafeDimentions(Triple &) {}

void Control::signaleSetSafeState(Triple&) {}

void Control::signalResetSafe(Triple&) {}

void Control::signalSelectBox(Triple&) {};

void Control::signalOpenBox(Triple&) {};

void Control::signalCloseBox(Triple&) {};

void Control::signalSetBoxKeys(Triple&) {};

void Control::signalError(Triple&) {};

void Control::signalAddBox(Triple&) {};
