#ifndef CONTROL
#define CONTROL

#include "Object.h"

class Control : public Object {
public:
    Control(Object* parent, string name="control");
    
    void signalSetSafeDimentions(vector<string>&);
    void signalOpenBox(vector<string>&);
    void signalSelectBox(vector<string>&);
    void signalCloseBox(vector<string>&);
    void signalSetBoxKeys(vector<string>&);
    void signalError(vector<string>&);
    void signalAddBox(vector<string>&);
    void signaleSetSafeState(vector<string>&);
    void signalResetSafe(vector<string>&);
    
    void handleResetSafe(vector<string>);
    void handleSetSafeDimentions(vector<string>);
    void handleSetBoxKeys(vector<string>);
    void handleSelectBox(vector<string>);
    void handleApplyClientKey(vector<string>);
    void handleApplyBankKey(vector<string>);
    void handleCloseBox(vector<string>);
    void handleCancelBoxOpening(vector<string>);
    bool isAllBoxesConfigured();
};

#endif
