#pragma once

#include <string>
#include <iostream>
#include <sstream> // Removed because not used in this header
#include "ChainHashMap.h"


class Parser {
private:
    ChainHashMap<std::string, std::string, 10> map;

public:
    void run();
};