#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

using Value = std::variant<double, std::string, bool>;  // Forward-declared so Sprite.hpp can include Variable.hpp and get Value without a circular dependency

class Variable {
public:
    std::string id;
    std::string name;  // In Scratch, it's saved as a UTF-16 string, but in project.json it's always UTF-8 so we store it as UTF-8 here
    std::string type;  // "" = scalar, "list" = list
    bool isCloud;

    // Only one of these is active at a time depending on `type`
    std::variant<Value, std::vector<Value>> value;

    Variable(std::string id, std::string name, std::string type, bool isCloud)
        : id(std::move(id)), name(std::move(name)), type(type), isCloud(isCloud)
    {
        if (type == "") { value = Value(0.0); }
        else if (type == "list") { value = std::vector<Value>{}; }
    }
};