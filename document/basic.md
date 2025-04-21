# Basic Thread
This article mainly introduces the basic control of threads, including thread initiation, waiting, how to wait under exceptional conditions, and basic operations such as background running.
---
### Thread Create && Wait
```cpp

void thread_work(std::string str)
{
    std::cout << "Thread: " << str << std::endl;
}

int main() {
    std::string hellostr = "Hello, World!";
    
    // 1. Initialize and start a thread using ()
    std::thread t1(thread_work1, hellostr);
    
    // Wait for the thread to finish
    t1.join();
    
    return 0;
}
```
- When we start a thread, it may not execute immediately. If a thread is started in a local scope or in the main function, it is possible that the child thread will be reclaimed before it runs. When reclaimed, the thread's destructor will be called, executing the terminate operation. To prevent the `child thread from being destructed due to the main thread exiting or the local scope ending`, we can use `join()` to make the `main thread wait for the child thread to start running`. After the child thread finishes running, the main thread will continue to run. 
---
### Functor as Parameter

In C++, a functor (or function object) is an object that can be called as if it were a function. This is achieved by overloading the `operator()`. Functors can be used as parameters to functions, including threads.

Here is an example of using a functor as a parameter to a thread:

```cpp
#include <iostream>
#include <thread>
#include <string>

// Define a functor
class ThreadFunctor {
public:
    void operator()(std::string str) {
        std::cout << "Thread: " << str << std::endl;
    }
};

int main() {
    std::string hellostr = "Hello, World!";
    
    // Initialize and start a thread using a functor
    ThreadFunctor functor;
    std::thread t1(functor, hellostr);
    
    // Wait for the thread to finish
    t1.join();
    
    return 0;
}
```
- If Use Below to call thread it will fault because the complier will will treat `t2` as a function object:
```cpp

    std::thread t2(ThreadFunctor());
    t2.join();
```
- Can understand for this `std::thread (*)(ThreadFunctor (*)())`
- Can rewrite by below code
```cpp
    //add one more()
    std::thread t2((ThreadFunctor()));
    t2.join();
    //or use {} init
    std::thread t3{ ThreadFunctor() };
    t3.join();
```
---
### Lambda Expressions

In C++, lambda expressions provide a concise way to define anonymous functions. They can be used as parameters to functions, including threads.

Here is an example of using a lambda expression to create and start a thread:

```cpp
#include <iostream>
#include <thread>
#include <string>

int main() {
    std::string hellostr = "Hello, World!";
    
    // Initialize and start a thread using a lambda expression
    std::thread t1([hellostr]() {
        std::cout << "Thread: " << hellostr << std::endl;
    });
    
    // Wait for the thread to finish
    t1.join();
    
    return 0;
}
```
---
### Detaching Threads
In C++, you can detach a thread from the main thread, `allowing it to run independently`. When a thread is detached, it will continue to run even after the main thread has finished. However, you need to ensure that the detached thread completes its execution properly to avoid any undefined behavior.

Here is an example of detaching a thread:

```cpp

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

void oops()
{
    int some_local_state = 0;
    func my_func(some_local_state);
    std::thread t(my_func);
    //Hidden danger: accessing local variables, local variables may be recycled when the } ends or when the main thread exits
    t.detach();
}

//main.cpp
oops();
//prevent main thread exit too quickly pause the thread，and let the child thread run detach
std::this_thread::sleep_for(std::chrono::seconds(1));

```
Above ex. have some hidden Danger. because the `some_local_state` is local variable. And `oops` func is running end  `some_local_state` maybe 
recycled. When the thread are detach running in background, is easy crashed.

There have some ways to aviod this problem
- use the smart pointer to pass the param, can check the variable will not be recycled when func end.
- pass the param on func param, but this need the param have copy and paste maybe will waste the space and efficieny
- change the thread running way to `join()`, can make sure the varibale recyceled the thread is alreay running down, but may be influence the running logic

---
## Exception Handling

In C++, it is important to handle exceptions properly when working with threads to ensure that resources are managed correctly and the program does not terminate unexpectedly.

Here is an example of handling exceptions in a thread:

