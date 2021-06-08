#include "../headers/bfs_search.h"


bfs_search::bfs_search()
{
}

bfs_search::bfs_search(const char*, criticality&)
{
}

bfs_search::bfs_search(const char* CK_PARAM_FILE, std::string PARAM_STR) : ck_search(CK_PARAM_FILE)
{
	param_str = PARAM_STR;
}

bfs_search::bfs_search(unsigned int KLIM, unsigned int KMIN, unsigned int KMAX, unsigned int DIM, std::string CRIT_TYPE, criticality&)
{
}

void bfs_search::agent_measurement() {

	std::string hash_key;
	std::vector<unsigned int> aux_vector(critical_data.get_meas_location().size(), 0);
	state aux(aux_vector, param_str);
	state next_state;
	clock_t tbegin, tend, tfreememoryBegin, tmaxFreememory = 100000;
	unsigned int convergenceValue;
	std::queue <state> priority;

	tbegin = clock(); // get  begin time
	tfreememoryBegin = clock();

	priority.push(aux);

	while (!priority.empty()) //loop ate a pilha ficar vazia
	{
		//printavetor(next_state.get_cklist());
		next_state = priority.front(); //retira o primeiro da pilha e armazena em uma nova variavel
		priority.pop(); // deleta o primeiro da pilha
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
								state aux_state(aux_vector, param_str);
								priority.push(aux_state); // armazena na fila de prioridade
							}
						}
						else
						{// caso encontre na tabela hash
							convergenceValue = visited_states.find(hash_key)->second; // verifica a convergencia armazenada
							if (convergenceValue == 2) // caso seja 2 (2-> observavel nao visitado)
							{
								state aux_state(aux_vector, param_str);
								priority.push(aux_state); // armazena na fila de prioridade
							}
							else if (convergenceValue == 3) // caso seja 3 (3-> nao observavel nao visitado)
							{
								if (test_ck_4(aux_vector) == 0) // verifica se contem uma tupla critica
								{//caso nao tenha tupla critica
									state aux_state(aux_vector, param_str);
									priority.push(aux_state); // armazena na fila de prioridade
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
							state aux_state(aux_vector, param_str);
							priority.push(aux_state); // armazena na fila de prioridad
						}
					}
					else
					{// caso encontre na tabela hash
						convergenceValue = visited_states.find(hash_key)->second; // verifica a convergencia armazenada
						if (convergenceValue == 2) // caso seja 2 (2-> observavel nao visitado)
						{
							state aux_state(aux_vector, param_str);
							priority.push(aux_state); // armazena na fila de prioridade
						}
						else if (convergenceValue == 3) // caso seja 3 (3-> nao observavel nao visitado)
						{
							if (test_ck_4(aux_vector) == 0) // verifica se contem uma tupla critica
							{//caso nao tenha tupla critica
								state aux_state(aux_vector, param_str);
								priority.push(aux_state); // armazena na fila de prioridade
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

	//time(&tend);// get total time
	elapsed_time = difftime(tend = clock(), tbegin);
	report();
}

void bfs_search::agent_munit() {

	std::string hash_key;
	std::vector<unsigned int> aux_vector(critical_data.get_mu_location().size(), 0);
	state aux(aux_vector, param_str);
	state next_state;
	clock_t tbegin, tend, tfreememoryBegin, tmaxFreememory = 100000;
	unsigned int convergenceValue;
	std::queue <state> priority;

	tbegin = clock(); // get  begin time
	tfreememoryBegin = clock();
	priority.push(aux);

	while (!priority.empty()) //loop ate a pilha ficar vazia
	{
		//printavetor(next_state.get_cklist());
		next_state = priority.front(); //retira o primeiro da pilha e armazena em uma nova variavel
		priority.pop(); // deleta o primeiro da pilha
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
							if (test_ck_munit(aux_vector) == 0) // verifica se contem uma tupla critica
							{//caso nao tenha tupla critica
								state aux_state(aux_vector, param_str);
								priority.push(aux_state); // armazena na fila de prioridade
							}
						}
						else
						{// caso encontre na tabela hash
							convergenceValue = visited_states.find(hash_key)->second; // verifica a convergencia armazenada
							if (convergenceValue == 2) // caso seja 2 (2-> observavel nao visitado)
							{
								state aux_state(aux_vector, param_str);
								priority.push(aux_state); // armazena na fila de prioridade
							}
							else if (convergenceValue == 3) // caso seja 3 (3-> nao observavel nao visitado)
							{
								if (test_ck_munit(aux_vector) == 0) // verifica se contem uma tupla critica
								{//caso nao tenha tupla critica
									state aux_state(aux_vector, param_str);
									priority.push(aux_state); // armazena na fila de prioridade
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
							state aux_state(aux_vector, param_str);
							priority.push(aux_state); // armazena na fila de prioridad
						}
					}
					else
					{// caso encontre na tabela hash
						convergenceValue = visited_states.find(hash_key)->second; // verifica a convergencia armazenada
						if (convergenceValue == 2) // caso seja 2 (2-> observavel nao visitado)
						{
							state aux_state(aux_vector, param_str);
							priority.push(aux_state); // armazena na fila de prioridade
						}
						else if (convergenceValue == 3) // caso seja 3 (3-> nao observavel nao visitado)
						{
							if (test_ck_munit(aux_vector) == 0) // verifica se contem uma tupla critica
							{//caso nao tenha tupla critica
								state aux_state(aux_vector, param_str);
								priority.push(aux_state); // armazena na fila de prioridade
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
		//if (no_of_visited_solutions/ 2461.0 >= 1)
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

	}

	//time(&tend);// get total time
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

unsigned int bfs_search::test_ck_munit(std::vector<unsigned int> x) {

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
