
#include <iostream>
#include <string>
#include <fstream>
#include "basic_ai_search.h"

void main() {

	basic_ai_search search;

	std::ifstream parameters_file("ck_directives.txt", std::ifstream::in); //construtor 
	std::string param_str;

	if (parameters_file.is_open()) {
		std::cout << "Parameter file has been successfully opened. Reading search method ...";
		getline(parameters_file, param_str);
		parameters_file.close();
		std::cout << "Done !!!" << std::endl;
		search.set_param_str(param_str);
	}
	else
	{
		std::cout << "Unable to open the search parameters file" << std::endl;
	}
	if (param_str.compare("bb_bfs") == 0) {
		unsigned int x = 1;
		search = basic_ai_search(x, "ck_directives.txt", param_str);
		if (search.get_crit_type() == "munit")
		{
			search.agent_munit();
		}
		else if (search.get_crit_type() == "measurement")
		{
			search.agent_measurement();
		}
	}
	else if (param_str.compare("bb_dfs") == 0) {
		unsigned int x = 2;
		search = basic_ai_search(x, "ck_directives.txt", param_str);
		if (search.get_crit_type() == "munit")
		{
			search.agent_munit();
		}
		else if (search.get_crit_type() == "measurement")
		{
			search.agent_measurement();
		}
	}
}
