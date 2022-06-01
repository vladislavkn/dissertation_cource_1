#ifndef READER
#define READER

#include "Object.h"

class Reader : public Object {
public:
    Reader(Object* parent, string name="reader");
    
    void handleInputLine(vector<string>);
    
    void signalNewLine(vector<string>&);
};

#endif
