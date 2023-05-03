#include <vector>
#include <Arduino.h>

// template for the keys class
// Keys::keys is an array of valid RFID UIDs that will open the door
// First character needs to be a white space

// copy this, rename to keys.cpp and insert keys

class Keys {

public:
    inline static std::vector<String> keys = std::vector<String>({ " first key here", " another one" , " another one"});
};