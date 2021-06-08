#include "/UFF/IC/Estimação de Estado/State_Estimator/State_Estimator_Otimizado/headers/ids_search.h"


ids_search::ids_search()
{
}

//ids_search::ids_search(const char*, criticality&)
//{
//}

ids_search::ids_search(const char* CK_PARAM_FILE, std::string PARAM_STR) : ck_search(CK_PARAM_FILE)
{
}

//ids_search::ids_search(unsigned int KLIM, unsigned int KMIN, unsigned int KMAX, unsigned int DIM, std::string CRIT_TYPE, criticality&)
//{
//}

void ids_search::agent_measurement() {

	std::string hash_key;
	state next_state;
	std::vector<unsigned int> aux_vector(critical_data.get_meas_location().size(), 0);
	time_t tbegin = clock(), tend, tfreememoryBegin = clock(), tmaxFreememory = 100000;
	unsigned int convergenceValue, currentDeepth;
	std::stack <state> priorityLIFO;
	bool searchCountinue = true;

	interativeMax = 0; // definir o valor maximo da interacao
	while (searchCountinue)
	{
		std::vector<unsigned int> aux_vector(critical_data.get_meas_location().size(), 0);
		searchCountinue = false;
		state aux(aux_vector, "bb_ids");
		priorityLIFO.push(aux);
		interativeMax++;
		//visited_states.clear();
		//std::cout << "reset-----------------\nProxima iteracao " << interativeMax << std::endl;

		while (!priorityLIFO.empty())
		{
			next_state = priorityLIFO.top();
			priorityLIFO.pop();
			no_of_visited_solutions++;
			//printavetor(next_state.get_cklist());

			hash_key = hashkey(next_state.get_cklist());
			if (visited_states.find(hash_key) != visited_states.end()) // verifica se a k ja foi visitada - caso sim continua
			{
				convergenceValue = visited_states.find(hash_key)->second;  //retorna o valor de convergencia e armazena em uma variavel
				if (convergenceValue == 3) //casos em que as k nao sao observaveis
				{
					visited_states.find(hash_key)->second = 1;  // muda o valor de convergencia para "1"
					lopt.push(next_state.get_cklist());  // armazena na pilha de resultados
				}
				else if (convergenceValue == 2 || convergenceValue == 0) // caso em que as k sao observaveis 
				{
					visited_states.find(hash_key)->second = 0; // caso encontre na tabela e é observavel - seta o valor para 0 (0-> visitado e observavel)
					for (int i = next_state.get_cklist().size() - 1; i >= 0; i--) // expansao dos estados
					{
						aux_vector = next_state.get_cklist(); // estado a ser expandido armazenado em uma nova variavel
						if (aux_vector[i] == 0)
						{
							aux_vector[i] = 1;
							hash_key = hashkey(aux_vector);
							//if (visited_states.find(hash_key) == visited_states.end())
							//{//caso nao encontrado
								if (test_ck_4(aux_vector) == 0){
									if (count_deepth(aux_vector) <= interativeMax) // avalia a profundidade do no expandido
									{
										state aux_state(aux_vector, "bb_ids");
										priorityLIFO.push(aux_state);
									}
									else
									{
										searchCountinue = true;
									}
							//}
							//else
							//{// caso encontre na tabela hash
							//	convergenceValue = visited_states.find(hash_key)->second; // verifica a convergencia armazenada
							//	if (convergenceValue == 2 || convergenceValue == 0)
							//	{
							//		if (count_deepth(aux_vector) <= interativeMax) // avalia a profundidade do no expandido
							//		{
							//			state aux_state(aux_vector, "bb_ids");
							//			priorityLIFO.push(aux_state); // se for menor ou igual a profundidade maxima coloca na pilha LIFO
							//		}
							//		else
							//		{
							//			searchCountinue = true;
							//		}
							//	}
							}
						}
						else { break; }
					}
				}
			}
			else if (critical_data.measurement_criticality(next_state.get_cklist()) == 0)
			{
				for (int i = next_state.get_cklist().size() - 1; i >= 0; i--)
				{
					aux_vector = next_state.get_cklist(); // estado a ser expandido armazenado em uma nova variavel
					if (aux_vector[i] == 0)
					{
						aux_vector[i] = 1;
						hash_key = hashkey(aux_vector); //calcula a hash do estado criado
						/*if (visited_states.find(hash_key) == visited_states.end())
						{*/
							if (test_ck_4(aux_vector) == 0){
								if (count_deepth(aux_vector) <= interativeMax) // avalia a profundidade do no expandido
								{
									state aux_state(aux_vector, "bb_ids");
									priorityLIFO.push(aux_state);
								}
								else
								{
									searchCountinue = true;
								}
						//}
						//else
						//{// caso encontre na tabela hash
						//	convergenceValue = visited_states.find(hash_key)->second;
						//	if (convergenceValue == 2 || convergenceValue == 0)
						//	{
						//		if (count_deepth(aux_vector) <= interativeMax) // avalia a profundidade do no expandido
						//		{
						//			priorityLIFO.push(aux);
						//		}
						//		else
						//		{
						//			searchCountinue = true;
						//		}
						//	}
							}
					}
					else { break; }
				}
			}
			else
			{ //caso nao esteja na tabela hash e nao seja observavel
				//x.positions = next_state;
				hash_key = hashkey(next_state.get_cklist());
				visited_states.emplace(hash_key, 1);;
				lopt.push(next_state.get_cklist());  // coloca na pilha de ck criticas
			}
			//if (visited_states.size() > 10000000000000000)
			//{
			//	visited_states.clear();
			//}
			//if (difftime(clock(), tfreememoryBegin) > tmaxFreememory) {
			//	std::cout << "Clearing memory...";
			//	free_memory(next_state.get_cklist()); // save state
			//	std::cout << "done!!!" << std::endl;
			//	//std::cout << "Saving states...";
			//	//temporary_report(); // saving states
			//	//std::cout << "done!!!" << std::endl;
			//	tfreememoryBegin = clock();
			//}

		}
	}
	//time(&tend);// get total time
	elapsed_time = difftime(tend = clock(), tbegin);
	report();
}


