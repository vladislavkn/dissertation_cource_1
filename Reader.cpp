#include "Reader.h"

Reader::Reader(Object* parent, string name) : Object(parent, name) {
    class_number = 3;
}
    
void Reader::signalNewLine(vector<string>& args) {}

void Reader::handleInputLine(vector<string> args) {
    string line;
    getline(cin, line);  
    vector<string> input;
    size_t pos = line.find(" ");
    size_t initialPos = 0;
    input.clear();
    while(pos != std::string::npos) {
        input.push_back(line.substr(initialPos, pos - initialPos));
        initialPos = pos + 1;
        pos = line.find(" ", initialPos);
    }
    input.push_back(line.substr(initialPos, min(pos, line.size()) - initialPos + 1 ));
    emit_signal(TOSIGNAL(Reader::signalNewLine), input);
}
