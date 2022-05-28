#ifndef APPLICATION
#define APPLICATION

#include "Object.h"
#include "Safe.h"
#include "Reader.h"
#include "SafeBox.h"
#include "Control.h"
#include "Server.h"
#include "Display.h"

// Класс приложения
class System : public Object {
public:
    // Конструктор класса, наследуемый от базового класса Object
    System(Object* parent, string name="system");
    // Метод построения дерева объектов
    void build_objects_tree();
    // Метод старта приложения
    int start();
    // Метод обработки сигнала
    void handleNewLine(Triple);
    void handleError(Triple);
    
    void signalInputLine(Triple&);
    void signalResetSafe(Triple&);
    void signalSetSafeDimentions(Triple&);
    void signalSetSafeBoxKeys(Triple&);
    void signalSelectBox(Triple&);
    void signalApplyClientKey(Triple&);
    void signalApplyBankKey(Triple&);
    void signalPrint(Triple&);
    void signalCloseBox(Triple&);
private:
    enum OperationType { Input, SetSafeDimentions, SetBoxKeys, StopSystem, SelectBox, ApplyClientKey, ApplyBankKey, CloseBox, Cancel, ShowTree, Reset, Recover, PromptClientCode, PromptBankCode };
    enum SystemState { InputSafeDimentions, InputBoxKeys, WaitingComleteKeyEntry, ExecuteCommands, WaitingClientKey, WaitingBankKey};
    OperationType nextOperation = Input;
    SystemState state = InputSafeDimentions;
    pair<int, int> overridedState = {-1, -1};
    Triple saved_args;
};

#endif
