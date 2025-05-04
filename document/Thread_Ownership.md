# Thread OwnerShip
Thread ownership refers to the concept of associating a specific thread with a particular resource or task. It ensures that only the owning thread can access or modify the resource, preventing race conditions and ensuring thread safety in concurrent programming.

### Thread Owner

We previously introduced that threads can either run in the background by being detached or have their parent thread wait for their completion.  
However, each thread should have ownership, meaning it should be managed by a specific variable.

```cpp

    void some_function() {
    }
    std::thread t1(some_function);
```

In C++, `std::thread` does not allow copy construction or copy assignment. Instead, thread ownership can only be transferred using move semantics or by returning a local variable. This ensures that only one `std::thread` object manages a particular thread at any given time.

Other C++ types with similar ownership semantics include `std::mutex`, `std::ifstream`, and `std::unique_ptr`.

For example, the following demonstrates how thread ownership can be transferred:

```cpp
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
//t1 bind some_function
std::thread t1(some_function); 
//2  transfer t1 manage thread to t2, after transfer t1 invalid.
std::thread t2 =  std::move(t1);
//3 t1 can bind other thread , run some_other_function
t1 = std::thread(some_other_function);
//4  create the new Thread t3
std::thread t3;
//5  transfer t2 to t3
t3 = std::move(t2);
//6  transfer t3 to t1
t1 = std::move(t3);
std::this_thread::sleep_for(std::chrono::seconds(2000));
```

By using `std::move`, the ownership of the thread is explicitly transferred, ensuring proper resource management and avoiding undefined behavior.

The above code will crash due to step 6. The main function is set to sleep for 2000 seconds to prevent the main thread from exiting, which would otherwise cause a crash if threads are not detached or joined. This was demonstrated earlier. However, even with the main thread sleeping, the program still crashes, indicating that step 6 is the cause of the crash.

In the code, the thread managed by `t2` is transferred to `t3`. Later, the thread managed by `t3` is transferred to `t1`. At this point, `t1` is managing a thread running `some_function`. The crash occurs in step 6 because the thread managed by `t3` is transferred to `t1`, which is already managing a thread running `some_other_function`.

This leads to the conclusion that transferring the ownership of a thread to a variable that is already managing another thread will trigger the thread's `terminate` function, causing a crash. 

### Thread Transfer use std::unique_ptr
Like `std::unique_ptr`, we can return a local `std::thread` variable from within a function. For example:

```cpp
std::thread create_thread() {
    return std::thread([] {
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    });
}

int main() {
    std::thread t = create_thread();
    t.join(); // Ensure the thread is joined before exiting
    return 0;
}
```

In this example, the `create_thread` function creates and returns a `std::thread` object. Ownership of the thread is transferred to the caller, ensuring proper resource management. The caller is responsible for managing the thread, such as joining or detaching it.

Because in C++, when returning a local variable, the compiler will first look for the copy constructor of the class. If the copy constructor is not available, it will use the move constructor of the class.

### Joining_thread

```cpp
class joining_thread {
    std::thread  _t;
public:
    joining_thread() noexcept = default;
    template<typename Callable, typename ...  Args>
    explicit  joining_thread(Callable&& func, Args&& ...args):
        t(std::forward<Callable>(func),  std::forward<Args>(args)...){}
    explicit joining_thread(std::thread  t) noexcept: _t(std::move(t)){}
    joining_thread(joining_thread&& other) noexcept: _t(std::move(other._t)){}

    joining_thread& operator=(joining_thread&& other) noexcept
    {
        //如果当前线程可汇合，则汇合等待线程完成再赋值
        if (joinable()) {
            join();
        }
        _t = std::move(other._t);
        return *this;
    }
    joining_thread& operator=(joining_thread other) noexcept
    {
        //如果当前线程可汇合，则汇合等待线程完成再赋值
        if (joinable()) {
            join();
        }
        _t = std::move(other._t);
        return *this;
    }
    ~joining_thread() noexcept {
        if (joinable()) {
            join();
        }
    }
    void swap(joining_thread& other) noexcept {
        _t.swap(other._t);
    }
    std::thread::id   get_id() const noexcept {
        return _t.get_id();
    }
    bool joinable() const noexcept {
        return _t.joinable();
    }
    void join() {
        _t.join();
    }
    void detach() {
        _t.detach();
    }
    std::thread& as_thread() noexcept {
        return _t;
    }
    const std::thread& as_thread() const noexcept {
        return _t;
    }
};

```

Using it is quite simple; we can directly construct a `joining_thread` object.

```cpp
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
ervice
}
```

### Container Storage (Vector ...)


When storing threads in a container like `std::vector`, using `push_back` will attempt to copy the `std::thread` object, which will result in a compilation error because `std::thread` does not have a copy constructor. 

To avoid this issue, you can use the `emplace_back` method, which constructs the thread directly in place using the provided arguments. This approach bypasses the need for a copy constructor and is particularly useful when initializing multiple threads in a container, such as in an `IOServicePool` or `IOThreadPool` implementation.

