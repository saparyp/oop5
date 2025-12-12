#ifndef DYNAMIC_HEAP_RESOURCE_H
#define DYNAMIC_HEAP_RESOURCE_H

#include <memory_resource>
#include <list>
#include <cstdlib>

class DynamicHeapResource : public std::pmr::memory_resource {
private:
    struct BlockInfo {
        void* ptr;
        std::size_t size;
        bool is_free;
    };

    std::list<BlockInfo> blocks;

protected:
    void* do_allocate(std::size_t bytes, std::size_t alignment) override;
    void do_deallocate(void* ptr, std::size_t bytes, std::size_t alignment) override;
    bool do_is_equal(const memory_resource& other) const noexcept override;

public:
    DynamicHeapResource();
    ~DynamicHeapResource() override;

    DynamicHeapResource(const DynamicHeapResource&) = delete;
    DynamicHeapResource& operator=(const DynamicHeapResource&) = delete;
};

#endif