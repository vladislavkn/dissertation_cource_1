#include "System.h"

System::System(Object* parent, string name) : Object(parent, name) {}

void System::build_objects_tree() {
    // Создаем основные объекты
    Control* control = new Control(this);
    Server* server = new Server(control);
    Safe* safe = new Safe(control);
    Reader* reader = new Reader(this);
    Display* display = new Display(this);
    
    // Строим связи
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
    // Цикл тактов
    while(readiness != 0) {
        switch (nextOperation) {
            case Input:{
                emit_signal(TOSIGNAL(System::signalInputLine), {});
                break;
            }
            case SetSafeDimentions:{
                emit_signal(TOSIGNAL(System::signalSetSafeDimentions), saved_args);
                state = InputBoxKeys;
                nextOperation = Input;
                break;
            }
            case SetBoxKeys:{
                emit_signal(TOSIGNAL(System::signalSetSafeBoxKeys), saved_args);
                Control* control = (Control*) find_by_path("/control");
                if(control->isAllBoxesConfigured()) state = WaitingComleteKeyEntry;
                nextOperation = Input;
                break;
            }
            case SelectBox:{
                emit_signal(TOSIGNAL(System::signalSelectBox), {saved_args[1]});
                nextOperation = PromptClientCode;
                break;
            }
            case ApplyClientKey:{
                emit_signal(TOSIGNAL(System::signalApplyClientKey), {saved_args[1]});
                nextOperation = PromptBankCode;
                break;
            }
            case ApplyBankKey:{
                emit_signal(TOSIGNAL(System::signalApplyBankKey), {saved_args[1]});
                nextOperation = Input;
                state = ExecuteCommands;
                break;
            }
            case Reset:{
                emit_signal(TOSIGNAL(System::signalResetSafe), {});
                nextOperation = Recover;
                break;
            }
            case CloseBox:{
                emit_signal(TOSIGNAL(System::signalCloseBox), {saved_args[1]});
                nextOperation = Input;
                state = ExecuteCommands;
                break;
            }
            case ShowTree:{
                print_subjects_with_readiness();
                nextOperation = StopSystem;
                break;}
            case Recover: {
                emit_signal(TOSIGNAL(System::signalPrint), {"\nReady to work"});
                nextOperation = Input;
                state = ExecuteCommands;
                break;
            }
            case PromptClientCode:{
                emit_signal(TOSIGNAL(System::signalPrint), {"\nEnter the code"});
                nextOperation = Input;
                state = WaitingClientKey;
                break;
            }
            case PromptBankCode:{
                emit_signal(TOSIGNAL(System::signalPrint), {"\nEnter the bank code"});
                nextOperation = Input;
                state = WaitingBankKey;
                break;
            }
            case ReactWrongSeq: {
                emit_signal(TOSIGNAL(System::signalPrint), {"\nError in the command sequence"});
                nextOperation = Recover;
                break;
            }
            case StopSystem: {
                emit_signal(TOSIGNAL(System::signalPrint), {"\nTurn off the safe"});
                set_readiness(0);
            }
            default:
                break;
        }
        
        // Если была ошибка, обновляем состояние
        if(overridedState.first >= 0 && overridedState.second >= 0) {
            nextOperation = (OperationType) overridedState.first;
            state = (SystemState) overridedState.second;
            overridedState = {-1, -1};
        }
    }
    return 0;
}

// Обработчик ввода из консоли
void System::handleNewLine(vector<string> args) {
    saved_args = args; // Сохраняем данные ввода для того, чтобы обработать их в цикле
    if(args[0] == "SHOW_TREE") nextOperation = ShowTree;
    else if (args[0] == "Turn") nextOperation = StopSystem;
    else if(state == InputSafeDimentions) nextOperation = SetSafeDimentions;
    else if(state == InputBoxKeys) nextOperation = SetBoxKeys;
    else if(state == WaitingComleteKeyEntry) nextOperation = Recover;
    else if(state == ExecuteCommands) {
        if(args[0] == "BOX") nextOperation = SelectBox;
        else if(args[0] == "CLOSE_BOX") nextOperation = CloseBox;
        else nextOperation = ReactWrongSeq;
    } else if(state == WaitingClientKey) {
        if(args[0] == "CLIENT_KEY") nextOperation = ApplyClientKey;
        else if(args[0] == "CANCEL") nextOperation = Reset;
        else nextOperation = ReactWrongSeq;
    } else if(state == WaitingBankKey) {
        if(args[0] == "BANK_KEY") nextOperation = ApplyBankKey;
        else if(args[0] == "CANCEL") nextOperation = Reset;
        else nextOperation = ReactWrongSeq;
    }
}

// При ошибке пишем сообщение в консоль и изменяем состояние
void System::handleError(vector<string> args) {
    emit_signal(TOSIGNAL(System::signalPrint), {args[0]});
    emit_signal(TOSIGNAL(System::signalPrint), {"\nReady to work"});
    overridedState.first = stoi(args[1]);
    overridedState.second = stoi(args[2]);
}

void System::signalSetSafeDimentions(vector<string> &args) {}

void System::signalInputLine(vector<string>& payload) {}

void System::signalSetSafeBoxKeys(vector<string>&) {}

void System::signalSelectBox(vector<string>&) {}

void System::signalApplyClientKey(vector<string>&) {}

void System::signalApplyBankKey(vector<string>&) {}

void System::signalPrint(vector<string>&) {}

void System::signalResetSafe(vector<string>&) {}

void System::signalCloseBox(vector<string>&) {}