For example:

```cpp
#include <iostream>
#include <thread>
#include <vector>

void thread_function(int id) {
    std::cout << "Thread " << id << " is running.\n";
}

int main() {
    std::vector<std::thread> thread_pool;

    // Use emplace_back to construct threads directly in the vector
    for (int i = 0; i < 5; ++i) {
        thread_pool.emplace_back(thread_function, i);
    }

    // Join all threads
    for (auto& t : thread_pool) {
        if (t.joinable()) {
            t.join();
        }
    }

    return 0;
}
```

In this example, `emplace_back` is used to construct threads directly in the `std::vector`, avoiding the need for a copy constructor. Each thread is initialized with the required arguments, and the program ensures proper resource management by joining all threads before exiting.

### Select the number of runs

### Parallel Sum Using `std::thread::hardware_concurrency`

The `std::thread::hardware_concurrency()` function provides an estimate of the number of threads that can run concurrently on the system. Using this, we can implement a parallel computation to calculate the sum of all elements in a container.

Here is an example:

```cpp
template <typename Iterator, typename T>
struct accumulate_block
{
    void operator()(Iterator first, Iterator last, T& result)
    {
        result = std::accumulate(first, last, result);    //⇽-- - ⑨
    }
};



template<typename Iterator, typename T>
T parallel_accumulate(Iterator first, Iterator last, T init)
{
    unsigned long const length = std::distance(first, last);
    if (!length)
        return init;    //⇽-- - ①
        unsigned long const min_per_thread = 25;
    unsigned long const max_threads =
        (length + min_per_thread - 1) / min_per_thread;    //⇽-- - ②
        unsigned long const hardware_threads =
        std::thread::hardware_concurrency();
    unsigned long const num_threads =
        std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);    //⇽-- - ③
        unsigned long const block_size = length / num_threads;    //⇽-- - ④
        std::vector<T> results(num_threads);
    std::vector<std::thread>  threads(num_threads - 1);   // ⇽-- - ⑤
        Iterator block_start = first;
    for (unsigned long i = 0; i < (num_threads - 1); ++i)
    {
        Iterator block_end = block_start;
        std::advance(block_end, block_size);    //⇽-- - ⑥
            threads[i] = std::thread(//⇽-- - ⑦
                accumulate_block<Iterator, T>(),
                block_start, block_end, std::ref(results[i]));
        block_start = block_end;    //⇽-- - ⑧
    }
    accumulate_block<Iterator, T>()(
        block_start, last, results[num_threads - 1]);    //⇽-- - ⑨
        for (auto& entry : threads)
            entry.join();    //⇽-- - ⑩
            return std::accumulate(results.begin(), results.end(), init);    //⇽-- - ⑪
}
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
```

### Explanation
At point 1, the code checks if the container has zero elements, and if so, it returns the initial value.

At point 2, the maximum number of threads is calculated based on the assumption that each thread will process 25 elements.

However, using `std::thread::hardware_concurrency`, the number of CPU cores is determined. At point 3, the minimum value between the calculated maximum threads and the number of CPU cores is chosen to avoid excessive thread creation, which could lead to overhead from context switching.

At point 4, the block size (step length) is calculated. This step length is used to move the iterator and divide the workload among threads.

At point 5, a vector of size `num_threads - 1` is initialized to store the threads, as the main thread also participates in the computation, reducing the need for one additional thread.

At point 6, the iterator is advanced by the block size, and at point 7, a new thread is created to process the block. At point 8, the starting position is updated for the next block.

At point 9, the main thread processes the remaining block of data.

At point 10, all threads are joined to ensure they complete execution before proceeding.

Finally, at point 11, the results from all threads are combined using `std::accumulate` to compute the final result.

### Identifying Threads 

/**
 * Thread identification refers to obtaining the thread ID, which can be used to determine 
 * whether two threads are the same by comparing their thread IDs.
 * 
 * For example, when a thread is started, its thread ID can be retrieved using the `get_id()` 
 * method of the thread variable.
 */

```cpp
std::thread t([](){
    std::cout << "thread start" << std::endl;
});
t.get_id();

```

However, if we want to distinguish threads within the thread's execution function or determine which are main threads or child threads, we can use the following approach:

```cpp
#include <iostream>
#include <thread>

void thread_function() {
    if (std::this_thread::get_id() == std::thread::id()) {
        std::cout << "This is the main thread.\n";
    } else {
        std::cout << "This is a child thread with ID: " << std::this_thread::get_id() << "\n";
    }
}

int main() {
    std::cout << "Main thread ID: " << std::this_thread::get_id() << "\n";

    std::thread t1(thread_function);
    std::thread t2(thread_function);

    t1.join();
    t2.join();

    return 0;
}
```

In this example, the `std::this_thread::get_id()` function is used to retrieve the ID of the currently executing thread. By comparing the thread ID, we can determine whether the thread is the main thread or a child thread.
