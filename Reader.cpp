#include "Reader.h"

Reader::Reader(Object* parent, string name) : Object(parent, name) {}
    
void Reader::signalNewLine(vector<string>&) {}

// Читаем строку из консоли и делим её по пробелам
void Reader::handleInputLine(vector<string>) {
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
    
//    cout << endl << "GOT INPUT: ";
//    for(string s : input) cout << s << " | ";
//    cout << endl;
    // Даем сигнал системе на обработку строки
    emit_signal(TOSIGNAL(Reader::signalNewLine), input);
}
