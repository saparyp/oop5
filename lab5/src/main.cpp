#include "../include/DynamicHeapResource.h"
#include "../include/Queue.h"
#include <iostream>
#include <string>

struct ComplexType {
    int id;
    std::string name;
    double value;

    ComplexType(int i, std::string n, double v) 
        : id(i), name(std::move(n)), value(v) {}

    friend std::ostream& operator<<(std::ostream& os, const ComplexType& ct) {
        return os << "{" << ct.id << ", " << ct.name << ", " << ct.value << "}";
    }
};

void test_int_queue() {
    std::cout << "Test 1: Queue with int\n";
    
    DynamicHeapResource my_resource;
    Queue<int> int_queue(&my_resource);
    
    for (int i = 1; i <= 5; ++i) {
        int_queue.push(i * 10);
    }
    
    std::cout << "Queue: ";
    for (const auto& val : int_queue) {
        std::cout << val << " ";
    }
    std::cout << "\nSize: " << int_queue.size();
    std::cout << "\nFront: " << int_queue.front();
    std::cout << "\nBack: " << int_queue.back() << "\n";
    
    int_queue.pop();
    std::cout << "After pop, front: " << int_queue.front() << "\n";
    
    int_queue.clear();
    std::cout << "After clear, size: " << int_queue.size() << "\n\n";
}

void test_complex_queue() {
    std::cout << "Test 2: Queue with ComplexType\n";
    
    DynamicHeapResource my_resource;
    Queue<ComplexType> complex_queue(&my_resource);
    
    complex_queue.push(ComplexType{1, "First", 10.5});
    complex_queue.push(ComplexType{2, "Second", 20.3});
    complex_queue.push(ComplexType{3, "Third", 30.7});
    
    std::cout << "Queue:\n";
    for (const auto& item : complex_queue) {
        std::cout << "  " << item << "\n";
    }
    
    std::cout << "Size: " << complex_queue.size();
    std::cout << "\nFront: " << complex_queue.front();
    std::cout << "\nBack: " << complex_queue.back() << "\n";
    
    complex_queue.emplace(4, "Emplaced", 40.5);
    std::cout << "After emplace, back: " << complex_queue.back() << "\n\n";
}

void test_memory_reuse() {
    std::cout << "Test 3: Memory Reuse\n";
    
    DynamicHeapResource my_resource;
    
    {
        Queue<int> queue1(&my_resource);
        queue1.push(100);
        queue1.push(200);
        queue1.push(300);
        
        std::cout << "Queue1: ";
        for (int val : queue1) {
            std::cout << val << " ";
        }
        std::cout << "\n";
    }
    
    std::cout << "Queue1 destroyed\n";
    
    {
        Queue<int> queue2(&my_resource);
        queue2.push(400);
        queue2.push(500);
        
        std::cout << "Queue2: ";
        for (int val : queue2) {
            std::cout << val << " ";
        }
        std::cout << "\n";
    }
    
    std::cout << "Queue2 destroyed\n\n";
}

void test_iterator_functionality() {
    std::cout << "Test 4: Iterator Functionality\n";
    
    DynamicHeapResource my_resource;
    Queue<int> queue(&my_resource);
    
    for (int i = 1; i <= 5; ++i) {
        queue.push(i * i);
    }
    
    auto it1 = queue.begin();
    auto it2 = queue.begin();
    
    std::cout << "it1 == it2: " << (it1 == it2) << "\n";
    
    ++it1;
    std::cout << "After ++it1, it1 == it2: " << (it1 == it2) << "\n";
    
    int sum = 0;
    for (int val : queue) {
        sum += val;
    }
    std::cout << "Sum: " << sum << "\n\n";
}

int main() {
    std::cout << "=== Queue with Custom Allocator ===\n\n";
    
    try {
        test_int_queue();
        test_complex_queue();
        test_memory_reuse();
        test_iterator_functionality();
        
        std::cout << "All tests completed successfully\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}