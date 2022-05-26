#ifndef APPLICATION
#define APPLICATION

#include "Object.h"
#include "Derived_2.h"
#include "Derived_3.h"
#include "Derived_4.h"
#include "Derived_5.h"
#include "Derived_6.h"
    
// Класс приложения
class Application : public Object {
public:
    // Конструктор класса, наследуемый от базового класса Object
    Application(Object* parent, string name="rootObject");
    // Метод построения дерева объектов
    void build_objects_tree();
    // Метод старта приложения
    int start();
    // Метод установки связей между объектами
    void setup_connections();
    // Метод сигнала
    void signal(string& payload);
    // Метод обработки сигнала
    void handler(string payload);
    // Метод получения меетода сигнала
    TYPE_SIGNAL extract_signal(Object* obj);
    // Метод получеения метода обработчика
    TYPE_HANDLER extract_handler(Object* obj);
};

#endif
