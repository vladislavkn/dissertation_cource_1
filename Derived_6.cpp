#include "Derived_6.h"
    
Derived_6::Derived_6(Object* parent, string name) : Object(parent, name) {
    class_number = 6;
}
    
void Derived_6::signal(string& payload) {
    cout << "\nSignal from " << get_absolute_path();
    payload += " (class: 6)";
};

void Derived_6::handler(string payload) {
    cout << "\nSignal to " << get_absolute_path() << " Text: " << payload;
}
