#include "../include/DynamicHeapResource.h"
#include <iostream>

DynamicHeapResource::DynamicHeapResource() {}

void* DynamicHeapResource::do_allocate(std::size_t bytes, std::size_t alignment) {
    for (auto& block : blocks) {
        if (block.is_free && block.size >= bytes) {
            block.is_free = false;
            std::cout << "[Alloc] Reuse: " << block.ptr << " (" << bytes << " bytes)\n";
            return block.ptr;
        }
    }

    void* ptr = std::aligned_alloc(alignment, bytes);
    if (!ptr) {
        throw std::bad_alloc();
    }
    blocks.push_back({ptr, bytes, false});
    std::cout << "[Alloc] New: " << ptr << " (" << bytes << " bytes)\n";
    return ptr;
}

void DynamicHeapResource::do_deallocate(void* ptr, std::size_t bytes, std::size_t) {
    for (auto& block : blocks) {
        if (block.ptr == ptr && !block.is_free) {
            block.is_free = true;
            std::cout << "[Alloc] Free: " << ptr << "\n";
            return;
        }
    }
}

bool DynamicHeapResource::do_is_equal(const memory_resource& other) const noexcept {
    return this == &other;
}

DynamicHeapResource::~DynamicHeapResource() {
    for (auto& block : blocks) {
        if (!block.is_free) {
            std::cout << "[Alloc] Cleanup leaked: " << block.ptr << "\n";
        }
        std::free(block.ptr);
    }
    blocks.clear();
}