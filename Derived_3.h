#ifndef DERIVED_3
#define DERIVED_3

#include "Object.h"

class Derived_3 : public Object {
public:
    Derived_3(Object* parent, string name="derived3");
    // Метод сигнала
    void signal(string& payload);
    // Метод обработки сигнала
    void handler(string payload);
};

#endif
