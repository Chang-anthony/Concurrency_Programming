/*
* @author - (Anthony)
*/
#include <iostream>
#include <string>
#include <stdio.h>
#include <vector>
#include "include/1/basic.hpp"
#include <thread>


int main()
{
    // create a thread 
    std::thread t1(thread_work, "Hello World!!!");
    // std::this_thread::sleep_for(std::chrono::seconds(1));
    // main thread waits for the thread to finish
    t1.join();

    // create a thread using a function object
    std::thread t2((ThreadFucntor()));
    t2.join();

    // create a thread using a lambda function
    std::thread t3([](){
        std::cout << "Lambda Thread" << std::endl;
    });
    t3.join();

    return 0;
}