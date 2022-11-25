// test_queue.cpp
#include <thread>
#include <mutex>
#include <iostream>
#include <queue>
#include <assert.h>    

#include "thread_safe_queue.hpp"

ThreadSafeQueue<int> safeQueue;
std::queue<int> unsafeQueue;

void pushToQueue()
{
	for (int i = 0; i < 10; i++) {
		safeQueue.push(i);
		//unsafeQueue.push(i);
	}
}

void popFromQueue()
{
	int popped_value = -1;
	for (int i = 0; i < 5; i++) {
		safeQueue.wait_and_pop(popped_value);

		/*
		while (unsafeQueue.empty()) {}

		popped_value = unsafeQueue.front();
		std::this_thread::sleep_for(std::chrono::nanoseconds(10000));
		unsafeQueue.pop();
		*/

		std::cout << popped_value;
	}
}

int main()
{
	std::thread t1(pushToQueue);
	std::thread t2(popFromQueue);
	std::thread t3(popFromQueue);

	t1.join();
	t2.join();
	t3.join();

	std::cout << std::endl;
	
	return EXIT_SUCCESS;
}
