#include "ThreadOwner.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <string>



void use_parallel_acc() {
    std::vector <int> vec(10000);
    for (int i = 0; i < 10000; i++) {
        vec.push_back(i);
    }
    int sum = 0;
    sum = parallel_accumulate<std::vector<int>::iterator, int>(vec.begin(), 
        vec.end(), sum);
    std::cout << "sum is " << sum << std::endl;
}


void param_function(int i) {
    for (int j = 0; j < 10; ++j) {
        std::cout << "in thread id " << std::this_thread::get_id()
            << " cur index is " << i << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void some_function() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
void some_other_function() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void use_vector() {
    std::vector<std::thread> threads;
    for (unsigned i = 0; i < 10; ++i) {
        //auto t = std::thread(param_function, i);
        //threads.push_back(std::move(t));
        /* same logic for above code */
        threads.emplace_back(param_function, i);
    }
    for (auto& entry : threads) {
        entry.join();
    }
}

void use_jointhread() {
    //1 根据线程构造函数构造joiningthread
    joining_thread j1([](int maxindex) {
        for (int i = 0; i < maxindex; i++) {
            std::cout << "in thread id " << std::this_thread::get_id()
                << " cur index is " << i << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        }, 10);
    //2 根据thread构造joiningthread
    joining_thread j2(std::thread([](int maxindex) {
        for (int i = 0; i < maxindex; i++) {
            std::cout << "in thread id " << std::this_thread::get_id()
                << " cur index is " << i << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        }, 10));
    //3 根据thread构造j3
    joining_thread j3(std::thread([](int maxindex) {
        for (int i = 0; i < maxindex; i++) {
            std::cout << "in thread id " << std::this_thread::get_id()
                << " cur index is " << i << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        }, 10));
    //4 把j3赋值给j1，joining_thread内部会等待j1汇合结束后
    //再将j3赋值给j1
    j1 = std::move(j3);
}


