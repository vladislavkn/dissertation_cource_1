#include "Derived_5.h"
    
Derived_5::Derived_5(Object* parent, string name) : Object(parent, name) {
    class_number = 5;
}
    
void Derived_5::signal(string& payload) {
    cout << "\nSignal from " << get_absolute_path();
    payload += " (class: 5)";
};

void Derived_5::handler(string payload) {
    cout << "\nSignal to " << get_absolute_path() << " Text: " << payload;
}
