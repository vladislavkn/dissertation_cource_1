#ifndef DERIVED_2
#define DERIVED_2

#include "Object.h"
#include "SafeBox.h"

class Safe : public Object {
public:
    enum SafeState {Ready, WaitingClientKey, WaitingBankKey};
    Safe(Object* parent, string name="safe");
    // Розмеры матрицы ячеек
    
    SafeBox* get_box_by_number(int number);
    int get_open_box_number();
    int getAllBoxesCount();
    
    void handleSetSafeDimentions(vector<string>);
    void handleSelectBox(vector<string>);
    void handleOpenBox(vector<string>);
    void handleCloseBox(vector<string>);
    void handleAddBox(vector<string>);
    void handleSetSafeState(vector<string>);
    void handleResetSafe(vector<string>);
    
    void signalSetBoxState(vector<string>&);
private:
    int open_box_number = 0;
    SafeState safe_state = SafeState::Ready;
    int n, m;
};

#endif
