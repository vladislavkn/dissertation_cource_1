#include "Server.h"
    

Server::Server(Object* parent, string name) : Object(parent, name) {}

void Server::handleSetBoxKeys(vector<string> args) {
    int boxNumber = stoi(args[0]);
    pair<string, string> keysPair(args[1], args[2]);
    keys[boxNumber] = keysPair;
}

bool Server::validateBoxClientkey(int boxNumber, string clientKey) {
    if(keys.find(boxNumber) == keys.end()) return false;
    return keys[boxNumber].first == clientKey;
}

bool Server::validateBoxBankkey(int boxNumber, string bankKey) {
    if(keys.find(boxNumber) == keys.end()) return false;
    return keys[boxNumber].second == bankKey;
}

unsigned long Server::getConfiguredBoxesCount() {
    return keys.size();
}
