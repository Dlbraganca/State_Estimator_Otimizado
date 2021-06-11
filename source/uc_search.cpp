#include "../headers/uc_search.h"


uc_search::uc_search()
{
}

uc_search::uc_search(const char*, criticality&)
{
}

uc_search::uc_search(const char* CK_PARAM_FILE, std::string critical_data) : ck_search(CK_PARAM_FILE)
{
	critical_data = critical_data;
}

uc_search::uc_search(unsigned int KLIM, unsigned int KMIN, unsigned int KMAX, unsigned int DIM, std::string CRIT_TYPE, criticality&)
{
}

//bool comparar_heuristicas_uc(state& a, state& b) {
//	return a.get_heuristic() > b.get_heuristic();
//}
//
//void ordena_heuristica_uc(std::vector<state>& x) {
//	std::sort(x.begin(), x.end(), &comparar_heuristicas_uc);
//}


state uc_search::fast_sort_uc() {
#undef max
	double bestDeepth = std::numeric_limits<double>::max(); //pegar a menor heuristica entao iniciamos com o maior valor
	unsigned int positionBest = 0;
	state bestState;
	bool foundTemporaryPile = false;
	bool foundPile = false;
	//std::cout << "heuristica da pilha : \n";
	for (unsigned int i = 0; i < pile.size(); i++) //avalia na pilha
	{
		//std::cout << pile[i].get_deepth() << std::endl;
		if (pile[i].get_deepth() < bestDeepth)
		{
			bestState = pile[i];
			bestDeepth = bestState.get_deepth();
			positionBest = i;
			foundPile = true;
		}
	}
	//std::cout << "heuristica da pilha temporaria: \n";
	for (unsigned int i = 0; i < temporaryPile.size(); i++)//avalia entre os novos estados
	{
		//std::cout << temporaryPile[i].get_deepth() << std::endl;
		if (temporaryPile[i].get_deepth() < bestDeepth)
		{
			bestState = temporaryPile[i];
			bestDeepth = bestState.get_deepth();
			positionBest = i;
			foundTemporaryPile = true;
		}
	}
	////TESTE HEURSITICA
	//std::cout << "pilha antiga: \n";
	//for (size_t i = 0; i < pile.size(); i++)
	//{
	//	std::cout << pile[i].get_deepth() << std::endl;
	//}
	//std::cout << "pilha nova: \n";
	//for (size_t i = 0; i < temporaryPile.size(); i++)
	//{
	//	std::cout << temporaryPile[i].get_deepth() << std::endl;
	//}
	//std::cout << "heuristica escolhida : " << bestState.get_deepth() << std::endl;
	////
	if (foundTemporaryPile) //se a melhor heurisitica foi encontrada na pilha temporaria
	{
		temporaryPile[positionBest].set_deepth(std::numeric_limits<double>::max());
	}
	else if (foundPile) //se a melhor heurisitica foi encontrada na pilha
	{
		pile[positionBest].set_deepth(std::numeric_limits<double>::max());//seta o valor desse estado com a maior heuristica possivel (busca-se a menor heuristica)
		freePosition.push_back(positionBest); // adiciona essa posicao no vetor com as posicoes livres
	}
	while (!temporaryPile.empty())
	{
		if (temporaryPile.back().get_deepth() < std::numeric_limits<double>::max())
		{
			if (!freePosition.empty())
			{
				unsigned int position = freePosition.back();
				freePosition.pop_back();
				pile[position] = temporaryPile.back();
				temporaryPile.pop_back();
			}
			else
			{
				pile.push_back(temporaryPile.back());
				temporaryPile.pop_back();
			}
		}
		else
		{
			temporaryPile.pop_back();
		}
	}
	if (bestState.get_cklist().empty()) //quando a pilha ta limpa so ira existir heuristicas com o valor maximo e por isso o melhor resultado sera o vazio
	{
		pile.clear();
	}
	return bestState;
}

