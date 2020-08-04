//#include "/UFF/IC/Estimação de Estado/State_Estimator/State_Estimator_Otimizado/headers/ids_search.h"
//
//
//ids_search::ids_search()
//{
//}
//
////ids_search::ids_search(const char*, criticality&)
////{
////}
//
//ids_search::ids_search(const char* CK_PARAM_FILE, std::string PARAM_STR) : ck_search(CK_PARAM_FILE)
//{
//}
//
////ids_search::ids_search(unsigned int KLIM, unsigned int KMIN, unsigned int KMAX, unsigned int DIM, std::string CRIT_TYPE, criticality&)
////{
////}
//
//void ids_search::agent_measurement() {
//
//	std::string hash_key;
//	std::vector<unsigned int> next_state;
//	time_t tbegin = clock(), tend, tfreememoryBegin = clock(), tmaxFreememory = 100000;
//	unsigned int convergenceValue, currentDeepth;
//	std::queue <std::vector<unsigned int>> priorityLIFO;
//
//	interativeMax = 0; // definir o valor maximo da interacao
//	while (interativeMax <= ck_search::get_kmax())
//	{
//		std::vector<unsigned int> aux(critical_data.get_meas_location().size(), 0);
//		priorityLIFO.push(aux);
//		interativeMax++;
//		visited_states.clear();
//		std::cout << "reset-----------------" << std::endl;
//
//		while (!priorityLIFO.empty())
//		{
//			next_state = priorityLIFO.front();
//			priorityLIFO.pop();
//			no_of_visited_solutions++;
//			hash_key = hashkey(next_state);
//			printavetor(next_state);
//
//			if (visited_states.find(hash_key) != visited_states.end()) // verifica se a ck ja foi visitada - caso sim continua
//			{
//				convergenceValue = visited_states.find(hash_key)->second.convergence; // procura o valor de convergencia da ck 
//				if (convergenceValue == 3 || convergenceValue == 1) //casos em que as ck nao sao observaveis
//				{
//					visited_states.find(hash_key)->second.convergence = 1; // define a convergencia como 1 (estado nao observavel ja visitado)
//					lopt.push(next_state); // insere na pilha de resultados
//				}
//				else // caso em que as ck sao observaveis 
//				{
//					visited_states.find(hash_key)->second.convergence = 0; //define como valor 0 a observabilidade (estado observavel ja visitado)
//					for (int i = next_state.size() - 1; i >= 0; i--) // expansao dos estados
//					{
//						aux = next_state;
//						if (aux[i] == 0)
//						{
//							aux[i] = 1;
//							hash_key = hashkey(aux);
//							if (visited_states.find(hash_key) == visited_states.end())
//							{
//								currentDeepth = count_deepth(aux); // avalia a profunidade do no expandido
//								if (test_ck_4(aux) == 0 && count_deepth(aux) <= interativeMax)
//								{
//									priorityLIFO.push(aux);
//								}
//							}
//							else
//							{
//								convergenceValue = visited_states.find(hash_key)->second.convergence;
//								currentDeepth = visited_states.find(hash_key)->second.depth;
//								if (convergenceValue == 2 && currentDeepth <= interativeMax)
//								{
//									priorityLIFO.push(aux); // se for menor ou igual a profundidade maxima coloca na pilha LIFO
//								}
//							}
//						}
//						else { break; }
//					}
//				}
//			}
//			else if (critical_data.measurement_criticality(next_state) == 0)
//			{
//				for (int i = next_state.size() - 1; i >= 0; i--)
//				{
//					aux = next_state;
//					if (aux[i] == 0)
//					{
//						aux[i] = 1;
//						hash_key = hashkey(aux);
//						if (visited_states.find(hash_key) == visited_states.end())
//						{
//							currentDeepth = count_deepth(aux); // avalia a profunidade do no expandido
//							if (test_ck_4(aux) == 0 && count_deepth(aux) <= interativeMax)
//							{
//								priorityLIFO.push(aux);
//
//							}
//						}
//						else
//						{
//							convergenceValue = visited_states.find(hash_key)->second.convergence;
//							currentDeepth = visited_states.find(hash_key)->second.depth;
//							if (convergenceValue == 2)
//							{
//								if (currentDeepth < interativeMax)
//								{
//									priorityLIFO.push(aux);
//								}
//								else
//								{
//									priorityFIFO.push(aux);
//									countDeepth++;
//								}
//							}
//						}
//					}
//					else { break; }
//				}
//			}
//			else
//			{
//				state x;
//				x.positions = next_state;
//				x.convergence = 1;
//				x.depth = count_deepth(next_state);
//				hash_key = hashkey(next_state);
//				visited_states.emplace(hash_key, x);
//				lopt.push(next_state);
//			}
//			if (difftime(clock(), tfreememoryBegin) > tmaxFreememory) {
//				std::cout << "Clearing memory...";
//				//free_memory(next_state); // save state
//				visited_states.clear();
//				//while (!lopt.empty())
//				//{
//				//	std::cout << "teste" << std::endl; 
//				//	lopt.pop();
//				//}
//				std::cout << "done!!!" << std::endl;
//				//std::cout << "Saving states...";
//				//temporary_report(); // saving states
//				//std::cout << "done!!!" << std::endl;
//				tfreememoryBegin = clock();
//			}
//		}
//	}
//	//time(&tend);// get total time
//	elapsed_time = difftime(clock(), tbegin);
//	report();
//}
//
//void ids_search::agent_munit() {
//
//	std::string hash_key;
//	std::vector<unsigned int> aux(critical_data.get_mu_location().size(), 0);
//	std::vector<unsigned int> next_state;
//	time_t tbegin, tend;
//
//	tbegin = clock();
//	{
//		std::stack<std::vector<unsigned int>> priorityLIFO;
//
//		priorityLIFO.push(aux);
//		while (priorityLIFO.empty() == false)
//		{
//			no_of_visited_solutions++;
//			next_state = priorityLIFO.top();
//			priorityLIFO.pop();
//
//			if (critical_data.munit_criticality(next_state) == 0)
//			{
//				for (int i = aux.size() - 1; i >= 0; i--)
//				{
//					aux = next_state;
//					if (aux[i] == 0)
//					{
//						aux[i] = 1;
//						hash_key = hashkey(aux);
//
//						if (visited_states.find(hash_key) == visited_states.end())
//						{
//							if (test_ck_munit(aux) == 0)
//							{
//								//[hash_key] = true;
//								priorityLIFO.push(aux);
//							}
//						}
//					}
//					else { break; }
//				}
//			}
//			else
//			{
//				lopt.push(next_state);
//			}
//		}
//	}
//
//	elapsed_time = difftime(clock(), tbegin);
//	report();
//}
//
//unsigned int ids_search::test_ck_4(std::vector<unsigned int> x) {
//
//	std::vector<unsigned int> aux;
//	std::string hash_key;
//	unsigned teste = 0;
//	unsigned convergenceValue;
//
//
//	for (unsigned int i = 0; i < x.size(); i++)
//	{
//		state atualState;
//		aux = x;
//		if (aux[i] == 1)
//		{
//			//count++;
//			aux[i] = 0;
//			hash_key = hashkey(aux);
//			if (visited_states.find(hash_key) == visited_states.end())
//			{
//				if (critical_data.measurement_criticality(aux) == 1) //teste de observabilidade
//				{
//					atualState.convergence = 3;
//					atualState.positions = aux;
//					visited_states.emplace(hash_key, atualState);
//					return 1;
//				}
//				else
//				{
//					atualState.convergence = 2;
//					atualState.positions = aux;
//					visited_states.emplace(hash_key, atualState);
//				}
//			}
//			else {
//				convergenceValue = visited_states.find(hash_key)->second.convergence;
//				if (convergenceValue == 1 || convergenceValue == 3)
//				{
//					return 1;
//				}
//			}
//		}
//	}
//	return 0;
//}
//
//unsigned int ids_search::test_ck_munit(std::vector<unsigned int> x) {
//
//	std::vector<unsigned int> aux;
//	std::string hash_key;
//	unsigned count = 0;
//	unsigned kmax = ck_search::get_kmax();
//
//
//	for (unsigned int i = 0; i < x.size(); i++)
//	{
//		if (x[i] == 1)
//		{
//			count++;
//		}
//		if (count > kmax)
//		{
//			return 1;
//		}
//	}
//
//	for (unsigned int i = 0; i < x.size(); i++)
//	{
//		aux = x;
//		if (aux[i] == 1)
//		{
//			//count++;
//			aux[i] = 0;
//			if (critical_data.munit_criticality(aux) == 1)
//			{
//				return 1;
//			}
//		}
//	}
//	return 0;
//}
//
//unsigned int ids_search::count_deepth(std::vector<unsigned int> x){
//	unsigned int count = 0;
//	for (size_t i = 0; i < x.size(); i++)
//	{
//		if (x[i] == 1)
//		{
//			count++;
//		}
//	}
//	return count;
//}
//
//
//void ids_search::report() {
//
//	std::ofstream critfile, report_file;
//	std::vector<unsigned int> tuple;
//	int result;
//
//	critfile.open("critical_tuples.txt", std::ios::trunc);
//	report_file.open("BB_report.txt");
//	if (report_file.is_open()) { // impressao parametros do problema
//		report_file << "Parametros do problema \n" << std::endl;
//		report_file << "Dimensao:" << dim << std::endl;
//		report_file << "k-limite:" << klim << std::endl;
//		report_file << "k-min:" << kmin << std::endl;
//		report_file << "k-max:" << kmax << std::endl;
//		report_file << "Criticalidade:" << crit_type << std::endl;
//		report_file << "Tipo de busca:" << param_str << std::endl;
//		report_file << "no. solucoes visitadas:" << no_of_visited_solutions << std::endl;
//		report_file << "tempo decorrido:" << elapsed_time << std::endl;
//		report_file << "memória utilizada:" << get_memory() << std::endl;
//	}
//	if (critfile.is_open()) { // impressao das tuplas criticas
//		while (!lopt.empty()) {
//			tuple = lopt.top(); // get stack top element		
//			lopt.pop(); // pop stack
//			for (std::vector<unsigned int>::iterator it = tuple.begin(); it != tuple.end(); ++it) {
//				critfile << *it << " ";
//			}
//			critfile << std::endl;
//		}
//		critfile.close();
//		std::cout << "critical tuples file successfully generated !!!!" << std::endl;
//	}
//	else
//	{
//		std::cout << "Unable to print the critical tuples" << std::endl;
//	}
//	result = remove("active_subsets.temp");// limpeza dos arquivos temporarios
//	result = remove("critical_list.temp");// limpeza dos arquivos temporarios
//	result = remove("temp_report.temp");// limpeza dos arquivos temporarios
//};
//
//void ids_search::printavetor(std::vector<unsigned int> x) {
//	for (unsigned int i = 0; i < x.size(); i++)
//	{
//		std::cout << x[i];
//	}
//	std::cout << std::endl;
//}