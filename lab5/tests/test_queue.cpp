#include "../include/DynamicHeapResource.h"
#include "../include/Queue.h"
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

// Test 1: Empty queue
void test_empty_queue() {
    std::cout << "Test: Empty Queue\n";
    
    DynamicHeapResource resource;
    Queue<int> queue(&resource);
    
    assert(queue.empty());
    assert(queue.size() == 0);
    assert(queue.begin() == queue.end());
    
    std::cout << "  ✓ Passed\n";
}

// Test 2: Basic push and pop operations
void test_push_pop() {
    std::cout << "Test: Push and Pop\n";
    
    DynamicHeapResource resource;
    Queue<int> queue(&resource);
    
    queue.push(1);
    queue.push(2);
    queue.push(3);
    
    assert(!queue.empty());
    assert(queue.size() == 3);
    assert(queue.front() == 1);
    assert(queue.back() == 3);
    
    queue.pop();
    assert(queue.size() == 2);
    assert(queue.front() == 2);
    assert(queue.back() == 3);
    
    queue.pop();
    assert(queue.size() == 1);
    assert(queue.front() == 3);
    assert(queue.back() == 3);
    
    queue.pop();
    assert(queue.empty());
    assert(queue.size() == 0);
    
    queue.pop(); // Should not crash
    std::cout << "  ✓ Passed\n";
}

// Test 3: Iterator functionality
void test_iterator() {
    std::cout << "Test: Iterator\n";
    
    DynamicHeapResource resource;
    Queue<int> queue(&resource);
    
    queue.push(10);
    queue.push(20);
    queue.push(30);
    
    auto it = queue.begin();
    assert(*it == 10);
    ++it;
    assert(*it == 20);
    ++it;
    assert(*it == 30);
    ++it;
    assert(it == queue.end());
    
    int sum = 0;
    for (int val : queue) {
        sum += val;
    }
    assert(sum == 60);
    
    it = queue.begin();
    auto old_it = it++;
    assert(*old_it == 10);
    assert(*it == 20);
    
    Queue<int> empty_queue(&resource);
    assert(empty_queue.begin() == empty_queue.end());
    
    std::cout << "  ✓ Passed\n";
}

// Test 4: Complex types
void test_complex_type() {
    std::cout << "Test: Complex Type\n";
    
    struct Point {
        int x, y;
        Point(int x, int y) : x(x), y(y) {}
        bool operator==(const Point& other) const { return x == other.x && y == other.y; }
    };
    
    DynamicHeapResource resource;
    Queue<Point> queue(&resource);
    
    queue.push(Point(1, 2));
    queue.push(Point(3, 4));
    queue.push(Point(5, 6));
    
    assert(queue.size() == 3);
    assert(queue.front() == Point(1, 2));
    assert(queue.back() == Point(5, 6));
    
    int x_sum = 0;
    for (const auto& p : queue) {
        x_sum += p.x;
    }
    assert(x_sum == 9);
    
    std::cout << "  ✓ Passed\n";
}

// Test 5: Memory reuse (main requirement)
void test_memory_reuse() {
    std::cout << "Test: Memory Reuse\n";
    
    DynamicHeapResource resource;
    
    {
        Queue<int> queue1(&resource);
        queue1.push(100);
        queue1.push(200);
    }
    
    {
        Queue<int> queue2(&resource);
        queue2.push(300);
        queue2.push(400);
        
        assert(queue2.front() == 300);
        assert(queue2.back() == 400);
        queue2.pop();
        assert(queue2.front() == 400);
    }
    
    std::cout << "  ✓ Passed\n";
}

// Test 6: Clear functionality
void test_clear() {
    std::cout << "Test: Clear\n";
    
    DynamicHeapResource resource;
    Queue<std::string> queue(&resource);
    
    queue.push("Hello");
    queue.push("World");
    queue.push("!");
    
    assert(queue.size() == 3);
    queue.clear();
    assert(queue.empty());
    assert(queue.size() == 0);
    assert(queue.begin() == queue.end());
    
    queue.push("New");
    assert(queue.size() == 1);
    assert(queue.front() == "New");
    
    std::cout << "  ✓ Passed\n";
}

// Test 7: Exception safety
void test_exception_safety() {
    std::cout << "Test: Exception Safety\n";
    
    struct ThrowOnCopy {
        int value;
        ThrowOnCopy(int v) : value(v) {}
        ThrowOnCopy(const ThrowOnCopy& other) {
            if (other.value == 2) {
                throw std::runtime_error("Copy failed!");
            }
            value = other.value;
        }
    };
    
    DynamicHeapResource resource;
    Queue<ThrowOnCopy> queue(&resource);
    
    queue.push(ThrowOnCopy(1));
    
    try {
        queue.push(ThrowOnCopy(2));
        assert(false);
    } catch (const std::runtime_error& e) {
        assert(queue.size() == 1);
        assert(queue.front().value == 1);
    }
    
    std::cout << "  ✓ Passed\n";
}

// Test 8: Move semantics
void test_move_semantics() {
    std::cout << "Test: Move Semantics\n";
    
    DynamicHeapResource resource;
    Queue<std::string> queue(&resource);
    
    std::string large_string = "Long string";
    queue.push(std::move(large_string));
    assert(large_string.empty());
    
    assert(queue.size() == 1);
    assert(queue.front() == "Long string");
    
    std::cout << "  ✓ Passed\n";
}

// Run all tests
void run_all_tests() {
    std::cout << "Running all tests...\n";
    std::cout << "=====================\n";
    
    test_empty_queue();
    test_push_pop();
    test_iterator();
    test_complex_type();
    test_memory_reuse();
    test_clear();
    test_exception_safety();
    test_move_semantics();
    
    std::cout << "\n=====================\n";
    std::cout << "All tests passed!\n";
}

int main() {
    try {
        run_all_tests();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Test failed with unknown exception" << std::endl;
        return 1;
    }
}