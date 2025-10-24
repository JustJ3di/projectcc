#ifndef JSON_H
#define JSON_H
#include <limits>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <variant>
#include <stdexcept>
#include <sstream>
#include <type_traits> 

// Forward declaration of the Json class
class Json;

// Type aliases for cleaner code
using JsonObject = std::map<std::string, Json>;
using JsonArray = std::vector<Json>;

struct FilenameTag {}; //struttura di supporto per il costruttore 

// Define the possible types a JSON value can hold
using JsonValue = std::variant<
    std::monostate,         // Represents JSON 'null'
    bool,
    int,
    double,
    std::string,
    JsonArray,              // Array of Json objects
    JsonObject              // Map of string to Json objects
>;

class Json {
public:
    // --- Members ---
    std::string file_name;
    JsonValue value;

    // --- Constructors ---
    Json() : value(std::monostate{}) {}
    
    //Il proble
    Json(FilenameTag, const std::string& filename) : file_name(filename), value(std::monostate{}) {}
    
    // Costruttore per i tipi strutturati (ancora explicit, OK)
    explicit Json(const JsonObject& dict_obj) : value(dict_obj) {}
    explicit Json(const JsonArray& array) : value(array) {}
    
    // Costruttori per i tipi semplici (int, double, bool)
    explicit Json(int simple) : value(simple) {}
    explicit Json(double simple) : value(simple) {}
    explicit Json(bool simple) : value(simple) {}
    
    Json(const std::string& simple) : value(simple) {} 
    
    // Costruttore unificato per JsonValue
    explicit Json(const JsonValue& val) : value(val) {} 
    
    // --- Public Methods (Interface) ---
    std::string get_type() const;
    void print_json() const;
    const JsonValue& get_json() const { return value; }
    void serialize(const std::string& file_output) const;
    void parse();
    
    // Helper to serialize to string
    std::string serialize_to_string() const;

private:
    // --- Private Helper Methods (Serialization) ---
    void serialize_recursive(std::ostream& os) const;
    void serialize_array(std::ostream& os, const JsonArray& array) const;
    void serialize_dict(std::ostream& os, const JsonObject& dict) const;

    // --- Private Helper Methods (Parsing) ---
    char consume_whitespace(std::ifstream& file);
    JsonObject parse_dict(std::ifstream& file);
    JsonArray parse_array(std::ifstream& file);
    JsonValue parse_simple(std::ifstream& file, char start_char);
};


// --- Public Methods ---

std::string Json::get_type() const {
    if (std::holds_alternative<JsonArray>(value)) {
        return "array";
    } else if (std::holds_alternative<JsonObject>(value)) {
        return "associative array";
    } else {
        return "simple";
    }
}

void Json::print_json() const {
    std::cout << serialize_to_string() << std::endl;
}

// Corresponds to serialize()
void Json::serialize(const std::string& file_output) const {
    std::ofstream file(file_output);
    if (file.is_open()) {
        serialize_recursive(file);
        file.close();
    } else {
        std::cerr << "Error: Unable to open file for writing: " << file_output << std::endl;
    }
}

std::string Json::serialize_to_string() const {
    std::stringstream ss;
    serialize_recursive(ss);
    return ss.str();
}


void Json::parse() {
    if (file_name.empty()) {
        throw std::runtime_error("Error: No file name provided for parsing.");
    }

    std::ifstream file(file_name);
    if (!file.is_open()) {
        throw std::runtime_error("Error: Unable to open file for reading: " + file_name);
    }

    char start_char = consume_whitespace(file);

    if (start_char == '{') {
        value = parse_dict(file);
    } else if (start_char == '[') {
        value = parse_array(file);
    } else {
        // Simple value
        file.unget(); // Put the character back for parse_simple
        value = parse_simple(file, start_char);
    }

    file.close();
}


void Json::serialize_recursive(std::ostream& os) const {
    std::visit([&](const auto& arg) {
        using T = std::decay_t<decltype(arg)>;

        if constexpr (std::is_same_v<T, std::monostate>) {
            os << "null";
        } else if constexpr (std::is_same_v<T, bool>) {
            os << (arg ? "true" : "false");
        } else if constexpr (std::is_integral_v<T> || std::is_floating_point_v<T>) {
            os << arg;
        } else if constexpr (std::is_same_v<T, std::string>) {
            os << "\"" << arg << "\"";
        } else if constexpr (std::is_same_v<T, JsonArray>) {
            serialize_array(os, arg);
        } else if constexpr (std::is_same_v<T, JsonObject>) {
            serialize_dict(os, arg);
        }
    }, value);
}

