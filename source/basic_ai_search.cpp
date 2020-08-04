#include "/UFF/IC/Estimação de Estado/State_Estimator/State_Estimator_Otimizado/headers/basic_ai_search.h"
//johnsus


basic_ai_search::basic_ai_search()
{
}

//basic_ai_search(unsigned int x, const char* CK_PARAM_FILE, criticality&)
//{
//	TYPE_SEARCH = x;
//}

basic_ai_search::basic_ai_search(const char* CK_PARAM_FILE, std::string PARAM_STR)
{
	get_search_type(CK_PARAM_FILE, PARAM_STR);
}

//basic_ai_search::basic_ai_search(unsigned int x, unsigned int KLIM, unsigned int KMIN, unsigned int KMAX, unsigned int DIM, std::string CRIT_TYPE, criticality&)
//{
//}


void basic_ai_search::get_search_type(const char* ck_param_file, std::string& param_str) {

	if (param_str.compare("bb_bfs") == 0) {  // caso a busca seja em largura

		PARAM_STR = "Em Largura"; // muda a variavel para o arquivo final 

		bfs_search search = bfs_search(ck_param_file, param_str); //construtor da busca em largura

		if (search.get_crit_type() == "munit")
		{
			search.agent_munit(); //caso seja a busca pela MU
		}
		else if (search.get_crit_type() == "measurement")
		{
			search.agent_measurement(); //caso seja  abusca pelas medidas
		}
	}
	else if (param_str.compare("bb_dfs") == 0) {  // caso a busca seja em profundidade

		PARAM_STR = "Em Profundidade";  // muda a variavel para o arquivo final 

		dfs_search search = dfs_search(ck_param_file, param_str); //construtor da busca em profundidade

		if (search.get_crit_type() == "munit")
		{
			search.agent_munit(); //caso seja a busca pela MU
		}
		else if (search.get_crit_type() == "measurement")
		{
			search.agent_measurement(); //caso seja a busca pela medidas
		}
	}
	//else if (param_str.compare("bb_ids") == 0) {  // caso a busca seja em profundidade

	//	PARAM_STR = "Em Profundidade Iterativa";  // muda a variavel para o arquivo final 

	//	ids_search search = ids_search(ck_param_file, param_str); //construtor da busca em profundidade

	//	if (search.get_crit_type() == "munit")
	//	{
	//		search.agent_munit(); //caso seja a busca pela MU
	//	}
	//	else if (search.get_crit_type() == "measurement")
	//	{
	//		search.agent_measurement(); //caso seja a busca pela medidas
	//	}
	//}
	else if (param_str.compare("bb_a*s") == 0) {  // caso a busca seja em profundidade

		PARAM_STR = "A*";  // muda a variavel para o arquivo final 

		a_search search = a_search(ck_param_file, param_str); //construtor da busca em profundidade

		if (search.get_crit_type() == "munit")
		{
			search.agent_munit(); //caso seja a busca pela MU
		}
		else if (search.get_crit_type() == "measurement")
		{
			search.agent_measurement(); //caso seja a busca pela medidas
		}
	}
}

//std::string basic_ai_search::get_type_search() {
//		return PARAM_STR;
//	}
