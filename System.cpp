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
                emit_signal(TOSIGNAL(System::signalInputLine), Triple());
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
                emit_signal(TOSIGNAL(System::signalSelectBox), Triple(saved_args.second));
                nextOperation = PromptClientCode;
                break;
            }
            case ApplyClientKey:{
                emit_signal(TOSIGNAL(System::signalApplyClientKey), Triple(saved_args.second));
                nextOperation = PromptBankCode;
                break;
            }
            case ApplyBankKey:{
                emit_signal(TOSIGNAL(System::signalApplyBankKey), Triple(saved_args.second));
                nextOperation = Input;
                state = ExecuteCommands;
                break;
            }
            case Reset:{
                emit_signal(TOSIGNAL(System::signalResetSafe), Triple());
                nextOperation = Recover;
                break;
            }
            case CloseBox:{
                emit_signal(TOSIGNAL(System::signalCloseBox), Triple(saved_args.second));
                nextOperation = Input;
                state = ExecuteCommands;
                break;
            }
            case ShowTree:{
                print_subjects_with_readiness();
                nextOperation = StopSystem;
                break;}
            case Recover: {
                emit_signal(TOSIGNAL(System::signalPrint), Triple("\nReady to work"));
                nextOperation = Input;
                state = ExecuteCommands;
                break;
            }
            case PromptClientCode:{
                emit_signal(TOSIGNAL(System::signalPrint), Triple("\nEnter the code"));
                nextOperation = Input;
                state = WaitingClientKey;
                break;
            }
            case PromptBankCode:{
                emit_signal(TOSIGNAL(System::signalPrint), Triple("\nEnter the bank code"));
                nextOperation = Input;
                state = WaitingBankKey;
                break;
            }
            case ReactWrongSeq: {
                emit_signal(TOSIGNAL(System::signalPrint), Triple("\nError in the command sequence"));
                nextOperation = Recover;
                break;
            }
            case StopSystem: {
                emit_signal(TOSIGNAL(System::signalPrint), Triple("\nTurn off the safe"));
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
void System::handleNewLine(Triple args) {
    saved_args = args; // Сохраняем данные ввода для того, чтобы обработать их в цикле
    if(args.first == "SHOW_TREE") nextOperation = ShowTree;
    else if (args.first == "Turn") nextOperation = StopSystem;
    else if(state == InputSafeDimentions) nextOperation = SetSafeDimentions;
    else if(state == InputBoxKeys) nextOperation = SetBoxKeys;
    else if(state == WaitingComleteKeyEntry) nextOperation = Recover;
    else if(state == ExecuteCommands) {
        if(args.first == "BOX") nextOperation = SelectBox;
        else if(args.first == "CLOSE_BOX") nextOperation = CloseBox;
        else nextOperation = ReactWrongSeq;
    } else if(state == WaitingClientKey) {
        if(args.first == "CLIENT_KEY") nextOperation = ApplyClientKey;
        else if(args.first == "CANCEL") nextOperation = Reset;
        else nextOperation = ReactWrongSeq;
    } else if(state == WaitingBankKey) {
        if(args.first == "BANK_KEY") nextOperation = ApplyBankKey;
        else if(args.first == "CANCEL") nextOperation = Reset;
        else nextOperation = ReactWrongSeq;
    }
}

// При ошибке пишем сообщение в консоль и изменяем состояние
void System::handleError(Triple args) {
    emit_signal(TOSIGNAL(System::signalPrint), Triple(args.first));
    overridedState.first = stoi(args.second);
    overridedState.second = stoi(args.third);
}

void System::signalSetSafeDimentions(Triple &args) {}

void System::signalInputLine(Triple& payload) {}

void System::signalSetSafeBoxKeys(Triple&) {}

void System::signalSelectBox(Triple&) {}

void System::signalApplyClientKey(Triple&) {}

void System::signalApplyBankKey(Triple&) {}

void System::signalPrint(Triple&) {}

void System::signalResetSafe(Triple&) {}

void System::signalCloseBox(Triple&) {}
