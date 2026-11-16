#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

// Forward-declared so Sprite.hpp can include Variable.hpp and get Value without a circular dependency
using Value = std::variant<double, std::string, bool>;

enum class VariableType : uint8_t {
    SCALAR,
    LIST
    // Scratch also has "broadcast" variables but they are handled separately as plain string identifiers, not stored here
};

class Variable {
public:
    std::string id;
    std::string name;  // In Scratch, it's saved as a UTF-16 string, but in project.json it's always UTF-8
    VariableType type;
    bool isCloud;

    // Only one of these is active at a time depending on `type`
    std::variant<Value, std::vector<Value>> value;

    Variable(std::string id, std::string name, VariableType type, bool isCloud)
        : id(std::move(id)), name(std::move(name)), type(type), isCloud(isCloud)
    {
        switch (type) {
            case VariableType::SCALAR:
                value = Value(0.0);
                break;
            case VariableType::LIST:
                value = std::vector<Value>{};
                break;
            default:
                throw std::invalid_argument("Invalid variable type");
        }
    }
};