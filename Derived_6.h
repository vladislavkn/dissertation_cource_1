#ifndef DERIVED_6
#define DERIVED_6

#include "Object.h"

class Derived_6 : public Object {
public:
    Derived_6(Object* parent, string name="derived6");
    // Метод сигнала
    void signal(string& payload);
    // Метод обработки сигнала
    void handler(string payload);
};

#endif
