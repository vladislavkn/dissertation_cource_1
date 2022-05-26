#include "Derived_4.h"
    
Derived_4::Derived_4(Object* parent, string name) : Object(parent, name) {
    class_number = 4;
}
    
void Derived_4::signal(string& payload) {
    cout << "\nSignal from " << get_absolute_path();
    payload += " (class: 4)";
};

void Derived_4::handler(string payload) {
    cout << "\nSignal to " << get_absolute_path() << " Text: " << payload;
}
