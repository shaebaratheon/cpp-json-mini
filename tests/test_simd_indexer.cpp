#include "json_mini/simd_parser.hpp"
#include <cassert>
#include <iostream>

int main() {
    using namespace json_mini;
    std::string_view json = "{\"a\": 1, \"b\": [2, 3]}";
    auto indices = SimdStructuralIndexer::find_structural_indices(json);
    assert(indices.size() == 7);
    std::cout << "SIMD structural indexer verified with " << indices.size() << " structural tokens." << std::endl;
    return 0;
}
