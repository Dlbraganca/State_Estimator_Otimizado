#include "/UFF/IC/Estimação de Estado/State_Estimator/State_Estimator_Otimizado/headers/bfs_search.h"


bfs_search::bfs_search()
{
}

bfs_search::bfs_search(const char*, criticality&)
{
}

bfs_search::bfs_search(const char* CK_PARAM_FILE, std::string PARAM_STR) : ck_search(CK_PARAM_FILE)
{
}

bfs_search::bfs_search(unsigned int KLIM, unsigned int KMIN, unsigned int KMAX, unsigned int DIM, std::string CRIT_TYPE, criticality&)
{
}

void bfs_search::agent_measurement() {

	std::string hash_key;
	std::vector<unsigned int> aux(critical_data.get_meas_location().size(), 0);
	std::vector<unsigned int> next_state;
	clock_t tbegin, tend, tfreememoryBegin, tmaxFreememory = 100000;
	unsigned int convergenceValue;

	tbegin = clock(); // get  begin time
	tfreememoryBegin = clock();

		std::stack <std::vector<unsigned int>> priority;
		priority.push(aux);
		while (!priority.empty())
		{
			next_state = priority.top();
			priority.pop();
			no_of_visited_solutions++;
			hash_key = hashkey(next_state);
			//printavetor(next_state);
			if (visited_states.find(hash_key) != visited_states.end())
			{
				convergenceValue = visited_states.find(hash_key)->second.convergence;
				if (convergenceValue == 3 || convergenceValue == 1)
				{
					visited_states.find(hash_key)->second.convergence = 1;
					lopt.push(next_state);
				}
				else
				{
					visited_states.find(hash_key)->second.convergence = 0;
					for (int i = next_state.size() - 1; i >= 0; i--)
					{
						aux = next_state;
						if (aux[i] == 0)
						{
							aux[i] = 1;
							hash_key = hashkey(aux);
							if (visited_states.find(hash_key) == visited_states.end())
							{
								if (test_ck_4(aux) == 0)
								{
									priority.push(aux);
								}
							}
							else
							{
								convergenceValue = visited_states.find(hash_key)->second.convergence;
								if (convergenceValue == 2)
								{
									priority.push(aux);
								}
							}
						}
						else { break; }
					}
				}
			}
			else if (critical_data.measurement_criticality(next_state) == 0)
			{
				for (int i = next_state.size() - 1; i >= 0; i--)
				{
					aux = next_state;
					if (aux[i] == 0)
					{
						aux[i] = 1;
						hash_key = hashkey(aux);
						if (visited_states.find(hash_key) == visited_states.end())
						{
							if (test_ck_4(aux) == 0)
							{
								priority.push(aux);
							}
						}
						else
						{
							convergenceValue = visited_states.find(hash_key)->second.convergence;
							if (convergenceValue == 2)
							{
								priority.push(aux);
							}
						}
					}
					else { break; }
				}
			}
			else
			{
				state x;
				x.positions = next_state;
				x.convergence = 1;
				hash_key = hashkey(next_state);
				visited_states.emplace(hash_key, x);
				lopt.push(next_state);
			}
			if (difftime(clock(), tfreememoryBegin) > tmaxFreememory) {
				std::cout << "Clearing memory...";
				free_memory(next_state); // save state
				std::cout << "done!!!" << std::endl;
				//std::cout << "Saving states...";
				//temporary_report(); // saving states
				//std::cout << "done!!!" << std::endl;
				tfreememoryBegin = clock();
			}
		
	}

	//time(&tend);// get total time
	elapsed_time = difftime(tend = clock(), tbegin);
	report();
}

