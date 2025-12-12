#ifndef QUEUE_H
#define QUEUE_H

#include <memory_resource>
#include <iterator>
#include <stdexcept>
#include <utility>

template<typename T>
class Queue {
private:
    struct Node {
        T data;
        Node* next;
        
        template<typename... Args>
        Node(Node* nxt, Args&&... args) 
            : data(std::forward<Args>(args)...), next(nxt) {}
    };

    Node* head = nullptr;
    Node* tail = nullptr;
    std::size_t size_ = 0;

    using allocator_type = std::pmr::polymorphic_allocator<Node>;
    allocator_type alloc;

    template<typename... Args>
    Node* create_node(Node* next, Args&&... args) {
        Node* new_node = alloc.allocate(1);
        try {
            alloc.construct(new_node, next, std::forward<Args>(args)...);
        } catch (...) {
            alloc.deallocate(new_node, 1);
            throw;
        }
        return new_node;
    }

public:
    class Iterator {
    private:
        Node* current;

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        explicit Iterator(Node* node = nullptr) : current(node) {}

        reference operator*() const { 
            if (!current) throw std::runtime_error("Dereferencing end iterator");
            return current->data; 
        }
        
        pointer operator->() const { 
            if (!current) throw std::runtime_error("Dereferencing end iterator");
            return &current->data; 
        }

        Iterator& operator++() {
            if (current) current = current->next;
            return *this;
        }

        Iterator operator++(int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        bool operator==(const Iterator& other) const { 
            return current == other.current; 
        }
        
        bool operator!=(const Iterator& other) const { 
            return !(*this == other); 
        }
    };

    explicit Queue(std::pmr::memory_resource* mr = std::pmr::get_default_resource())
        : alloc(mr) {}

    Queue(const Queue&) = delete;
    Queue& operator=(const Queue&) = delete;

    ~Queue() {
        clear();
    }

    template<typename U>
    void push(U&& value) {
        Node* new_node = create_node(nullptr, std::forward<U>(value));

        if (!tail) {
            head = tail = new_node;
        } else {
            tail->next = new_node;
            tail = new_node;
        }
        ++size_;
    }
    
    template<typename... Args>
    void emplace(Args&&... args) {
        Node* new_node = create_node(nullptr, std::forward<Args>(args)...);

        if (!tail) {
            head = tail = new_node;
        } else {
            tail->next = new_node;
            tail = new_node;
        }
        ++size_;
    }
    
    void pop() {
        if (!head) return;
        
        Node* to_delete = head;
        head = head->next;
        if (!head) tail = nullptr;

        alloc.destroy(to_delete);
        alloc.deallocate(to_delete, 1);
        --size_;
    }
    
    T& front() {
        if (!head) throw std::runtime_error("Queue is empty");
        return head->data;
    }
    
    const T& front() const {
        if (!head) throw std::runtime_error("Queue is empty");
        return head->data;
    }
    
    T& back() {
        if (!tail) throw std::runtime_error("Queue is empty");
        return tail->data;
    }
    
    const T& back() const {
        if (!tail) throw std::runtime_error("Queue is empty");
        return tail->data;
    }
    
    bool empty() const {
        return size_ == 0;
    }
    
    std::size_t size() const {
        return size_;
    }
    
    void clear() {
        while (!empty()) {
            pop();
        }
    }

    Iterator begin() { return Iterator(head); }
    Iterator end() { return Iterator(nullptr); }
    
    Iterator begin() const { return Iterator(head); }
    Iterator end() const { return Iterator(nullptr); }
};

#endif