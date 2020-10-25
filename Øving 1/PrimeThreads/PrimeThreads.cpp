// PrimeThreads.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>

using namespace std;

vector<int> primes;

bool isPrime(int inNumber)
{
	
	if (inNumber <= 1)
	{
		return false;
	}

	for (int i = 2; i < int(sqrt(inNumber)); ++i)
	{
		if (inNumber % i == 0)
		{
			return false;
		}		
	}
	
	return true;
}

int main()
{
	int threadAmount = 101;
	int startNumber = 0;
	int endNumber = 100;
	
	vector<thread> threads;
	

	for (int i = startNumber; i < threadAmount + startNumber; i++)
	{
		threads.emplace_back([i, threadAmount, endNumber]
			{
				
				for (int j = i; j < endNumber; j += threadAmount)
				{
					if (isPrime(j))
					{
						//std::cout << "about to emplace_back";
						primes.emplace_back(j);
					}
				}
				//cout << "Output from thread " << i << endl;
			});
	}

	for (auto &thread : threads)
	{
		thread.join();
	}

	sort(primes.begin(), primes.end());

	for (auto &prime : primes)
	{
		cout << prime << " ";
	}
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu