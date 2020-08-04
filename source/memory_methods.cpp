#include"/UFF/IC/Estimação de Estado/State_Estimator/State_Estimator_Otimizado/headers/memory_methods.h"




std::string hashkey(std::vector<unsigned int> x) {
	std::string aux;
	for (unsigned int i = 0; i < x.size(); i++)
	{
		aux.push_back(x[i]);
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

