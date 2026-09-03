#pragma once

#include <cstddef>
#include <vector>
#include <memory>
#include <cstdlib>

namespace json_mini {

class MemoryArena {
public:
    explicit MemoryArena(size_t block_size = 65536) : block_size_(block_size) {
        allocate_block();
    }

    ~MemoryArena() {
        for (char* block : blocks_) {
            std::free(block);
        }
    }

    void* allocate(size_t bytes) {
        bytes = (bytes + 7) & ~7; // 8-byte alignment
        if (current_offset_ + bytes > block_size_) {
            allocate_block();
        }
        void* ptr = blocks_.back() + current_offset_;
        current_offset_ += bytes;
        return ptr;
    }

    void reset() {
        for (size_t i = 1; i < blocks_.size(); ++i) {
            std::free(blocks_[i]);
        }
        blocks_.resize(1);
        current_offset_ = 0;
    }

private:
    void allocate_block() {
        char* block = static_cast<char*>(std::malloc(block_size_));
        blocks_.push_back(block);
        current_offset_ = 0;
    }

    size_t block_size_;
    size_t current_offset_ = 0;
    std::vector<char*> blocks_;
};

} // namespace json_mini
