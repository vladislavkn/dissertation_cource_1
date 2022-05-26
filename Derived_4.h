#ifndef DERIVED_4
#define DERIVED_4

#include "Object.h"

class Derived_4 : public Object {
public:
    Derived_4(Object* parent, string name="derived4");
    // Метод сигнала
    void signal(string& payload);
    // Метод обработки сигнала
    void handler(string payload);
};

#endif
