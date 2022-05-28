#ifndef CONTROL
#define CONTROL

#include "Object.h"

class Control : public Object {
public:
    Control(Object* parent, string name="control");
    
    void signalSetSafeDimentions(Triple&);
    void signalOpenBox(Triple&);
    void signalSelectBox(Triple&);
    void signalCloseBox(Triple&);
    void signalSetBoxKeys(Triple&);
    void signalError(Triple&);
    void signalAddBox(Triple&);
    void signaleSetSafeState(Triple&);
    void signalResetSafe(Triple&);
    
    void handleResetSafe(Triple);
    void handleSetSafeDimentions(Triple);
    void handleSetBoxKeys(Triple);
    void handleSelectBox(Triple);
    void handleApplyClientKey(Triple);
    void handleApplyBankKey(Triple);
    void handleCloseBox(Triple);
    void handleCancelBoxOpening(Triple);
    bool isAllBoxesConfigured();
};

#endif
