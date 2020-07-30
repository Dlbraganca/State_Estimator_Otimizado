#include "windows.h"
#include "psapi.h"
#include<vector>
#include<unordered_map>


struct state {
	std::vector<unsigned int> positions;
	unsigned int convergence;
	unsigned int depth;
	unsigned int heuristic;
};

SIZE_T get_memory();
std::string hashkey(std::vector<unsigned int> x);
