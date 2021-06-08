#pragma once
#include "ck_search.h"
#include "bfs_search.h"
#include "dfs_search.h"
#include "ids_search.h"
#include "a_search.h"
#include "a_search_fast.h"
#include "uc_search.h"
#include<unordered_map>
#include<vector>





class basic_ai_search {
protected:
	//ck_search mu_file;
	std::string PARAM_STR;
public:
	//methods
	basic_ai_search();
	~basic_ai_search() {};
	//basic_ai_search(unsigned int, unsigned int KLIM, unsigned int KMIN, unsigned int KMAX, unsigned int DIM, std::string CRIT_TYPE, criticality&);
	//basic_ai_search(const char*, criticality&);
	//basic_ai_search(unsigned int, const char*, std::string);
	basic_ai_search(const char*, std::string);
	//void agent_measurement();
	//void agent_munit();
	void get_search_type(const char*, std::string&);
	std::string get_type_search();
	//std::string hashkey_2(std::vector<unsigned int>);*/
	//::vector<unsigned int> conv_measures(std::vector<unsigned int>, unsigned int);
	//unsigned int test_ck_4(std::vector<unsigned int>);
	//unsigned int test_ck_munit(std::vector<unsigned int>);
	//std::vector<unsigned int> get_measures() { return measures; }
	//void set_dim(std::vector<unsigned int>& MEASURES) { measures = MEASURES; };
	//void save_state();;
	//void temporary_report();
	//void printavetor(std::vector<unsigned int>);
};