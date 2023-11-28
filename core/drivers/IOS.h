#pragma once

#include <memory>
#include <string>
#include <functional>

typedef void (*mainloop_cb)(void *data);

enum TaskPriority { NORMAL, HIGH };

struct ITimer {
    virtual ~ITimer() = default;

    virtual void start(uint32_t timeoutMs, const std::function<void()> &callback) = 0;
    virtual void stop() = 0;
};

struct IMutex {
    virtual ~IMutex() = default;

    virtual void lock(){};
    virtual bool tryLock() = 0;
    virtual void unlock(){};
};

struct ICondition {
    virtual ~ICondition() = default;

    virtual void notifyOne() = 0;
    virtual void notifyAll() = 0;
    virtual void wait() = 0;
};

struct ITask {
    enum State { ACTIVE, DEAD, KILL };

    virtual ~ITask() = default;

    virtual void start(std::function<void(ITask *t)> task) = 0;
    virtual void sleep(uint32_t miliseconds) = 0;
    virtual State getState() const = 0;
    virtual bool isRunning() const = 0;
    virtual void stop() = 0;
    virtual void join() = 0;
    virtual void noCancelable() = 0;
};

struct IOS {
    virtual ~IOS() = default;

    virtual std::shared_ptr<ITimer> createPeriodicTimer() = 0;
    virtual std::shared_ptr<ITimer> createOneShotTimer() = 0;
    virtual std::shared_ptr<IMutex> createMutex() = 0;
    virtual std::shared_ptr<ICondition> createCondition() = 0;
    virtual std::shared_ptr<ITask> createTask(TaskPriority priority = TaskPriority::NORMAL) = 0;
    virtual std::string getOptionOrDefault(const std::string &option, const std::string &defaultValue) = 0;

    virtual void reset(bool hardReset) = 0;
    virtual void sleep(uint32_t miliseconds) = 0;
    virtual timespec getCurrentTime() = 0;

    virtual int mainLoop() = 0;
    virtual int quit() = 0;
    virtual int addToMainLoop(int fd, mainloop_cb callback, void *data) = 0;
    virtual int enqueue(std::function<void()> callback) = 0;
    virtual uint32_t getSerialNumber() = 0;
};

template <typename T> struct LockGuard final {
    LockGuard(T &mutex) : mutex{mutex} {
        this->mutex.lock();
    }
    ~LockGuard() {
        this->mutex.unlock();
    }

  private:
    T &mutex{};
};