```cpp
    void catch_exception() {
        int some_local_state = 0;
        func myfunc(some_local_state);
        std::thread  functhread{ myfunc };
        try {
            //do something maybe will crashed
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }catch (std::exception& e) {
            functhread.join();
            throw;
        }
        functhread.join();
    }
```

But code like above will look bloated, can also use below code, the can make sure the thread running down and call `deconstruct func` recycled the resource.

```cpp
    class thread_guard {
    private:
        std::thread& _t;
    public:
        explicit thread_guard(std::thread& t):_t(t){}
        ~thread_guard() {
            //join只能调用一次
            if (_t.joinable()) {
                _t.join();
            }
        }
        thread_guard(thread_guard const&) = delete;
        thread_guard& operator=(thread_guard const&) = delete;
    };

//how to use
void auto_guard() {
    int some_local_state = 0;
    func my_func(some_local_state);
    std::thread  t(my_func);
    thread_guard g(t);
    //do something
    std::cout << "auto guard finished " << std::endl;
}
auto_guard();
```
The `thread_guard` class is a `RAII (Resource Acquisition Is Initialization)` wrapper for a std::thread object. It ensures that the thread is properly joined when the thread_guard object goes out of scope, preventing potential issues with detached threads or unjoined threads.

---
Use implicit conversions with caution.

``` c
void danger_oops(int som_param) {
    char buffer[1024];
    sprintf(buffer, "%i", som_param);
    // Inside the thread, convert `char const*` to `std::string`
    // Pointer constant: `char * const` - The pointer itself cannot change
    // Constant pointer: `const char *` - The content being pointed to cannot change
    std::thread t(print_str, 3, buffer);
    t.detach();
    std::cout << "danger oops finished " << std::endl;
}
```
When we let the child thread detach, it means the main thread does not need to wait for the child thread to finish execution before continuing. However, there is a potential risk that the buffer used in the child thread might be reclaimed by memory.

Can use this type conversion

``` c
void safe_oops(int some_param) {
    char buffer[1024];
    sprintf(buffer, "%i", some_param);
    std::thread t(print_str, 3, std::string(buffer));
    t.detach();
}
```
That the thread new string type is `std::string` not the char implicit conversions

---
### Passing Functions by Reference

In C++, you can pass functions by reference to threads to avoid copying overhead and ensure the function operates on the original data. This is particularly useful when working with large objects or when you want to modify the original data.

Here is an example of passing a function by reference to a thread:

```cpp
void change_param(int& param) {
    param++;
}
void ref_oops(int some_param) {
    std::cout << "before change , param is " << some_param << std::endl;
    //Explicit conversion to reference is required.
    std::thread  t2(change_param, some_param);
    t2.join();
    std::cout << "after change , param is " << some_param << std::endl;
}

Because the param pause into the thread will change two the right value and save into thread.

void ref_oops(int some_param) {
    std::cout << "before change , param is " << some_param << std::endl;
    //Explicit conversion to reference is required.
    std::thread  t2(change_param, std::ref(some_param));
    t2.join();
    std::cout << "after change , param is " << some_param << std::endl;
}

Because cannot use the left value two reference the right value.
```

# lvalues and rvalues in C
What is an lvalue?
An lvalue (locator value) refers to an object that has an identifiable memory location.

It can appear on the left-hand side of an assignment.

You can take its address using the & operator.

Common lvalues: variables, array elements, dereferenced pointers (*ptr), etc.

```cpp
int x = 10;   // x is an lvalue
int *p = &x;  // *p is an lvalue, refers to x
```
🔸 What is an rvalue?
An rvalue is a value that does not have a persistent memory address.

It typically appears on the right-hand side of an assignment.

You cannot take the address of an rvalue (e.g., &10 is invalid).

Common rvalues: constants (e.g., 10), expressions (e.g., a + b), return values from functions (if not references or pointers), etc.

```cpp

int y = x + 1;  // x + 1 is an rvalue
```

🔄 Relationship in Assignment

```cpp
x = 5;      // x is an lvalue, 5 is an rvalue
p = &x;     // p is an lvalue, &x is an rvalue
*p = 20;    // *p is an lvalue, 20 is an rvalue
```
🧠 Special Cases
x++ and x-- are rvalues: even though they modify the variable, the result is a temporary value.

++x is an lvalue: the result is the updated variable itself, which can be addressed.





