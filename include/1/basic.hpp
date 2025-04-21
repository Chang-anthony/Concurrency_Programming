#ifndef _BASIC_H_
#define _BASIC_H_

#include <iostream>
#include <string>

void thread_work(std::string str);

void oops();

class ThreadFucntor {
public:
    void operator()() {
        std::cout << "BackGround Task called: " << std::endl;
    }
};

struct func {
    int& _i;
    func(int & i): _i(i){}
    void operator()() {
        for (int i = 0; i < 3; i++) {
            _i = i;
            std::cout << "_i is " << _i << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
};

#endif /* _BASIC_H_ */
