#include "Parser.h"


void Parser::run(){
    {
        std::string line;
        std::cout << "Simple HashMap Parser (commands: set/get/del/print/exit)\n";

        while (true) {
            std::cout << "> ";
            if (!std::getline(std::cin, line)) break;

            std::istringstream iss(line);
            std::string cmd;
            iss >> cmd;

            try {
                if (cmd == "set") {
                    std::string key, value;
                    iss >> key >> value;
                    if (key.empty() || value.empty()) {
                        std::cout << "Usage: set <key> <value>\n";
                    } else {
                        map.set(key, value);
                    }
                } else if (cmd == "get") {
                    std::string key;
                    iss >> key;
                    if (key.empty()) {
                        std::cout << "Usage: get <key>\n";
                    } else {
                        std::cout << map.get(key) << "\n";
                    }
                } else if (cmd == "del") {
                    std::string key;
                    iss >> key;
                    if (key.empty()) {
                        std::cout << "Usage: del <key>\n";
                    } else {
                        map.del(key);
                    }
                } else if (cmd == "print") {
                    map.print();
                } else if (cmd == "exit") {
                    break;
                } else {
                    std::cout << "Unknown command: " << cmd << "\n";
                }
            } catch (std::exception& e) {
                std::cout << "Error: " << e.what() << "\n";
            }
        }
    }
}
