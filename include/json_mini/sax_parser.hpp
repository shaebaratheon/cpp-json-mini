#pragma once

#include <string>
#include <functional>
#include <istream>

namespace json_mini {

class JsonSaxHandler {
public:
    virtual ~JsonSaxHandler() = default;
    virtual bool on_null() = 0;
    virtual bool on_bool(bool val) = 0;
    virtual bool on_number(double val) = 0;
    virtual bool on_string(const std::string& val) = 0;
    virtual bool on_start_object() = 0;
    virtual bool on_key(const std::string& key) = 0;
    virtual bool on_end_object() = 0;
    virtual bool on_start_array() = 0;
    virtual bool on_end_array() = 0;
};

class SaxParser {
public:
    explicit SaxParser(std::istream& is);
    bool parse(JsonSaxHandler& handler);

private:
    std::istream& stream_;
};

} // namespace json_mini
