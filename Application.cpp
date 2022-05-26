#include "Application.h"

Application::Application(Object* parent, string name) : Object(parent, name) {
    class_number = 1;
}
    
Object::TYPE_SIGNAL Application::extract_signal(Object* obj) {
    int class_number = obj->get_class_number();
    if(class_number == 1) return TOSIGNAL(Application::signal);
    if(class_number == 2) return TOSIGNAL(Derived_2::signal);
    if(class_number == 3) return TOSIGNAL(Derived_3::signal);
    if(class_number == 4) return TOSIGNAL(Derived_4::signal);
    if(class_number == 5) return TOSIGNAL(Derived_5::signal);
    return TOSIGNAL(Derived_6::signal);
}

Object::TYPE_HANDLER Application::extract_handler(Object* obj) {
    int class_number = obj->get_class_number();
    if(class_number == 1) return TOHANDLER(Application::handler);
    if(class_number == 2) return TOHANDLER(Derived_2::handler);
    if(class_number == 3) return TOHANDLER(Derived_3::handler);
    if(class_number == 4) return TOHANDLER(Derived_4::handler);
    if(class_number == 5) return TOHANDLER(Derived_5::handler);
    return TOHANDLER(Derived_6::handler);
}

void Application::build_objects_tree() {}

int Application::start() {
    return 0;
}

void Application::signal(string& payload) {
    cout << "\nSignal from " << get_absolute_path();
    payload += " (class: 1)";
};

void Application::handler(string payload) {
    cout << "\nSignal to " << get_absolute_path() << " Text: " << payload;
}
