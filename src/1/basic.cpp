#include <basic.hpp>
#include <thread>

void thread_work(std::string str)
{
    std::cout << "Thread: " << str << std::endl;
}

void oops()
{
    int some_local_state = 0;
    func my_func(some_local_state);
    std::thread t(my_func);
    t.detach();
}
