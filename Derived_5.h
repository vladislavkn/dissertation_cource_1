#ifndef DERIVED_5
#define DERIVED_5

#include "Object.h"

class Derived_5 : public Object {
public:
    Derived_5(Object* parent, string name="derived5");
    // Метод сигнала
    void signal(string& payload);
    // Метод обработки сигнала
    void handler(string payload);
};

#endif
