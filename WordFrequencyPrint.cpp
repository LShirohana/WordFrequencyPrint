#include "multi_threaded_count.hpp"
#include <thread>
#include <chrono>

int main()
{
    std::string filePath = "wordlist.txt";

	// Open the word list.
	std::ifstream file(filePath, std::ios::binary|std::ios::ate);

	if (file.is_open())
	{
        if (!file)
            throw std::runtime_error(filePath);

        // String object for storing each line.
        std::string nextLine = "";
        
        // To the beginning of the file we go.
        file.seekg(0, std::ios::beg);

        // Create a thread object for each line, and then put the thread object into a vector
        while (std::getline(file, nextLine))
        {
            // Increment Atomic int for thread count.
            ThreadedWordCounter::threadsRunning += 1;

            // Start a thread, sending that line as an argument to CountStrings
            std::thread thread_object(ThreadedWordCounter::CountStrings, nextLine);

            // Detach each thread object, because we are not gonna wait until it's joinable. It's better to use an atomic thread counter as the overhead is incredibly small.
            thread_object.detach();
        }
        
        // Wait for threads to finish.
        while (ThreadedWordCounter::threadsRunning > 0)
        {
            // Use std::this_thread rather than sleep(), because sleep requires windows.h, and it's good to make your code portable!
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }

        // Threads are finished! Time to sort our map and output the correct results!
        std::vector<std::pair<std::string, int>> Elements(ThreadedWordCounter::wordCount.begin(), ThreadedWordCounter::wordCount.end());
        std::sort(Elements.begin(), Elements.end(),
            [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) -> bool
            {
                return a.second > b.second;
            }
        );

        for (auto it = Elements.begin(); it != Elements.end(); it++)
        {

            std::cout << "Word: "
                << it->first    // string (key)
                << " -> Ocurrences: "
                << it->second   // string's value 
                << std::endl;
        }

        std::cout << "\nProgram exiting, finished successfully" << std::endl;
        return 0;
	}
    else
    {
        std::cout << "\nProgram exiting, wordlist.txt not found! (Please make sure its in the same directory as the executable!)" << std::endl;
        return 0;
    }
}