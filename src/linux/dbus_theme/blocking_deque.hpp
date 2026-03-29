#ifndef F2C1A2AC_2D80_484B_85A5_4F72EA6C1AA7
#define F2C1A2AC_2D80_484B_85A5_4F72EA6C1AA7

#include <mutex>
#include <condition_variable>
#include <deque>
#include <optional>
#include <atomic>

template<typename T>
class BlockingDeque {
  public:
    void push(T value) {
        std::lock_guard<std::mutex> lock(mutex_);
        deque_.push_back(value);
        cond_var_.notify_one();
    }

    std::optional<T> pop() {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_var_.wait(lock, [this] { return !deque_.empty() || stopFlag.load(); });
        if(stopFlag.load()) return std::nullopt;
        T value = deque_.front();
        deque_.pop_front();
        return value;
    }

    bool empty() {
        std::lock_guard<std::mutex> lock(mutex_);
        return deque_.empty();
    }

    void stop() {
        stopFlag.store(true);
        cond_var_.notify_all();
    }

  private:
    std::atomic<bool> stopFlag{false};
    std::deque<T> deque_;
    std::mutex mutex_;
    std::condition_variable cond_var_;
};

#endif /* F2C1A2AC_2D80_484B_85A5_4F72EA6C1AA7 */
