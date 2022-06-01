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
    void handleNewLine(vector<string>);
    void handleError(vector<string>);
    
    void signalInputLine(vector<string>&);
    void signalResetSafe(vector<string>&);
    void signalSetSafeDimentions(vector<string>&);
    void signalSetSafeBoxKeys(vector<string>&);
    void signalSelectBox(vector<string>&);
    void signalApplyClientKey(vector<string>&);
    void signalApplyBankKey(vector<string>&);
    void signalPrint(vector<string>&);
    void signalCloseBox(vector<string>&);
private:
    enum OperationType { Input, SetSafeDimentions, SetBoxKeys, StopSystem, SelectBox, ApplyClientKey, ApplyBankKey, CloseBox, Cancel, ShowTree, Reset, Recover, PromptClientCode, PromptBankCode, ReactWrongSeq };
    enum SystemState { InputSafeDimentions, InputBoxKeys, WaitingComleteKeyEntry, ExecuteCommands, WaitingClientKey, WaitingBankKey};
    OperationType nextOperation = Input;
    SystemState state = InputSafeDimentions;
    pair<int, int> overridedState = {-1, -1};
    vector<string> saved_args;
};

#endif
