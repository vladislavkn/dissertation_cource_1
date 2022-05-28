#ifndef SAFEBOX
#define SAFEBOX

#include "Object.h"

class SafeBox : public Object {
public:
    enum SafeBoxState { Open, Closed };
    SafeBox(Object* parent, int box_number, string name="safeBox");
    SafeBoxState get_box_state();
    void set_box_state(SafeBoxState new_safe_box_state);
    int get_box_number();
    
    void handleSetBoxState(Triple);
private:
    int box_number;
    SafeBoxState state = SafeBoxState::Closed;
};

#endif
