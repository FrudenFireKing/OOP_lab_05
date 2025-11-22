#include <iostream>
#include "../include/queue.h"

struct Person {
    std::string name;
    int age;
};

int main() {
    DynamicListMemoryResource memory_resource;

    std::cout << "Тест с int:\n";
    Queue<int> int_queue{std::pmr::polymorphic_allocator<int>(&memory_resource)};
    
    for (int i = 1; i <= 5; ++i) {
        int_queue.push(i * 10);
    }
    
    std::cout << "Очередь: ";
    for (const auto& value : int_queue) {
        std::cout << value << " ";
    }
    std::cout << "\n";
    
    std::cout << "Front: " << int_queue.front() << ", Back: " << int_queue.back() << "\n";
    
    int_queue.pop();
    std::cout << "После pop - Front: " << int_queue.front() << "\n\n";
    
    std::cout << "Тест с Person:\n";
    Queue<Person> person_queue{std::pmr::polymorphic_allocator<Person>(&memory_resource)};
    
    person_queue.push({"Alice", 25});
    person_queue.push({"Bob", 30}); 
    person_queue.push({"Charlie", 35});
    
    std::cout << "Все люди в очереди:\n";
    for (const auto& person : person_queue) {
        std::cout << "Имя: " << person.name << ", Возраст: " << person.age << "\n";
    }
    
    std::cout << "\nПервый человек: " << person_queue.front().name << "\n";
    std::cout << "Последний человек: " << person_queue.back().name << "\n";
    std::cout << "Размер очереди: " << person_queue.size() << "\n\n";
    
    // Удаление первого
    std::cout << "Удаляем первого человека...\n";
    person_queue.pop();
    std::cout << "Теперь первый: " << person_queue.front().name << "\n";
    std::cout << "Размер после удаления: " << person_queue.size() << "\n\n";
    
    // Добавляем еще одного человека
    person_queue.push({"David", 40});
    std::cout << "После добавления David:\n";
    for (const auto& person : person_queue) {
        std::cout << person.name << " " << person.age << "\n";
    }
    
    return 0;
}