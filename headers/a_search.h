
#include "ck_search.h"
#include<unordered_map>
#include<vector>
#include "windows.h"
#include "psapi.h"


class a_search : public ck_search {

protected:
	//boost::unordered::unordered_map<std::string, state> visited_states;
	std::unordered_map<std::string, state> visited_states;
	ck_search mu_file;
	unsigned int TYPE_SEARCH;
	unsigned int no_of_visited_solutions = 0;
	const char* CK_PARAM_FILE;
	std::stack<std::vector<unsigned int>> lopt;
	std::string param_str;
	//std::queue <std::vector<unsigned int>> priority;
	std::vector<std::vector<unsigned int>> priority;

public:
	//methods
	a_search();
	~a_search() {};
	//a_search(unsigned int KLIM, unsigned int KMIN, unsigned int KMAX, unsigned int DIM, std::string CRIT_TYPE, criticality&);
	//a_search(const char*, criticality&);
	a_search(const char*, std::string);
	void agent_measurement();
	void agent_munit();
	//std::vector<unsigned int> conv_measures(std::vector<unsigned int>, unsigned int);
private:
	unsigned int test_ck_4(std::vector<unsigned int>);
	unsigned int test_ck_munit(std::vector<unsigned int>);
	void report();
	void save_state();
	bool comparar_heuristicas(state, state);
	void ordena_heuristica(std::vector<std::vector<unsigned int>>&);
};

