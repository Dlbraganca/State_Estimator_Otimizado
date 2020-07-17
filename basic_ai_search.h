#pragma once
#pragma once
#include "ck_search.h"

#include<unordered_map>
#include<vector>
#include "windows.h"
#include "psapi.h"


struct state {
	std::vector<unsigned int> positions;
	bool convergence;
};

class basic_ai_search : public ck_search {
protected:
	ck_search mu_file;
	unsigned int TYPE_SEARCH;
	unsigned int no_of_visited_solutions = 0;
	const char* CK_PARAM_FILE;
	std::unordered_map<std::string, state> visited_states;
	std::map<std::string, bool> type_state;
	std::queue<std::vector<unsigned int>>  not_visited;
	std::map<std::string, std::vector<unsigned int>> critical_tuples;
	std::vector<unsigned int>measures;
	std::stack <std::vector<unsigned int>> result;
	std::stack<std::vector<unsigned int>> lopt, lopt_buff;
	std::string param_str;

public:
	//methods
	basic_ai_search();
	~basic_ai_search() {};
	basic_ai_search(unsigned int, unsigned int KLIM, unsigned int KMIN, unsigned int KMAX, unsigned int DIM, std::string CRIT_TYPE, criticality&);
	basic_ai_search(unsigned int, const char*, criticality&);
	basic_ai_search(unsigned int, const char*, std::string);
	void agent_measurement();
	void agent_munit();
	std::string hashkey_2(std::vector<unsigned int>);
	std::vector<unsigned int> conv_measures(std::vector<unsigned int>, unsigned int);
	unsigned int test_ck_4(std::vector<unsigned int>);
	unsigned int test_ck_munit(std::vector<unsigned int>);
	std::vector<unsigned int> get_measures() { return measures; }
	void set_dim(std::vector<unsigned int>& MEASURES) { measures = MEASURES; };
	void save_state();
	void report();
	void printavetor(std::vector<unsigned int>);
	SIZE_T get_memory() {

		PROCESS_MEMORY_COUNTERS_EX pmc;

		GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
		SIZE_T virtualMemUsedByMe = pmc.PrivateUsage;
		SIZE_T physMemUsedByMe = pmc.WorkingSetSize;
		return physMemUsedByMe;
	};

	void set_param_str(std::string& PARAM_STR) {
		if (PARAM_STR == "bb_bfs")
		{
			param_str = "Em largura";
		}
		else if (PARAM_STR == "bb_dfs")
		{
			param_str = "Em profundidade";
		}
	};
};