// Corresponds to serialize_array()
void Json::serialize_array(std::ostream& os, const JsonArray& array) const {
    os << "[";
    for (size_t i = 0; i < array.size(); ++i) {
        array[i].serialize_recursive(os);
        if (i < array.size() - 1) {
            os << ",";
        }
    }
    os << "]";
}

// Corresponds to serialize_dict()
void Json::serialize_dict(std::ostream& os, const JsonObject& dict) const {
    os << "{";
    size_t count = 0;
    for (const auto& pair : dict) {
        os << "\"" << pair.first << "\":";
        pair.second.serialize_recursive(os);
        if (++count < dict.size()) {
            os << ",";
        }
    }
    os << "}";
}


// Reads and discards all whitespace characters, returning the first non-space character.
char Json::consume_whitespace(std::ifstream& file) {
    char chunk;
    while (file.get(chunk) && std::isspace(chunk));
    return chunk;
}

// assumes '{' is already consumed
JsonObject Json::parse_dict(std::ifstream& file) {
    JsonObject diz;

    while (true) {
        char chunk = consume_whitespace(file);

        if (chunk == '}') {
            return diz; // End of dictionary
        }

        if (chunk == ',') {
            chunk = consume_whitespace(file); // Move to the start of the next key
        }
        
        // Key must be a string (starts with '"')
        if (chunk != '"') {
            throw std::runtime_error("Expected '\"' for key in object.");
        }

        // Parse key
        JsonValue key_value = parse_simple(file, chunk);
        std::string key = std::get<std::string>(key_value);

        // Consume ':'
        if (consume_whitespace(file) != ':') {
            throw std::runtime_error("Expected ':' after key.");
        }

        // Parse value
        chunk = consume_whitespace(file);
        
        if (chunk == '{') {
            diz[key] = Json(parse_dict(file));
        } else if (chunk == '[') {
            diz[key] = Json(parse_array(file));
        } else {
            file.unget(); // Put the chunk back for parse_simple
            diz[key] = Json(parse_simple(file, chunk));
        }
    }
}

//'[' is already consumed
JsonArray Json::parse_array(std::ifstream& file) {
    JsonArray arr;
    
    while (true) {
        char chunk = consume_whitespace(file);

        if (chunk == ']') {
            return arr; // End of array
        }

        if (chunk == ',') {
            chunk = consume_whitespace(file); // Move to the start of the next value
        }
        
        // Parse value
        if (chunk == '{') {
            arr.push_back(Json(parse_dict(file)));
        } else if (chunk == '[') {
            arr.push_back(Json(parse_array(file)));
        } else {
            file.unget(); // Put the chunk back for parse_simple
            arr.push_back(Json(parse_simple(file, chunk)));
        }
    }
}


JsonValue Json::parse_simple(std::ifstream& file, char start_char) {
    std::string token(1, start_char);
    char chunk = start_char;

    // --- Number Parsing ---
    if (std::isdigit(chunk) || chunk == '-' || chunk == '+') {
        bool is_float = false;
        
        while (file.get(chunk)) {
            if (std::isdigit(chunk)) {
                token += chunk;
            } else if (chunk == '.') {
                is_float = true;
                token += chunk;
            } else {
                file.unget();
                if (is_float) {
                    return std::stod(token);
                } else {
                    return std::stoi(token);
                }
            }
        }
        return is_float ? JsonValue(std::stod(token)) : JsonValue(std::stoi(token));
    }
    
    // --- String Parsing ---
    if (chunk == '"') {
        token.clear(); // Clear the starting '"'
        while (file.get(chunk) && chunk != '"') {
            token += chunk;
        }
        if (chunk != '"') {
             throw std::runtime_error("Unterminated string literal.");
        }
        
        // String value is fully consumed, the next read will handle the separator
        return token;
    }

    // --- Boolean/Null Parsing (t, f, n) ---
    if (chunk == 't' || chunk == 'f' || chunk == 'n') {
        while (file.get(chunk) && std::isalpha(chunk)) {
            token += chunk;
        }
        
        // Non-alpha char marks end of token, put it back
        if (file.good()) file.unget();

        if (token == "true") return true;
        if (token == "false") return false;
        if (token == "null") return std::monostate{};
    }

    throw std::runtime_error("Invalid simple value in JSON: starting with '" + std::string(1, start_char) + "'");
}


#endif // JSON_H