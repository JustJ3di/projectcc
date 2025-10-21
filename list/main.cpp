#include "list.hpp" // Assumendo che il tuo codice sia qui
#include <iostream>
#include <string>


void TEST1(list<int>& intList){

    intList.push_front(10);
    intList.push_front(20);
    intList.push_front(30);

}


int main() {
    list<int> intList;

    TEST1(intList);
    // 2. Lettura e stampa con l'iterator
    std::cout << "Elementi nella lista: ";
    for (const auto& value : intList) { // Usa un range-based for loop (richiede begin/end)
        std::cout << value << " ";
    }
    std::cout << "\n\n";

    std::cout << "--- Test Lista di Stringhe ---\n";
    list<std::string> stringList;
    stringList.push_front("C++");
    stringList.push_front("Coding");
    stringList.push_front("Inizio");

    std::cout << "Elementi nella lista: ";
    for (auto it = stringList.begin(); it != stringList.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n";

    // 3. La memoria viene rilasciata quando intList e stringList escono dallo scope (grazie al distruttore)
    
    return 0;
}