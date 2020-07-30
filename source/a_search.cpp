#include "/UFF/IC/Estimação de Estado/State_Estimator/State_Estimator_Otimizado/headers/a_search.h"

a_search::a_search()
{
}

//a_search::a_search(const char*, criticality&)
//{
//}

a_search::a_search(const char* CK_PARAM_FILE, std::string PARAM_STR) : ck_search(CK_PARAM_FILE)
{
}

//a_search::a_search(unsigned int KLIM, unsigned int KMIN, unsigned int KMAX, unsigned int DIM, std::string CRIT_TYPE, criticality&)
//{
//}

void a_search::agent_measurement() {

	std::string hash_key;
	std::vector<unsigned int> aux(critical_data.get_meas_location().size(), 0);
	std::vector<unsigned int> next_state;
	time_t tbegin = clock(), tend, tfreememoryBegin = clock(), tmaxFreememory = 100000;
	unsigned int convergenceValue;
	SSIZE_T atual_memory;

	priority.push_back(aux);


	while (!priority.empty())
	{
		next_state = priority.back();   //pega o proximo da pilha 
		priority.pop_back(); //deleta da pilha
		no_of_visited_solutions++; //adiciona mais um estado no numero de visitados
		hash_key = hashkey(next_state); //calcula o valor da hash para aquele estado

		if (visited_states.find(hash_key) != visited_states.end()) // busca na tabela hash
		{//caso encontre
			convergenceValue = visited_states.find(hash_key)->second.convergence; // retorna o valor de convergencia
			if (convergenceValue == 3 || convergenceValue == 1) // valores de convergencia (3-> nao observavel nao visitado) (1-> visitado e nao observavel)
			{
				visited_states.find(hash_key)->second.convergence = 1; // muda o valor de convergencia para "1"
				lopt.push(next_state); // armazena na pilha de resultados
			}
			else 
			{
				visited_states.find(hash_key)->second.convergence = 0; // caso encontre na tabela e é observavel - seta o valor para 0 (0-> visitado e observavel)
				for (int i = next_state.size() - 1; i >= 0; i--) //expansao do estado
				{
					aux = next_state; // estado a ser expandido armazenado em uma nova variavel
					if (aux[i] == 0) 
					{
						aux[i] = 1; //transforma 0 em 1
						hash_key = hashkey(aux); //calcula a hash do estado criado
						if (visited_states.find(hash_key) == visited_states.end()) //procura na tabela hash
						{//caso nao encontrado
							if (test_ck_4(aux) == 0) // verifica se contem uma tupla critica
							{//caso nao tenha tupla critica
								priority.push_back(aux); // armazena na fila de prioridade
							}
						}
						else
						{// caso encontre na tabela hash
							convergenceValue = visited_states.find(hash_key)->second.convergence; // verifica a convergencia armazenada
							if (convergenceValue == 2) // caso seja 2 (2-> observavel nao visitado)
							{
								priority.push_back(aux); // armazena na fila de prioridade
							}
						}
					}
					else { break; }
				}
			}
		}
		else if (critical_data.measurement_criticality(next_state) == 0) //caso nao encontre na tabela hash verifica a observabilidaed
		{//caso seja observavel -> expande (mesmo processo anterior)
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
							priority.push_back(aux);
						}
					}
					else
					{
						convergenceValue = visited_states.find(hash_key)->second.convergence;
						if (convergenceValue == 2)
						{
							priority.push_back(aux);
						}
					}
				}
				else { break; }
			}
		}
		else
		{// caso nao seja observavel
			state x; // declara a struct
			x.positions = next_state; //armazena o vetor
			x.convergence = 1; //armazena a convergencia
			hash_key = hashkey(next_state); //calcula a hash
			visited_states.emplace(hash_key, x); //armazena na hash
			lopt.push(next_state); // coloca na pulah de resultados
		}
	
		if (visited_states.size() >= 0)
		{
			visited_states.erase(visited_states.begin(), visited_states.end());
		}
		if (clock() - tfreememoryBegin > 3600000)
		{
			atual_memory = get_memory();
			std::cout << "Saving State...";
			save_state();
			std::cout << "Done\n";
			tfreememoryBegin = clock();
		}
	}
	elapsed_time = difftime(clock(), tbegin);
	report();
}

void a_search::agent_munit() {

	std::string hash_key;
	std::vector<unsigned int> aux(critical_data.get_mu_location().size(), 0);
	std::vector<unsigned int> next_state;
	time_t tbegin, tend;

	tbegin = clock();
	{
		std::stack<std::vector<unsigned int>> priority;

		priority.push(aux);
		while (priority.empty() == false)
		{
			no_of_visited_solutions++;
			next_state = priority.top();
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
								//[hash_key] = true;
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

	elapsed_time = difftime(clock(), tbegin);
	report();
}
bool a_search::comparar_heuristicas(state a, state b) {
	if (a.heuristic != b.heuristic)
	{
		return a.heuristic < b.heuristic;
	}
}

void a_search::ordena_heuristica(std::vector<std::vector<unsigned int>>&a) {
	std::sort(a.begin(), a.end(), a_search::comparar_heuristicas);
}