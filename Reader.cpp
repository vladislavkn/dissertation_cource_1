#include "Reader.h"

Reader::Reader(Object* parent, string name) : Object(parent, name) {}
    
void Reader::signalNewLine(Triple&) {}

// Читаем строку из консоли и делим её по пробелам
void Reader::handleInputLine(Triple) {
    string line;
    getline(cin, line);  
    vector<string> input;
    size_t pos = line.find(" ");
    size_t initialPos = 0;
    
    while(pos != std::string::npos) {
        input.push_back(line.substr(initialPos, pos - initialPos));
        initialPos = pos + 1;
        pos = line.find(" ", initialPos);
    }
    input.push_back(line.substr(initialPos, min(pos, line.size()) - initialPos + 1 ));
    
    Triple args;
    if(input.size() >= 1) args.first = input[0];
    if(input.size() >= 2) args.second = input[1];
    if(input.size() >= 3) args.third = input[2];
    
    // Даем сигнал системе на обработку строки
    emit_signal(TOSIGNAL(Reader::signalNewLine), args);
}
