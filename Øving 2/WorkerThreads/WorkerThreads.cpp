// WorkerThreads.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include <iostream>
#include <list>
#include <thread>
#include <vector>
#include <condition_variable>
#include <mutex>
#include <functional>


using namespace std;

class Worker {

    vector<thread> threads;
    list<function<void()>> tasks;
    mutex wait_mutex;
    condition_variable cv;
    int noOfThreads;
    bool stopThread = false;


public:
    explicit Worker(int threads) {
        noOfThreads = threads;
    }


    void post(const function<void()> task) {
        {
            lock_guard<mutex> lock(wait_mutex);
            tasks.emplace_back(task);
        }

        cv.notify_one();
    }

	void post_timemout(const function<void()> &task, const int timeoutMs)
    {
        cout << "post_timeout has been called!\n";
        thread timeout_thread([this, task, timeoutMs]
        {
                this_thread::sleep_for(chrono::milliseconds(timeoutMs));
                {
                    lock_guard<mutex> lock(wait_mutex);
                    tasks.emplace_back(task);
                }
                cv.notify_one();
        });
        timeout_thread.detach();
        
        cout << "post_timeout has finished\n";
        
    }

    void start() {
        for (int i = 0; i < noOfThreads; i++) {
            threads.emplace_back([this] {
                while (true)
                {
                    function<void()> task;
                    {

                        unique_lock<mutex> lock(wait_mutex);

                        while (!stopThread && tasks.empty()) {
                            cv.wait(lock);
                        }

                        if (tasks.empty())
                        {
                            return;
                        }

                        task = tasks.front(); // Copy task for later use
                        tasks.pop_front(); // Remove task from list
                    }

                    task_timeout(25);
                    task(); // Run task outside of mutex lock

                }
            });
        }
    }

    void join() {
        for (auto& thread : threads)
            thread.join();
        stop();
    }

    void stop() {
        stopThread = true;
        cv.notify_all();
    }

    void task_timeout(int timeoutMs) {
        this_thread::sleep_for(chrono::milliseconds(timeoutMs));
    }

};

int main() {
	
    Worker worker_threads(4);
    Worker event_loop(1);
	
    worker_threads.start(); // Create 4 internal threads
    event_loop.start(); // Create 1 internal thread

    worker_threads.post_timemout([] {
        // Task A, ran directly on a worker thread
        cout << "Hello from worker task A\n";
        }, 2000);
    worker_threads.post([] {
        // Task B, ran directly on a worker thread
        cout << "Hello from worker task B\n";
        });
    event_loop.post([] {
        // Task C, posted on the event loop
        cout << "Hello from event loop task C\n";
        });
    event_loop.post([] {

        cout << "Hello from task D, Event loop\n";
        // Task D, posted on the event loop
        // Will run after task C
        // Might run in parallel with task A and B
        });
    /*
    event_loop.post_timemout([] {

        cout << "Hello from task E, this task has been delayed by post timeout!\n";
        // Task D, posted on the event loop
        // Will run after task C
        // Might run in parallel with task A and B
        }, 2000);*/



    this_thread::sleep_for(5s);
    worker_threads.stop();
    event_loop.stop();

    worker_threads.join();
    event_loop.join();
}