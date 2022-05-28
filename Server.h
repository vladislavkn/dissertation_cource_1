#ifndef SERVER
#define SERVER

#include "Object.h"

class Server : public Object {
public:
    Server(Object* parent, string name="server");
    
    void handleSetBoxKeys(Triple);
    
    bool validateBoxClientkey(int boxNumber, string clientKey);
    bool validateBoxBankkey(int boxNumber, string bankKey);
    unsigned long getConfiguredBoxesCount();
private:
    map<int, pair<string, string>> keys;
};

#endif
