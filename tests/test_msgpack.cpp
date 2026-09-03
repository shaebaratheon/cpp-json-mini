#include "json_mini/msgpack.hpp"
#include <cassert>
#include <iostream>

int main() {
    using namespace json_mini;
    JsonObject obj;
    obj["key"] = "hello";

    auto bytes = MsgPackSerializer::pack(JsonValue("hello msgpack"));
    assert(!bytes.empty());
    std::cout << "MsgPack packed byte stream size: " << bytes.size() << std::endl;
    return 0;
}
