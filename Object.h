#ifndef OBJECT
#define OBJECT
#define TOSIGNAL(signal_f) (TYPE_SIGNAL) (& signal_f)
#define TOHANDLER(handler_f) (TYPE_HANDLER) (& handler_f)

#include <vector>
#include <iostream>
#include <string>
#include <map>
#include <cmath>
#include "Triple.h"
using namespace std;

struct Connection;
class Object;

// Базовый класс
class Object {
    public:
        typedef void (Object::*TYPE_SIGNAL)(Triple&);
        typedef void (Object::*TYPE_HANDLER)(Triple);
        // Конструктор базового класса
        Object(Object*, string name);
        // Деструктор базового класса
        ~Object();
        // Метод установки имени
        void set_name(string);
        // Метод получения имени объекта
        string get_name();
        // Метод вывода объекта и его потомков
        void print_subjects(int depth=0);
        // Метод установки указателя на объект-родитель
        void set_parent(Object*);
        // Метод поиска объекта в иерархии по имени
        Object* find_by_name(string name);
        // Метод получения указателя на объект-родитель
        Object* get_parent();
        // Метод установки готовности объекта
        void set_readiness(int readiness);
        // Метод вывода дерева иерархии объектов с показателем их готовности
        void print_subjects_with_readiness(int depth=0);
        // Поиск объекта по его пути
        Object* find_by_path(string path);
        // Поиск головного объекта в иерархии
        Object* find_head();
        // Метод установки связи
        void set_connection(TYPE_SIGNAL, Object*, TYPE_HANDLER);
        // Методу удаления связи
        void remove_connection(TYPE_SIGNAL, Object*, TYPE_HANDLER);
        // Метод выдачи сигнала
        void emit_signal(TYPE_SIGNAL, Triple);
        // Метод получения абсолютного пути до текущего объекта
        string get_absolute_path(string end="");
        // метод установки готовности всех объектов
        void set_readiness_for_all(int);
    protected:
        // Массив указателей на объекты-потомки
        vector<Object*> children;
        int readiness = 0;
    private:
        struct Connection {
            TYPE_SIGNAL signal;
            Object* target;
            TYPE_HANDLER handler;
        };
        // Массив связанных объектов
        vector<Connection*> connections;
        // Метод удаления объекта-потомка
        void delete_child(string);
        // Имя объекта
        string name;
        // Указатель на родительский объект
        Object* parent;
};

#endif
