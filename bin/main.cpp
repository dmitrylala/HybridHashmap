#include <iostream>
#include <string>
#include <set>

#include "hashmap.h"


void show_hashmap(const Hashmap<std::string, std::string> &h) {
    if (!h.empty())
        std::cout << "Your hashmap:" << std::endl;

    for (auto &bucket : h.data()) {
        for (auto &pair : bucket.data()) {
            auto key = pair.first;
            std::cout << "### " << key << ": " << h[key] << std::endl;
        }
    }
}


int main() {
    const std::set<std::string> actions = {"a", "r", "m", "h", "q"};
    const std::string input_invite = "Enter action [add (a), remove (r), modify (m), help (h), q (quit)]: ";
    const std::string bad_action = "Bad action. Try again.";
    const std::string enter_key_val = "Enter key and value string (sep. with \\n): ";
    const std::string enter_key = "Enter key string to remove: ";
    const std::string help = R"(Actions:
        add (a) - add pair of strings as key-value pair to hashmap
        remove (r) - remove pair from hashmap
        modify (m) - set another value to existing key
        quit (q) - quit example program)";

    Hashmap<std::string, std::string> h;
    std::string user_input;
    while (true) {
        std::cout << input_invite;
        std::cin >> user_input;

        bool is_action_incorrect = (actions.find(user_input) == actions.end());
        if (is_action_incorrect) {
            std::cout << bad_action << std::endl;
            continue;
        }

        if (user_input == "q")
            break;

        if (user_input == "h") {
            std::cout << help << std::endl;
            continue;
        }

        std::string key, value;
        if (user_input == "r") {
            std::cout << enter_key << std::endl;
            std::cin >> key;
            h.remove(key);

            show_hashmap(h);
            continue;
        }

        std::cout << enter_key_val << std::endl;
        std::cin >> key >> value;
        std::pair<std::string, std::string> p(key, value);

        if (user_input == "a")
            h.add(p);

        if ((user_input == "m") && (h.contains(key)))
            h[key] = value;

        if (user_input == "r")
            h.remove(key);

        show_hashmap(h);
    }

    return 0;
}
