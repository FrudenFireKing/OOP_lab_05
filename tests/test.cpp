#include <gtest/gtest.h>
#include "../include/queue.h"

TEST(QueueTest, EmptyQueue) {
    Queue<int> queue;
    EXPECT_TRUE(queue.empty());
    EXPECT_EQ(queue.size(), 0);
}

TEST(QueueTest, PushAndSize) {
    Queue<int> queue;
    queue.push(1);
    queue.push(2);
    queue.push(3);
    
    EXPECT_FALSE(queue.empty());
    EXPECT_EQ(queue.size(), 3);
}

TEST(QueueTest, FrontAndBack) {
    Queue<int> queue;
    queue.push(10);
    queue.push(20);
    
    EXPECT_EQ(queue.front(), 10);
    EXPECT_EQ(queue.back(), 20);
}

TEST(QueueTest, Pop) {
    Queue<int> queue;
    queue.push(1);
    queue.push(2);
    queue.push(3);
    
    queue.pop();
    EXPECT_EQ(queue.front(), 2);
    EXPECT_EQ(queue.size(), 2);
}

TEST(QueueTest, Iteration) {
    Queue<int> queue;
    queue.push(1);
    queue.push(2);
    queue.push(3);
    
    int sum = 0;
    for (const auto& value : queue) {
        sum += value;
    }
    
    EXPECT_EQ(sum, 6);
}

TEST(QueueTest, MemoryResource) {
    DynamicListMemoryResource resource;
    Queue<int> queue{std::pmr::polymorphic_allocator<int>(&resource)};  // исправлено на {}
    
    queue.push(100);
    queue.push(200);
    
    EXPECT_EQ(queue.front(), 100);
    EXPECT_EQ(queue.back(), 200);
}

struct TestStruct {
    int a;
    double b;
};

TEST(QueueTest, CustomStruct) {
    Queue<TestStruct> queue;
    queue.push({1, 2.5});
    queue.push({3, 4.5});
    
    EXPECT_EQ(queue.front().a, 1);
    EXPECT_EQ(queue.back().b, 4.5);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}