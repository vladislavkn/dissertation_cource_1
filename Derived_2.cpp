#include "Derived_2.h"
    
Derived_2::Derived_2(Object* parent, string name) : Object(parent, name) {
    class_number = 2;
}
    
void Derived_2::signal(string& payload) {
    cout << "\nSignal from " << get_absolute_path();
    payload += " (class: 2)";
};

void Derived_2::handler(string payload) {
    cout << "\nSignal to " << get_absolute_path() << " Text: " << payload;
}
