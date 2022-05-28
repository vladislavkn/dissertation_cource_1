#include "Object.h"
    
// Конструктор корневого объекта
Object::Object(Object* parent, string name="object") {
    // Установка значений полей объекта
    this->parent = parent;
    this->name = name;
    
    // Если получена ссылка на объект-родитель, добавить ему в массив потомков указатель на текущий объект
    if(parent != nullptr) {
        parent->children.push_back(this);
    }
}

// Деструктор, удаляющий каждый объект-потомок
Object::~Object() {
    for(int i = 0; i < children.size(); i++) {
        delete children[i];
    }
}

// Метод установки имени объекта
void Object::set_name(string name) {
    this->name = name;
}

// Метод удаления объекта-наследника
void Object::delete_child(string name) {
    for(int i = 0; i < children.size(); i++)
        if(children[i]->get_name() == name) {
            children.erase(children.begin()+i);
            break;
        }
}

// Метод поиска объекта в иерархии по имени
Object* Object::find_by_name(string name) {
    if(this->name == name) return this;
    
    Object* result = nullptr;
    for(int i = 0; i < children.size(); i++) {
        result = children[i]->find_by_name(name);
        if(result != nullptr) return result;
    }
    return result;
}

// Метод установки готовности объекта
void Object::set_readiness(int readiness) {
    if(readiness == 0) {
        this->readiness = readiness;
        for(int i = 0; i < children.size(); i++)
            children[i]->set_readiness(0);
    }
    else if(parent && (parent->readiness != 0) || !parent)
        this->readiness = readiness;
}

// Метод получения имени объекта
string Object::get_name() {
    return name;
}

// Метод печати объекта и его потомков
void Object::print_subjects(int depth) {
    for(int i = 0; i < depth; i++) cout << "    ";
    cout << name;
    
    // Вывод имени каждого потомка
    for(int i = 0; i < children.size(); i++) {
        cout << endl;
        children[i]->print_subjects(depth+1);
    }
};

// Метод вывода дерева иерархии объектов с показателем их готовности
void Object::print_subjects_with_readiness(int depth) {
    for(int i = 0; i < depth; i++) cout << "    ";
    cout << name << ((readiness != 0) ? " is ready" : " is not ready");
    
    // Вывод имени каждого потомка
    for(int i = 0; i < children.size(); i++) {
        cout << endl;
        children[i]->print_subjects_with_readiness(depth+1);
    }
};

// Метод установки указателя на объект-родитель
void Object::set_parent(Object* new_parent) {
    // Проверка, что переданный указатель не является нулевым и текущий объект не является корневым
    if((new_parent == nullptr) || (parent == nullptr)) return;
    parent->delete_child(name);
    parent = new_parent;
    parent->children.push_back(this);
}

// Метод получения ссылки на объект-родитель
Object* Object::get_parent() {
    return this->parent;
}

// Поиск головного объекта в иерархии
Object* Object::find_head() {
    Object* current_object = this;
    while(current_object->parent) // Пока у объекта есть ссылка на родительский объект
        current_object = current_object->parent;
    return current_object;
}

// Поиск объекта по пути
Object* Object::find_by_path(string path) {
    if(path.find("//") == 0) {
        if(parent == nullptr) return this->find_by_name(path.erase(0, 2));
        return parent->find_by_path(path);
    }
    else if(path[0] == '/' && path.size() == 1) {
        if(parent == nullptr) return this;
        return parent->find_by_path("/");
    }
    else if(path[0] == '/') {
        if(parent == nullptr) return find_by_path(path.erase(0, 1));
        return parent->find_by_path(path);
    }
    else if(path.find("/") != -1 && path.find("/") != 0) {
        string step = path, obj_name;
        for(int i = 0; i < path.find("/"); i++) {
            obj_name.push_back(path[i]);
            step.erase(0, 1);
        }
        for(int i = 0; i < children.size(); i++)
            if(children[i]->get_name() == obj_name)
                return children[i]->find_by_path(step.erase(0, 1));
    }
    else if(path == ".") return this;
    else for(int i = 0; i < children.size(); i++)
            if(children[i]->get_name() == path)
                return children[i];
    return nullptr;
}

void Object::set_connection(TYPE_SIGNAL signal, Object* target, TYPE_HANDLER handler) {
    for(int i = 0; i < connections.size(); i++)
        if(connections[i]->signal == signal && connections[i]->target == target && connections[i]->handler == handler)
            return;
    Connection* c = new Connection;
    c->signal = signal;
    c->target = target;
    c->handler = handler;
    connections.push_back(c);
}

void Object::remove_connection(TYPE_SIGNAL signal, Object* target, TYPE_HANDLER handler) {
    for(int i = 0; i < connections.size(); i++) {
        Connection* c = connections[i];
        if(c->signal == signal && c->target == target && c->handler == handler) {
            connections.erase(connections.begin() + i);
            return;
        }
    }
}

void Object::emit_signal(TYPE_SIGNAL signal, vector<string>& payload) {
    if(readiness == 0) return;
    (this->*signal)(payload);
    if(connections.empty()) return;
    for(int i = 0; i < connections.size(); i++) {
        Connection* c = connections[i];
        if(c->signal == signal && c->target->readiness != 0)
            (c->target->*c->handler)(payload);
    }
}

string Object::get_absolute_path(string end) {
    if(!parent) return "/";
    return parent->get_absolute_path("/") + name + end;
}

int Object::get_class_number() {
    return class_number;
}

void Object::set_readiness_for_all(int readiness) {
    set_readiness(readiness);
    for(int i = 0; i < children.size(); i++)
        children[i]->set_readiness_for_all(readiness);
}
