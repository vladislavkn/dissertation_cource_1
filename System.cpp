#include "System.h"

System::System(Object* parent, string name) : Object(parent, name) {}

void System::build_objects_tree() {
    Control* control = new Control(this);
    Server* server = new Server(control);
    Safe* safe = new Safe(control);
    Reader* reader = new Reader(this);
    Display* display = new Display(this);
    
    set_connection(TOSIGNAL(System::signalPrint), display, TOHANDLER(Display::handlePrint));
    
    control->set_connection(TOSIGNAL(Control::signalError), this, TOHANDLER(System::handleError));

    set_connection(TOSIGNAL(System::signalInputLine), reader, TOHANDLER(Reader::handleInputLine));
    reader->set_connection(TOSIGNAL(Reader::signalNewLine), this, TOHANDLER(System::handleNewLine));
    
    set_connection(TOSIGNAL(System::signalSetSafeDimentions), control, TOHANDLER(Control::handleSetSafeDimentions));
    control->set_connection(TOSIGNAL(Control::signalSetSafeDimentions), safe, TOHANDLER(Safe::handleSetSafeDimentions));
    
    set_connection(TOSIGNAL(System::signalSetSafeBoxKeys), control, TOHANDLER(Control::handleSetBoxKeys));
    control->set_connection(TOSIGNAL(Control::signalSetBoxKeys), server, TOHANDLER(Server::handleSetBoxKeys));
    control->set_connection(TOSIGNAL(Control::signalAddBox), safe, TOHANDLER(Safe::handleAddBox));
    
    set_connection(TOSIGNAL(System::signalSelectBox), control, TOHANDLER(Control::handleSelectBox));
    control->set_connection(TOSIGNAL(Control::signalSelectBox), safe, TOHANDLER(Safe::handleSelectBox));
    
    set_connection(TOSIGNAL(System::signalApplyClientKey), control, TOHANDLER(Control::handleApplyClientKey));
    control->set_connection(TOSIGNAL(Control::signaleSetSafeState), safe, TOHANDLER(Safe::handleSetSafeState));
    
    set_connection(TOSIGNAL(System::signalApplyBankKey), control, TOHANDLER(Control::handleApplyBankKey));
    control->set_connection(TOSIGNAL(Control::signalOpenBox), safe, TOHANDLER(Safe::handleOpenBox));
    
    set_connection(TOSIGNAL(System::signalResetSafe), control, TOHANDLER(Control::handleResetSafe));
    control->set_connection(TOSIGNAL(Control::signalResetSafe), safe, TOHANDLER(Safe::handleResetSafe));
    
    set_connection(TOSIGNAL(System::signalCloseBox), control, TOHANDLER(Control::handleCloseBox));
    control->set_connection(TOSIGNAL(Control::signalCloseBox), safe, TOHANDLER(Safe::handleCloseBox));
}

int System::start() {
    print_subjects();
    set_readiness_for_all(1);
    vector<string> inputLineArgs;
    while(readiness != 0) {
        if(nextOperation == Input) {
            emit_signal(TOSIGNAL(System::signalInputLine), inputLineArgs);
        } else if(nextOperation == SetSafeDimentions) {
            emit_signal(TOSIGNAL(System::signalSetSafeDimentions), saved_args);
            state = InputBoxKeys;
            nextOperation = Input;
        } else if(nextOperation == SetBoxKeys) {
            emit_signal(TOSIGNAL(System::signalSetSafeBoxKeys), saved_args);
            Control* control = (Control*) find_by_path("/control");
            if(control->isAllBoxesConfigured()) {
                state = WaitingComleteKeyEntry;
            }
            nextOperation = Input;
        } else if(nextOperation == SelectBox) {
            vector<string> selectboxArgs = {saved_args[1]};
            emit_signal(TOSIGNAL(System::signalSelectBox), selectboxArgs);
            nextOperation = Input;
            state = WaitingClientKey;
        } else if(nextOperation == ApplyClientKey) {
            vector<string> applyClientKeyArgs = { saved_args[1] };
            emit_signal(TOSIGNAL(System::signalApplyClientKey), applyClientKeyArgs);
            nextOperation = Input;
            state = WaitingBankKey;
        } else if(nextOperation == ApplyBankKey) {
            vector<string> applyBankKeyArgs = { saved_args[1] };
            emit_signal(TOSIGNAL(System::signalApplyBankKey), applyBankKeyArgs);
            nextOperation = Input;
            state = ExecuteCommands;
        } else if(nextOperation == Reset) {
            vector<string> resetSafeArgs;
            emit_signal(TOSIGNAL(System::signalResetSafe), resetSafeArgs);
            nextOperation = Recover;
        } else if(nextOperation == CloseBox) {
            vector<string> closeBoxArgs = { saved_args[1] };
            emit_signal(TOSIGNAL(System::signalCloseBox), closeBoxArgs);
            nextOperation = Input;
            state = ExecuteCommands;
        } else if(nextOperation == ShowTree) {
            print_subjects_with_readiness();
            set_readiness(0);
        } else if(nextOperation == Recover) {
            vector<string> printArgs = {"\nReady to work"};
            emit_signal(TOSIGNAL(System::signalPrint), printArgs);
            nextOperation = Input;
            state = ExecuteCommands;
        }
        
        if(overridedState.first >= 0 && overridedState.second >= 0) {
            nextOperation = (OperationType) overridedState.first;
            state = (SystemState) overridedState.second;
            overridedState = {-1, -1};
        }
    }
    return 0;
}

void System::handleNewLine(vector<string> args) {
    saved_args = args;
    if(state == InputSafeDimentions) { 
        nextOperation = SetSafeDimentions;
    } else if(state == InputBoxKeys) {
        nextOperation = SetBoxKeys;
    } else if(state == WaitingComleteKeyEntry) {
        nextOperation = Input;
        state = ExecuteCommands;
    } else if(state == ExecuteCommands) {
        string command = args[0];
        if(command == "BOX") {
            nextOperation = SelectBox;
        } else if(command == "CLOSE_BOX") {
            nextOperation = CloseBox;
        } else if(command == "SHOW_TREE") {
            nextOperation = ShowTree;
        } else if (command == "Turn") {
            set_readiness(0);
        }
    } else if(state == WaitingClientKey) {
        if(args[0] == "CLIENT_KEY") {
            nextOperation = ApplyClientKey;
        } else if(args[0] == "CANCEL") {
            nextOperation = Reset;
        } else nextOperation = Recover;
    } else if(state == WaitingBankKey) {
        if(args[0] == "BANK_KEY") {
            nextOperation = ApplyClientKey;
        } else if(args[0] == "CANCEL") {
            nextOperation = Reset;
        } else nextOperation = Recover;
    }
}

void System::handleError(vector<string> args) {
    vector<string> printArgs = {args[0]};
    emit_signal(TOSIGNAL(System::signalPrint), printArgs);
    overridedState.first = stoi(args[1]);
    overridedState.second = stoi(args[2]);
    
}

void System::signalSetSafeDimentions(vector<string> &args) {}

void System::signalInputLine(string& payload) {}

void System::signalSetSafeBoxKeys(vector<string>&) {}

void System::signalSelectBox(vector<string> &) {}

void System::signalApplyClientKey(vector<string> &) {}

void System::signalApplyBankKey(vector<string> &) {}

void System::signalPrint(vector<string>&) {}

void System::signalResetSafe(vector<string>&) {}

void System::signalCloseBox(vector<string> &) {}
