#ifndef READER
#define READER

#include "Object.h"

class Reader : public Object {
public:
    Reader(Object* parent, string name="reader");
    vector<string> nextLine();
    
    void handleInputLine(Triple);
    
    void signalNewLine(Triple&);
private:
    vector<string> input;
};

#endif
