#ifndef DERIVED_2
#define DERIVED_2

#include "Object.h"

class Derived_2 : public Object {
public:
    Derived_2(Object* parent, string name="derived2");
    // Метод сигнала
    void signal(string& payload);
    // Метод обработки сигнала
    void handler(string payload);
};

#endif