void bfs_search::agent_munit() {

	std::string hash_key;
	std::vector<unsigned int> aux(critical_data.get_mu_location().size(), 0);
	std::vector<unsigned int> next_state;
	time_t tbegin, tend;

	tbegin = clock();
	if (TYPE_SEARCH == 1) // width search
	{
		std::queue <std::vector<unsigned int>> priority;
		priority.push(aux);
		while (priority.empty() == false)
		{
			no_of_visited_solutions++;
			next_state = priority.front();
			priority.pop();

			if (critical_data.munit_criticality(next_state) == 0)
			{
				for (int i = aux.size() - 1; i >= 0; i--)
				{
					aux = next_state;
					if (aux[i] == 0)
					{
						aux[i] = 1;
						hash_key = hashkey(aux);
						if (visited_states.find(hash_key) == visited_states.end())
						{
							if (test_ck_munit(aux) == 0)
							{
								//visited_states[hash_key] = true;
								priority.push(aux);
							}
						}
					}
					else { break; }
				}
			}
			else
			{
				lopt.push(next_state);
			}
		}
	}
	
	elapsed_time = difftime(tend = clock(), tbegin);
	report();
}


unsigned int bfs_search::test_ck_4(std::vector<unsigned int> x) {

	std::vector<unsigned int> aux;
	std::string hash_key;
	unsigned teste = 0;
	unsigned convergenceValue;


	for (unsigned int i = 0; i < x.size(); i++)
	{
		state atualState;
		aux = x;
		if (aux[i] == 1)
		{
			//count++;
			aux[i] = 0;
			hash_key = hashkey(aux);
			if (visited_states.find(hash_key) == visited_states.end())
			{
				if (critical_data.measurement_criticality(aux) == 1) //teste de observabilidade
				{
					atualState.convergence = 3;
					atualState.positions = aux;
					visited_states.emplace(hash_key, atualState);
					return 1;
				}
				else
				{
					atualState.convergence = 2;
					atualState.positions = aux;
					visited_states.emplace(hash_key, atualState);
				}
			}
			else {
				convergenceValue = visited_states.find(hash_key)->second.convergence;
				if (convergenceValue == 1 || convergenceValue == 3)
				{
					return 1;
				}
			}
		}
	}
	return 0;
}

unsigned int bfs_search::test_ck_munit(std::vector<unsigned int> x) {

	std::vector<unsigned int> aux;
	std::string hash_key;
	unsigned count = 0;
	unsigned kmax = ck_search::get_kmax();


	for (unsigned int i = 0; i < x.size(); i++)
	{
		if (x[i] == 1)
		{
			count++;
		}
		if (count > kmax)
		{
			return 1;
		}
	}

	for (unsigned int i = 0; i < x.size(); i++)
	{
		aux = x;
		if (aux[i] == 1)
		{
			//count++;
			aux[i] = 0;
			if (critical_data.munit_criticality(aux) == 1)
			{
				return 1;
			}
		}
	}
	return 0;
}

//
//void bfs_search::save_state() {
//
//	std::ofstream active_subsets_file("active_subsets.temp", std::ios::out | std::ios::trunc);
//	std::ofstream critical_list_file("critical_list.temp", std::ios::out | std::ios::trunc);
//	std::ofstream temp_report_file("temp_report.temp", std::ios::out | std::ios::trunc);
//	std::vector<unsigned int> T_temp;
//	std::queue<std::vector<unsigned int>> Asp_buff = not_visited;
//	std::stack<std::vector<unsigned int>>  lopt_buff = result;
//
//	// lista de tuplas criticas
//	if (critical_list_file.is_open()) { // impressao das tuplas criticas
//		while (!lopt_buff.empty()) {
//			T_temp = lopt_buff.top(); // get stack top element		
//			lopt_buff.pop(); // pop stack
//			for (std::vector<unsigned int>::iterator it = T_temp.begin(); it != T_temp.end(); ++it) {
//				critical_list_file << *it << " ";
//			}
//			critical_list_file << std::endl;
//		}
//		critical_list_file << 9999;// successful writing
//		critical_list_file.close();
//	}
//	else
//	{
//		std::cout << "Unable to print the temporary critical tuples file" << std::endl;
//	}
//	// lista de conjuntos ativos
//	if (active_subsets_file.is_open()) { // impressao do conjunto de soluções não visitadas
//		while (!Asp_buff.empty()) {
//			T_temp = Asp_buff.front(); //access first element of the list
//			Asp_buff.pop(); //delete this element		
//			for (std::vector<unsigned int>::iterator it = T_temp.begin(); it != T_temp.end(); ++it) {
//				active_subsets_file << *it << " ";
//			}
//			active_subsets_file << std::endl;
//		}
//		active_subsets_file << 9999;// successful writing
//		active_subsets_file.close();
//	}
//	else
//	{
//		std::cout << "Unable to print the temporary active subsets list file" << std::endl;
//	}
//	// parametros de busca
//	if (temp_report_file.is_open()) { // impressao parametros da busca
//
//		temp_report_file << klim << std::endl;
//		temp_report_file << kmin << std::endl;
//		temp_report_file << kmax << std::endl;
//		temp_report_file << dim << std::endl;
//		temp_report_file << crit_type << std::endl;
//		temp_report_file << no_of_visited_solutions << std::endl;
//		temp_report_file << elapsed_time << std::endl;
//		temp_report_file << 9999;// successful writing
//		temp_report_file.close();
//	}
//	else
//	{
//		std::cout << "Unable to print the temporary report file" << std::endl;
//	}
//};

