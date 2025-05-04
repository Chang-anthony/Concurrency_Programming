#ifndef _THREADOWNER_H_
#define _THREADOWNER_H_

#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <numeric>
#include <iterator>


void some_function();
void some_other_function();
void use_jointhread();
void use_vector();
void param_function(int i);
void use_parallel_acc();

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
    if (!length) {
        return init;    //⇽-- - ①
    }
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
        for (auto& entry : threads){
            entry.join();    //⇽-- - ⑩
        }
    return std::accumulate(results.begin(), results.end(), init);    //⇽-- - ⑪
}


class joining_thread {

std::thread _t;

public:
    joining_thread() noexcept = default;
    
    template<typename Callable, typename... Args>
    explicit joining_thread(Callable&& func, Args&&... args) {
        _t = std::thread(std::forward<Callable>(func), std::forward<Args>(args)...);
    }

    explicit joining_thread(std::thread  t) noexcept: _t(std::move(t)){}

    joining_thread(joining_thread&& other) noexcept: _t(std::move(other._t)){}

    joining_thread& operator=(joining_thread&& other) noexcept {
        if (joinable()) {
            _t.join();
        }
        _t = std::move(other._t);
        return *this;
    }

    // joining_thread& operator=(joining_thread other) noexcept
    // {
    //     //如果当前线程可汇合，则汇合等待线程完成再赋值
    //     if (joinable()) {
    //         join();
    //     }
    //     _t = std::move(other._t);
    //     return *this;
    // }

    ~joining_thread() noexcept {
        if (joinable()) {
            _t.join();
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


    // joining_thread(const joining_thread&) = delete;
    // joining_thread& operator=(const joining_thread&) = delete;

};



#endif /* _THREADOWNER_H_ */
