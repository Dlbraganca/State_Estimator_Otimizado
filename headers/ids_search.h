//#include "ck_search.h"
//#include<unordered_map>
//#include<vector>
//#include "windows.h"
//#include "psapi.h"
//
//class ids_search : public ck_search {
//
//protected:
//	std::unordered_map<std::string, state> visited_states;
//	ck_search mu_file;
//	unsigned int TYPE_SEARCH;
//	unsigned int no_of_visited_solutions = 0;
//	const char* CK_PARAM_FILE;
//	unsigned int interativeMax, interativeValue, countDeepth = 0;
//	std::stack<std::vector<unsigned int>> lopt;
//	std::stack<std::vector<unsigned int>> priorityLIFO;
//	std::queue<std::vector<unsigned int>> priorityFIFO;
//	std::string param_str;
//
//public:
//	//methods
//	ids_search();
//	~ids_search() {};
//	//ids_search(unsigned int KLIM, unsigned int KMIN, unsigned int KMAX, unsigned int DIM, std::string CRIT_TYPE, criticality&);
//	//ids_search(const char*, criticality&);
//	ids_search(const char*, std::string);
//	void agent_measurement();
//	void agent_munit();
//	//std::vector<unsigned int> conv_measures(std::vector<unsigned int>, unsigned int);
//private:
//	unsigned int test_ck_4(std::vector<unsigned int>);
//	unsigned int test_ck_munit(std::vector<unsigned int>);
//	unsigned int count_deepth(std::vector<unsigned int>);
//	void report();
//	void printavetor(std::vector<unsigned int>);
//};