void ids_search::agent_munit() {
	std::string hash_key;
	state next_state;
	std::vector<unsigned int> aux_vector(critical_data.get_mu_location().size(), 0);
	time_t tbegin = clock(), tend, tfreememoryBegin = clock(), tmaxFreememory = 100000;
	unsigned int convergenceValue, currentDeepth;
	std::stack <state> priorityLIFO;
	bool searchCountinue = true;

	while (searchCountinue)
	{
		std::vector<unsigned int> aux_vector(critical_data.get_mu_location().size(), 0);
		searchCountinue = false;
		state aux(aux_vector, "bb_ids");
		priorityLIFO.push(aux);
		interativeMax++;
		//visited_states.clear();
		//std::cout << "reset-----------------\nProxima iteracao" << interativeMax << std::endl;
			while (!priorityLIFO.empty())
			{
				next_state = priorityLIFO.top();
				priorityLIFO.pop();
				no_of_visited_solutions++;
				//printavetor(next_state.get_cklist());

				hash_key = hashkey(next_state.get_cklist());
				if (visited_states.find(hash_key) != visited_states.end()) // verifica se a k ja foi visitada - caso sim continua
				{
					convergenceValue = visited_states.find(hash_key)->second;  //retorna o valor de convergencia e armazena em uma variavel
					if (convergenceValue == 3) //casos em que as k nao sao observaveis
					{
						visited_states.find(hash_key)->second = 1;  // muda o valor de convergencia para "1"
						lopt.push(next_state.get_cklist());  // armazena na pilha de resultados
					}
					else if (convergenceValue == 2 || convergenceValue == 0) // caso em que as k sao observaveis 
					{
						visited_states.find(hash_key)->second = 0; // caso encontre na tabela e é observavel - seta o valor para 0 (0-> visitado e observavel)
						for (int i = next_state.get_cklist().size() - 1; i >= 0; i--) // expansao dos estados
						{
							aux_vector = next_state.get_cklist(); // estado a ser expandido armazenado em uma nova variavel
							if (aux_vector[i] == 0)
							{
								aux_vector[i] = 1;
								hash_key = hashkey(aux_vector);
								//if (visited_states.find(hash_key) == visited_states.end())
								//{//caso nao encontrado
								if (test_ck_munit(aux_vector) == 0) {
									if (count_deepth(aux_vector) <= interativeMax) // avalia a profundidade do no expandido
									{
										state aux_state(aux_vector, "bb_ids");
										priorityLIFO.push(aux_state);
									}
									else
									{
										searchCountinue = true;
									}
									//}
									//else
									//{// caso encontre na tabela hash
									//	convergenceValue = visited_states.find(hash_key)->second; // verifica a convergencia armazenada
									//	if (convergenceValue == 2 || convergenceValue == 0)
									//	{
									//		if (count_deepth(aux_vector) <= interativeMax) // avalia a profundidade do no expandido
									//		{
									//			state aux_state(aux_vector, "bb_ids");
									//			priorityLIFO.push(aux_state); // se for menor ou igual a profundidade maxima coloca na pilha LIFO
									//		}
									//		else
									//		{
									//			searchCountinue = true;
									//		}
									//	}
								}
							}
							else { break; }
						}
					}
				}
				else if (critical_data.munit_criticality(next_state.get_cklist()) == 0)
				{
					for (int i = next_state.get_cklist().size() - 1; i >= 0; i--)
					{
						aux_vector = next_state.get_cklist(); // estado a ser expandido armazenado em uma nova variavel
						if (aux_vector[i] == 0)
						{
							aux_vector[i] = 1;
							hash_key = hashkey(aux_vector); //calcula a hash do estado criado
							/*if (visited_states.find(hash_key) == visited_states.end())
							{*/
							if (test_ck_munit(aux_vector) == 0) {
								if (count_deepth(aux_vector) <= interativeMax) // avalia a profundidade do no expandido
								{
									state aux_state(aux_vector, "bb_ids");
									priorityLIFO.push(aux_state);
								}
								else
								{
									searchCountinue = true;
								}
								//}
								//else
								//{// caso encontre na tabela hash
								//	convergenceValue = visited_states.find(hash_key)->second;
								//	if (convergenceValue == 2 || convergenceValue == 0)
								//	{
								//		if (count_deepth(aux_vector) <= interativeMax) // avalia a profundidade do no expandido
								//		{
								//			priorityLIFO.push(aux);
								//		}
								//		else
								//		{
								//			searchCountinue = true;
								//		}
								//	}
							}
						}
						else { break; }
					}
				}
				else
				{ //caso nao esteja na tabela hash e nao seja observavel
					//x.positions = next_state;
					hash_key = hashkey(next_state.get_cklist());
					visited_states.emplace(hash_key, 1);;
					lopt.push(next_state.get_cklist());  // coloca na pilha de ck criticas
				}
				//if (visited_states.size() > 0)
				//{
				//	visited_states.clear();
				//}
				//if (difftime(clock(), tfreememoryBegin) > tmaxFreememory) {
				//	std::cout << "Clearing memory...";
				//	free_memory(next_state.get_cklist()); // save state
				//	std::cout << "done!!!" << std::endl;
				//	//std::cout << "Saving states...";
				//	//temporary_report(); // saving states
				//	//std::cout << "done!!!" << std::endl;
				//	tfreememoryBegin = clock();
				//}
			}
		}
		//time(&tend);// get total time
		elapsed_time = difftime(tend = clock(), tbegin);
		report();
	}

