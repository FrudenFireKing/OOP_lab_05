#include <cstddef>
#include <iterator>
#include <memory_resource>
#include <cstdlib>
#include <list>
#include <new>
#include <iostream>
#include <stdexcept>

class DynamicListMemoryResource: public std::pmr::memory_resource{
    private: 
        // ”казатель на конкретное место в пам€ти, где блок будет выделен после NEW, size_ - размер блока, 
        // is free - проверка на переиспользование
        struct Block{
            void* ptr;
            size_t size_;
            bool is_free;
        };
        std::list <Block> blocks;
    public:
        void* do_allocate(size_t bytes, [[maybe_unused]] size_t alignment) override{
            for (auto& block : blocks){
                if (block.is_free && block.size_ >= bytes){
                    block.is_free = false;
                    return block.ptr;
                }
            }
            void* new_memory = ::operator new(bytes);
            blocks.push_back({new_memory, bytes, false});
            return new_memory;
        }

        void do_deallocate(void* p, [[maybe_unused]] size_t bytes, [[maybe_unused]] size_t alignment) override{
            for(auto& block : blocks){
                if( p == block.ptr){
                    block.is_free = true;
                    return;
                }
            }
        }
        bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override{
            return this == &other;
        }
        ~DynamicListMemoryResource() override {
            for (auto& block : blocks) {
                if (!block.is_free) { 
                    ::operator delete(block.ptr);
                }
            }
        }
};

template<typename T, typename Allocator = std::pmr::polymorphic_allocator<T>>
class Queue{
private:
    struct Node {
        T data;
        Node* next;
    };

    using NodeAllocator =
        typename std::allocator_traits<Allocator>::template rebind_alloc<Node>;

    NodeAllocator alloc;  // <-- теперь тип уже известен!
    Node* head;
    Node* tail;
    size_t size_;

public:
    class iterator {
    private:
        Node* current;
    public:
        iterator(Node* node) : current(node) {}
        T& operator*() { return current->data; }
        T* operator->() { return &(current->data); }
        iterator& operator++() { if (current) current = current->next; return *this; }
        iterator operator++(int) { iterator tmp = *this; ++(*this); return tmp; }
        bool operator==(const iterator& other) const { return current == other.current; }
        bool operator!=(const iterator& other) const { return current != other.current; }
    };

    class const_iterator {
    private:
        const Node* current;
    public:
        const_iterator(const Node* node) : current(node) {}
        const T& operator*() const { return current->data; }
        const T* operator->() const { return &(current->data); }
        const_iterator& operator++() { if (current) current = current->next; return *this; }
        const_iterator operator++(int) { const_iterator tmp = *this; ++(*this); return tmp; }
        bool operator==(const const_iterator& other) const { return current == other.current; }
        bool operator!=(const const_iterator& other) const { return current != other.current; }
    };

    Queue() : alloc(), head(nullptr), tail(nullptr), size_(0) {}

    Queue(const Allocator& allocator)
        : alloc(allocator), head(nullptr), tail(nullptr), size_(0) {}

    iterator begin() { return iterator(head); }
    iterator end() { return iterator(nullptr); }
    const_iterator begin() const { return const_iterator(head); }
    const_iterator end() const { return const_iterator(nullptr); }

    void push(const T& value) {
        Node* new_node = alloc.allocate(1);
        std::allocator_traits<NodeAllocator>::construct(alloc, new_node, Node{value, nullptr});

        if (empty()) {
            head = tail = new_node;
        } else {
            tail->next = new_node;
            tail = new_node;
        }
        size_++;
    }

    void pop() {
        if (!empty()) {
            Node* old_head = head;
            head = head->next;
            if (head == nullptr)
                tail = nullptr;

            std::allocator_traits<NodeAllocator>::destroy(alloc, old_head);
            alloc.deallocate(old_head, 1);
            size_--;
        } else {
            std::cout << "ќшибка удалени€ из пустого." << std::endl;
        }
    }

    T& front() {
        if (empty()) throw std::runtime_error("Queue is empty");
        return head->data;
    }

    const T& front() const {
        if (empty()) throw std::runtime_error("Queue is empty");
        return head->data;
    }

    T& back() {
        if (empty()) throw std::runtime_error("Queue is empty");
        return tail->data;
    }

    const T& back() const {
        if (empty()) throw std::runtime_error("Queue is empty");
        return tail->data;
    }

    bool empty() const { return size_ == 0; }
    size_t size() const { return size_; }

    ~Queue() {
        while (!empty()) pop();
    }
};