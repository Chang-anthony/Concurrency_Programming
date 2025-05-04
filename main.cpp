/*
* @author - (Anthony)
*/
#include <iostream>
#include <string>
#include <stdio.h>
#include <vector>
// #include "include/1/basic.hpp"
#include "include/2/ThreadOwner.hpp"
#include <thread>


int main()
{

    //t1 bind some_function
    // std::thread t1(some_function); 
    // //2  transfer t1 manage thread to t2, after transfer t1 invalid.
    // std::thread t2 =  std::move(t1);
    // //3 t1 can bind other thread , run some_other_function
    // t1 = std::thread(some_other_function);
    // //4  create the new Thread t3
    // std::thread t3;
    // //5  transfer t2 to t3
    // t3 = std::move(t2);
    // //6  transfer t3 to t1
    // t1 = std::move(t3);
    // std::this_thread::sleep_for(std::chrono::seconds(2000));

    // use_jointhread();
    use_parallel_acc();

    return 0;
}