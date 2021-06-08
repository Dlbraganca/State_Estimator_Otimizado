#include"../headers/memory_methods.h"




std::string hashkey(std::vector<unsigned int> x) {
	std::string aux;
	for (unsigned int i = 0; i < x.size(); i++)
	{
		if (x[i] == 1)
		{
			aux.push_back(1);
		}
		else
		{
			aux.push_back(2);
		}
	}
	return aux;
}

SIZE_T get_memory() {

	PROCESS_MEMORY_COUNTERS_EX pmc;

	GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
	SIZE_T virtualMemUsedByMe = pmc.PrivateUsage;
	SIZE_T physMemUsedByMe = pmc.WorkingSetSize;
	return physMemUsedByMe;
};