unsigned int ids_search::test_ck_4(std::vector<unsigned int> x) {

	std::vector<unsigned int> aux;
	std::string hash_key;
	unsigned teste = 0;
	unsigned convergenceValue;


	for (unsigned int i = 0; i < x.size(); i++)
	{
		aux = x;
		if (aux[i] == 1)
		{
			//count++;
			aux[i] = 0;
			hash_key = hashkey(aux);
			if (visited_states.find(hash_key) == visited_states.end())
			{
				if (critical_data.measurement_criticality(aux) == 1) //teste de observabilidade
				{//nao observavel
					//atualState.positions = aux;
					visited_states.emplace(hash_key, 3);
					return 1;
				}
				else
				{//observavel
					//atualState.positions = aux;
					visited_states.emplace(hash_key, 2);
				}
			}
			else {
				convergenceValue = visited_states.find(hash_key)->second;
				if (convergenceValue == 1 || convergenceValue == 3)
				{
					return 1;
				}
			}
		}
	}
	return 0;
}

unsigned int ids_search::test_ck_munit(std::vector<unsigned int> x) {

	std::vector<unsigned int> aux;
	std::string hash_key;
	unsigned teste = 0;
	unsigned convergenceValue;


	for (unsigned int i = 0; i < x.size(); i++)
	{
		aux = x;
		if (aux[i] == 1)
		{
			//count++;
			aux[i] = 0;
			hash_key = hashkey(aux);
			if (visited_states.find(hash_key) == visited_states.end())
			{
				if (critical_data.munit_criticality(aux) == 1) //teste de observabilidade
				{//nao observavel
					//atualState.positions = aux;
					visited_states.emplace(hash_key, 3);
					return 1;
				}
				else
				{//observavel
					//atualState.positions = aux;
					visited_states.emplace(hash_key, 2);
				}
			}
			else {
				convergenceValue = visited_states.find(hash_key)->second;
				if (convergenceValue == 1 || convergenceValue == 3)
				{
					return 1;
				}
			}
		}
	}
	return 0;
}

unsigned int ids_search::count_deepth(std::vector<unsigned int> x){
	unsigned int count = 0;
	for (size_t i = 0; i < x.size(); i++)
	{
		if (x[i] == 1)
		{
			count++;
		}
	}
	return count;
}


void ids_search::report() {

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

void ids_search::printavetor(std::vector<unsigned int> x) {
	for (unsigned int i = 0; i < x.size(); i++)
	{
		std::cout << x[i];
	}
	std::cout << std::endl;
}