#include "multi_threaded_count.hpp"

#define MAPNAME phmap::parallel_flat_hash_map
#define NMSP phmap

#define EXTRAARGS , NMSP::priv::hash_default_hash<std::string>, \
                            NMSP::priv::hash_default_eq<std::string>, \
                            std::allocator<std::pair<const std::string, int>>, 4, std::mutex

namespace ThreadedWordCounter
{
	using HashT = MAPNAME<std::string, int EXTRAARGS>;
	HashT wordCount;
	std::atomic<int> threadsRunning = 0;

	void CountStrings(std::string const& stringLine)
	{
		// If the string is empty, or it contains just a null terminator, (or, well, a non-alphabetic letter, as that's not a word)
		if (stringLine.empty() || stringLine.size() == 1 && isalpha(stringLine[0]) == 0)
		{
			threadsRunning -= 1;
			return;
		}

		// convert to stringstream so we can use operators to extract words
		std::stringstream ss(stringLine);

		// Next word variable
		std::string nextWord;

		// Count each instance. Is thread-safe for writing, even if multiple threads are writing at the same time.
		while (ss >> nextWord)
		{
			// Lowercase all of it, as it is case-insensitive.
			std::transform(nextWord.begin(), nextWord.end(), nextWord.begin(), ::tolower);

			// Count it.
			wordCount[nextWord] += 1;
		}

		threadsRunning -= 1;
		return;
	}

}