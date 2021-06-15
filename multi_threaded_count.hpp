#pragma once

#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <map>
#include <string>
#include "parallel_hashmap/phmap.h"
#include <fstream>
#include <filesystem>

using phmap::parallel_flat_hash_map;

// Defines to define a multi-thread safe hashmap.
#define MAPNAME phmap::parallel_flat_hash_map
#define NMSP phmap
#define EXTRAARGS , NMSP::priv::hash_default_hash<std::string>, \
                            NMSP::priv::hash_default_eq<std::string>, \
                            std::allocator<std::pair<const std::string, int>>, 4, std::mutex

namespace ThreadedWordCounter
{
	extern std::atomic<int> threadsRunning;

	using HashT = MAPNAME<std::string, int EXTRAARGS>;
	extern HashT wordCount;

	// function for multi-threading occurence reading.
	void CountStrings(std::string const& stringLine);
}