void bfs_search::report() {

	std::ofstream critfile, report_file;
	std::vector<unsigned int> tuple;
	int result;

	critfile.open("critical_tuples.txt", std::ios::trunc);
	report_file.open("BB_report.txt");
	if (report_file.is_open()) { // impressao parametros do problema
		report_file << "Parametros do problema \n" << std::endl;
		report_file << "Dimensao:" << dim << std::endl;
		report_file << "k-limite:" << klim << std::endl;
		report_file << "k-min:" << kmin << std::endl;
		report_file << "k-max:" << kmax << std::endl;
		report_file << "Criticalidade:" << crit_type << std::endl;
		report_file << "Tipo de busca:" << param_str << std::endl;
		report_file << "no. solucoes visitadas:" << no_of_visited_solutions << std::endl;
		report_file << "tempo decorrido:" << elapsed_time << std::endl;
		report_file << "memória utilizada:" << get_memory() << std::endl;
	}
	if (critfile.is_open()) { // impressao das tuplas criticas
		while (!lopt.empty()) {
			tuple = lopt.top(); // get stack top element		
			lopt.pop(); // pop stack
			for (std::vector<unsigned int>::iterator it = tuple.begin(); it != tuple.end(); ++it) {
				critfile << *it << " ";
			}
			critfile << std::endl;
		}
		critfile.close();
		std::cout << "critical tuples file successfully generated !!!!" << std::endl;
	}
	else
	{
		std::cout << "Unable to print the critical tuples" << std::endl;
	}
	result = remove("active_subsets.temp");// limpeza dos arquivos temporarios
	result = remove("critical_list.temp");// limpeza dos arquivos temporarios
	result = remove("temp_report.temp");// limpeza dos arquivos temporarios
};

void bfs_search::printavetor(std::vector<unsigned int> x) {
	for (unsigned int i = 0; i < x.size(); i++)
	{
		std::cout << x[i];
	}
	std::cout << std::endl;
}

void bfs_search::free_memory(std::vector<unsigned int> nextState) {
	unsigned int oneCounter = 0;
	unsigned int aux = 0;
	//std::vector<unsigned int> endVector(nextState.size(),0);
	//for (unsigned int i = 0; i < nextState.size(); i++)
	//{
	//	if (nextState[i] == 1)
	//	{
	//		oneCounter++;
	//	}
	//}
	////oneCounter = oneCounter - 1;
	//for (int i = endVector.size() - 1; i >= 0; i--)
	//{
	//	endVector[i] = 1;
	//	aux++;
	//	if (aux == oneCounter)
	//	{
	//		break;
	//	}
	//}
	visited_states.erase(visited_states.begin(), visited_states.find(hashkey(nextState)));
}
