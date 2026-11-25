#pragma once

#include <algorithm>
#include <limits>
#include <string>
#include <variant>

#include "Variable.hpp"

inline double JS_ToNumber(const std::variant<double, std::string, bool>& value) {
    if (std::holds_alternative<double>(value)) {
        return std::get<double>(value);  // Already a number, return it directly
    } else if (std::holds_alternative<bool>(value)) {
        return std::get<bool>(value) ? 1.0 : 0.0;  // Convert boolean to number
    } else if (std::holds_alternative<std::string>(value)) {
        std::string s = std::get<std::string>(value);
        
        // Trim whitespace from s
        s.erase(0, s.find_first_not_of(" \t\n\r"));
        s.erase(s.find_last_not_of(" \t\n\r") + 1);
        
        if (s.empty()) return 0.0;  // Empty string
        
        if (s.size() > 2 && s[0] == '0') {
            if (s[1] == 'x' || s[1] == 'X') {  // Hexadecimal
                s.erase(0, 2);  // Remove the "0x" prefix
                return std::strtol(s.c_str(), nullptr, 16);
            } else if (s[1] == 'b' || s[1] == 'B') { // Binary
                s.erase(0, 2);  // Remove the "0b" prefix
                return std::strtol(s.c_str(), nullptr, 2);
            } else if (s[1] == 'o' || s[1] == 'O') { // Octal... literally who is gonna be using octal numbers in their Scratch projects?
                s.erase(0, 2);  // Remove the "0o" prefix
                return std::strtol(s.c_str(), nullptr, 8);
            }
        }

        // Use std::stod to convert string to double, but handle exceptions for invalid strings
        try {
            size_t idx;
            double num = std::stod(s, &idx);
            if (idx != s.size()) return std::numeric_limits<double>::quiet_NaN();  // Not a valid number
            return num;
        } catch (...) {
            return std::numeric_limits<double>::quiet_NaN();  // Not a valid number
        }
    }
    return 0.0;  // Default case, should not happen
}

inline std::string JS_ToString(const std::variant<double, std::string, bool>& value) {
    if (std::holds_alternative<std::string>(value)) {
        return std::get<std::string>(value);
    } else if (std::holds_alternative<double>(value)) {
        return std::to_string(std::get<double>(value));
    } else if (std::holds_alternative<bool>(value)) {
        return std::get<bool>(value) ? "true" : "false";
    }
    return "";  // Default case, should not happen
}

inline bool JS_ToBool(const Value& value) {
    if (std::holds_alternative<bool>(value))
        return std::get<bool>(value);
    if (std::holds_alternative<double>(value))
        return std::get<double>(value) != 0.0;
    if (std::holds_alternative<std::string>(value)) {
        const auto& s = std::get<std::string>(value);
        return !s.empty() && s != "0" && s != "false";
    }
    return false;
}

inline std::string JS_ToLowerCase(const std::variant<double, std::string, bool>& value) {
    std::string str = JS_ToString(value);
    std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return std::tolower(c); });
    return str;
}

inline bool JS_IsWhiteSpace(const std::string& s) {
    return s.find_first_not_of(" \t\n\r\f\v") == std::string::npos;
}