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

void Application::build_objects_tree() {
    // Имя родительского объекта и объекта-потомка
    string parent_object_path, child_object_name;
    int class_number;
    
    // Получение и установка имени корневого объекта
    cin >> parent_object_path;
    set_name(parent_object_path);
    
    cin >> parent_object_path;
    // Ввод и обработка имен объектов; создание дерева объектов
    while(parent_object_path != "endtree") {
        cin >> child_object_name >> class_number;
        
        // Определение ссылки на объект-родитель
        Object* parent_pointer = find_by_path(parent_object_path);
        if(!parent_pointer) {
            cout << "Object tree" << endl;
            print_subjects();
            cout << "\nThe head object " << parent_object_path << " is not found";
            exit(1);
        }
        // Создание нового объекта
        if(class_number == 2) new Derived_2(parent_pointer, child_object_name);
        if(class_number == 3) new Derived_3(parent_pointer, child_object_name);
        if(class_number == 4) new Derived_4(parent_pointer, child_object_name);
        if(class_number == 5) new Derived_5(parent_pointer, child_object_name);
        if(class_number == 6) new Derived_6(parent_pointer, child_object_name);
        cin >> parent_object_path;
    }
    setup_connections();
}

void Application::setup_connections() {
    string emitter_path, receiver_path;
    Object *emitter, *receiver;
    cin >> emitter_path;
    while(emitter_path != "end_of_connections") {
        emitter = find_by_path(emitter_path);
        cin >> receiver_path;
        receiver = find_by_path(receiver_path);
        emitter->set_connection(extract_signal(emitter), receiver, extract_handler(receiver));
        cin >> emitter_path;
    }
}

int Application::start() {
    // Вывод всех дерева иерархии объектов
    cout << "Object tree" << endl;
    print_subjects();
    set_readiness_for_all(1);
    string command, path, str_arg;
    int int_arg;
    Object* obj;
    cin >> command;
    // Обработка пользовательских команд
    while(command != "END") {
        cin >> path;
        obj = find_by_path(path);
        if(command == "SET_CONDITION") {
            cin >> int_arg;
            if(!obj) cout << "\nObject " << path << " not found";
            else obj->set_readiness(int_arg);
        }
        if(command == "EMIT") {
            getline(cin, str_arg);
            if(!obj) cout << "\nObject " << path << " not found";
            else obj->emit_signal(extract_signal(obj), str_arg);
        }
        if(command == "SET_CONNECT") {
            cin >> str_arg;
            Object* receiver = find_by_path(str_arg);
            if(!obj) cout << "\nObject " << path << " not found";
            else if(!receiver) cout << "\nHandler object " << str_arg << " not found";
            else obj->set_connection(extract_signal(obj), receiver, extract_handler(receiver));
        }
        if(command == "DELETE_CONNECT") {
            cin >> str_arg;
            Object* receiver = find_by_path(str_arg);
            if(!obj) cout << "\nObject " << path << " not found";
            else if(!receiver) cout << "\nHandler object " << str_arg << " not found";
            else obj->remove_connection(extract_signal(obj), receiver, extract_handler(receiver));
        }
        cin >> command;
    }
    return 0;
}

void Application::signal(string& payload) {
    cout << "\nSignal from " << get_absolute_path();
    payload += " (class: 1)";
};

void Application::handler(string payload) {
    cout << "\nSignal to " << get_absolute_path() << " Text: " << payload;
}