void uc_search::agent_measurement() {

	std::string hash_key;
	std::vector<unsigned int> aux_vector(critical_data.get_meas_location().size(), 0);
	state next_state(aux_vector, critical_data, "uniform");
	clock_t tbegin, tend;
	unsigned int convergenceValue;
	std::vector <state> priority;

	tbegin = clock(); // get  begin time


	while (!next_state.get_cklist().empty())  //loop ate a pilha ficar vazia (nesse caso é cklist vazio pq a ordem quando nao tem mais estados retorna uma "state" com o ck_list vazio
	{
		//for (size_t i = 0; i < priority.size(); i++)
		//{
		//	std::cout << priority[i].get_heuristic() << std::endl;
		//}
		//std::cout << std::endl;
		//ordena_heuristica_uc(priority); //ordena a pilha 
		//for (size_t i = 0; i < priority.size(); i++)
		//{
		//	std::cout << priority[i].get_heuristic() << std::endl;
		//}
		//std::cout << std::endl;
		//next_state = priority.back(); //retira o primeiro da pilha e armazena em uma nova variavel
		//priority.pop_back(); // deleta o primeiro da pilha
		no_of_visited_solutions++; // adiciona em 1 o numero de solucoes visitadas
		hash_key = hashkey(next_state.get_cklist()); // calculo da funcao hash para o vetor
		if (visited_states.find(hash_key) != visited_states.end()) // caso o estado esteja na tabela hash
		{
			convergenceValue = visited_states.find(hash_key)->second;  //retorna o valor de convergencia e armazena em uma variavel
			if (convergenceValue == 3) // valores de convergencia (3-> nao observavel nao visitado) (1-> visitado e nao observavel)
			{
				visited_states.find(hash_key)->second = 1;  // muda o valor de convergencia para "1"
				lopt.push(next_state.get_cklist());  // armazena na pilha de resultados
			}
			else
			{
				visited_states.find(hash_key)->second = 0; // caso encontre na tabela e é observavel - seta o valor para 0 (0-> visitado e observavel)
				for (int i = next_state.get_cklist().size() - 1; i >= 0; i--) //expansao do estado
				{
					aux_vector = next_state.get_cklist(); // estado a ser expandido armazenado em uma nova variavel
					if (aux_vector[i] == 0)
					{
						aux_vector[i] = 1; //transforma 0 em 1
						hash_key = hashkey(aux_vector); //calcula a hash do estado criado
						if (visited_states.find(hash_key) == visited_states.end()) //procura na tabela hash
						{//caso nao encontrado
							if (test_ck_4(aux_vector) == 0) // verifica se contem uma tupla critica
							{//caso nao tenha tupla critica
								state aux_state(aux_vector, critical_data, "uniform");
								temporaryPile.push_back(aux_state); // armazena na fila de prioridade
							}
						}
						else
						{// caso encontre na tabela hash
							convergenceValue = visited_states.find(hash_key)->second; // verifica a convergencia armazenada
							if (convergenceValue == 2) // caso seja 2 (2-> observavel nao visitado)
							{
								state aux_state(aux_vector, critical_data, "uniform");
								temporaryPile.push_back(aux_state); // armazena na fila de prioridade
							}
							else if (convergenceValue == 3) // caso seja 3 (3-> nao observavel nao visitado)
							{
								if (test_ck_4(aux_vector) == 0) // verifica se contem uma tupla critica
								{//caso nao tenha tupla critica
									state aux_state(aux_vector, critical_data, "uniform");
									temporaryPile.push_back(aux_state); // armazena na fila de prioridade
								}
							}
						}
					}
					else { break; }
				}
			}
		}
		else if (critical_data.measurement_criticality(next_state.get_cklist()) == 0)
		{//caso nao esteja na tabela hash e seja observavel
			for (int i = next_state.get_cklist().size() - 1; i >= 0; i--)//expansao do estado
			{
				aux_vector = next_state.get_cklist(); // estado a ser expandido armazenado em uma nova variavel
				if (aux_vector[i] == 0)
				{
					aux_vector[i] = 1;
					hash_key = hashkey(aux_vector); //calcula a hash do estado criado
					if (visited_states.find(hash_key) == visited_states.end())
					{
						if (test_ck_4(aux_vector) == 0)
						{
							state aux_state(aux_vector, critical_data, "uniform");
							temporaryPile.push_back(aux_state); // armazena na fila de prioridad
						}
					}
					else
					{// caso encontre na tabela hash
						convergenceValue = visited_states.find(hash_key)->second; // verifica a convergencia armazenada
						if (convergenceValue == 2) // caso seja 2 (2-> observavel nao visitado)
						{
							state aux_state(aux_vector, critical_data, "uniform");
							temporaryPile.push_back(aux_state); // armazena na fila de prioridade
						}
						else if (convergenceValue == 3) // caso seja 3 (3-> nao observavel nao visitado)
						{
							if (test_ck_4(aux_vector) == 0) // verifica se contem uma tupla critica
							{//caso nao tenha tupla critica
								state aux_state(aux_vector, critical_data, "uniform");
								temporaryPile.push_back(aux_state); // armazena na fila de prioridade
							}
						}
					}
				}
				else { break; }
			}
		}
		else
		{ //caso nao esteja na tabela hash e nao seja observavel
			//x.positions = next_state;
			hash_key = hashkey(next_state.get_cklist());
			visited_states.emplace(hash_key, 1);
			lopt.push(next_state.get_cklist());  // coloca na pilha de ck criticas
		}
		next_state = fast_sort_uc(); // escolhe o a maior heuristica da borda e deleta ela
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

	//time(&tend);// get total time
	elapsed_time = difftime(tend = clock(), tbegin);
	report();
}

void uc_search::agent_munit() {

	std::string hash_key;
	std::vector<unsigned int> aux_vector(critical_data.get_mu_location().size(), 0);
	state next_state(aux_vector, critical_data, "uniform");
	clock_t tbegin, tend;
	unsigned int convergenceValue;
	std::vector <state> priority;

	tbegin = clock(); // get  begin time


	while (!next_state.get_cklist().empty()) //loop ate a pilha ficar vazia (nesse caso é cklist vazio pq a ordem quando nao tem mais estados retorna uma "state" com o ck_list vazio
	{
		//for (size_t i = 0; i < priority.size(); i++)
		//{
		//	std::cout << priority[i].get_heuristic() << std::endl;
		//}
		//std::cout << std::endl;
		//ordena_heuristica_uc(priority); //ordena a pilha 
		//for (size_t i = 0; i < priority.size(); i++)
		//{
		//	std::cout << priority[i].get_heuristic() << std::endl;
		//}
		//std::cout << "---------"<< std::endl;
		//next_state = priority.back(); //retira o primeiro da pilha e armazena em uma nova variavel
		//priority.pop_back(); // deleta o primeiro da pilha
		no_of_visited_solutions++; // adiciona em 1 o numero de solucoes visitadas
		hash_key = hashkey(next_state.get_cklist()); // calculo da funcao hash para o vetor
		/*std::cout << "proximo estado ---------------" << std::endl;
		printavetor(next_state.get_cklist());
		std::cout << "heuristica: " << next_state.get_heuristic() << std::endl;*/
		if (visited_states.find(hash_key) != visited_states.end()) // caso o estado esteja na tabela hash
		{
			convergenceValue = visited_states.find(hash_key)->second;  //retorna o valor de convergencia e armazena em uma variavel
			if (convergenceValue == 3) // valores de convergencia (3-> nao observavel nao visitado) (1-> visitado e nao observavel)
			{
				visited_states.find(hash_key)->second = 1;  // muda o valor de convergencia para "1"
				lopt.push(next_state.get_cklist());  // armazena na pilha de resultados
			}
			else
			{
				visited_states.find(hash_key)->second = 0; // caso encontre na tabela e é observavel - seta o valor para 0 (0-> visitado e observavel)
				for (int i = next_state.get_cklist().size() - 1; i >= 0; i--) //expansao do estado
				{
					aux_vector = next_state.get_cklist(); // estado a ser expandido armazenado em uma nova variavel
					if (aux_vector[i] == 0)
					{
						aux_vector[i] = 1; //transforma 0 em 1
						hash_key = hashkey(aux_vector); //calcula a hash do estado criado
						if (visited_states.find(hash_key) == visited_states.end()) //procura na tabela hash
						{//caso nao encontrado
							if (test_ck_munit(aux_vector) == 0) // verifica se contem uma tupla critica
							{//caso nao tenha tupla critica
								state aux_state(aux_vector, critical_data, "uniform");
								temporaryPile.push_back(aux_state); // armazena na fila de prioridade
							}
						}
						else
						{// caso encontre na tabela hash
							convergenceValue = visited_states.find(hash_key)->second; // verifica a convergencia armazenada
							if (convergenceValue == 2) // caso seja 2 (2-> observavel nao visitado)
							{
								state aux_state(aux_vector, critical_data, "uniform");
								temporaryPile.push_back(aux_state); // armazena na fila de prioridade
							}
							else if (convergenceValue == 3) // caso seja 3 (3-> nao observavel nao visitado)
							{
								if (test_ck_munit(aux_vector) == 0) // verifica se contem uma tupla critica
								{//caso nao tenha tupla critica
									state aux_state(aux_vector, critical_data, "uniform");
									temporaryPile.push_back(aux_state); // armazena na fila de prioridade
								}
							}
						}
					}
					else { break; }
				}
			}
		}
		else if (critical_data.munit_criticality(next_state.get_cklist()) == 0)
		{//caso nao esteja na tabela hash e seja observavel
			for (int i = next_state.get_cklist().size() - 1; i >= 0; i--)//expansao do estado
			{
				aux_vector = next_state.get_cklist(); // estado a ser expandido armazenado em uma nova variavel
				if (aux_vector[i] == 0)
				{
					aux_vector[i] = 1;
					hash_key = hashkey(aux_vector); //calcula a hash do estado criado
					if (visited_states.find(hash_key) == visited_states.end())
					{
						if (test_ck_munit(aux_vector) == 0)
						{
							state aux_state(aux_vector, critical_data, "uniform");
							temporaryPile.push_back(aux_state); // armazena na fila de prioridad
						}
					}
					else
					{// caso encontre na tabela hash
						convergenceValue = visited_states.find(hash_key)->second; // verifica a convergencia armazenada
						if (convergenceValue == 2) // caso seja 2 (2-> observavel nao visitado)
						{
							state aux_state(aux_vector, critical_data, "uniform");
							temporaryPile.push_back(aux_state); // armazena na fila de prioridade
						}
						else if (convergenceValue == 3) // caso seja 3 (3-> nao observavel nao visitado)
						{
							if (test_ck_munit(aux_vector) == 0) // verifica se contem uma tupla critica
							{//caso nao tenha tupla critica
								state aux_state(aux_vector, critical_data, "uniform");
								temporaryPile.push_back(aux_state); // armazena na fila de prioridade
							}
						}
					}
				}
				else { break; }
			}
		}
		else
		{ //caso nao esteja na tabela hash e nao seja observavel
			//x.positions = next_state;
			hash_key = hashkey(next_state.get_cklist());
			visited_states.emplace(hash_key, 1);
			lopt.push(next_state.get_cklist());  // coloca na pilha de ck criticas
		}
		next_state = fast_sort_uc(); // escolhe o a maior heuristica da borda e deleta ela
		//if (no_of_visited_solutions / 2461.0 >= 1)
		//{
		//	std::cout << "100%: " << lopt.size() << std::endl;
		//	system("pause");
		//}
		//else if (no_of_visited_solutions / 2461.0 >= 0.75)
		//{
		//	std::cout << "75%: " << lopt.size() << std::endl;
		//}
		//else if (no_of_visited_solutions / 2461.0 >= 0.5)
		//{
		//	std::cout << "50%: " << lopt.size() << std::endl;
		//}
		//else if (no_of_visited_solutions / 2461.0 >= 0.25)
		//{
		//	std::cout << "25%: " << lopt.size() << std::endl;
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
		//if (visited_states.size() > 0)
		//{
		//	visited_states.clear();
		//}

	}

	//time(&tend);// get total time
	elapsed_time = difftime(tend = clock(), tbegin);
	report();
}

void uc_search::multiple_munit_report() {
	int maxSolution = 1000;
	int solutionNumber = 0;
	std::ofstream  status_file;
	status_file.open("status_reportA.txt", std::ios::trunc);
	while (solutionNumber < maxSolution)
	{
		clock_t searchTime = clock();
		solutionNumber++;
		agent_munit();
		status_file << "solucao: " << solutionNumber << " tempo_de_execucao: " << abs(searchTime - clock()) << " ms" << " memoria: " << get_memory() << "\n";
	}
}


unsigned int uc_search::test_ck_4(std::vector<unsigned int> x) {

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

unsigned int uc_search::test_ck_munit(std::vector<unsigned int> x) {

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

//
//void uc_search::save_state() {
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

void uc_search::report() {

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

void uc_search::printavetor(std::vector<unsigned int> x) {
	for (unsigned int i = 0; i < x.size(); i++)
	{
		std::cout << x[i];
	}
	std::cout << std::endl;
}

void uc_search::free_memory(std::vector<unsigned int> nextState) {
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
