#include "Derived_3.h"

Derived_3::Derived_3(Object* parent, string name) : Object(parent, name) {
    class_number = 3;
}
    
void Derived_3::signal(string& payload) {
    cout << "\nSignal from " << get_absolute_path();
    payload += " (class: 3)";
};

void Derived_3::handler(string payload) {
    cout << "\nSignal to " << get_absolute_path() << " Text: " << payload;
}
