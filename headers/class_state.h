#include<vector>
#include<string>
#include"criticality.h"

class state {

protected:
	std::vector<unsigned int> CKLIST;
	double PATHCOST; 
	double HEURISTIC;
	double F;
	criticality CRITICAL_DATA;
public:
	state() {};
	~state() {};
	state(std::vector<unsigned int> cklist, std::string param_str) : CKLIST(cklist), HEURISTIC(0){} //construtor antigo considerando o custo do caminho como profudidade
	state(std::vector<unsigned int> cklist, criticality& critical_data,std::string x) : CKLIST(cklist), CRITICAL_DATA(critical_data) , HEURISTIC(0), PATHCOST (set_depth(CKLIST)) {
		//construtor antigo considerando o custo do caminho como profudidade
		if (x == "measurement")
		{
			HEURISTIC = measurement_heuristic();
		}
		else if (x == "munit")
		{
			HEURISTIC = munit_heuristic();
		}
		F = HEURISTIC + PATHCOST;
	}
	state(std::vector<unsigned int> cklist, criticality& critical_data, std::string x, state lastState) : CKLIST(cklist), CRITICAL_DATA(critical_data) {
		//novo construtor com custo do caminho diferente
		if (x == "measurement")
		{
			HEURISTIC = measurement_heuristic();
			PATHCOST = 1.0 + 1.0 / abs((lastState.get_heuristic() - HEURISTIC)); //calculo do custo do caminho do estado anterior para o estado atual
			PATHCOST = lastState.get_path_cost() + PATHCOST; // acumulo dos custos
			F = HEURISTIC + PATHCOST;
		}
		else if (x == "munit")
		{
			HEURISTIC = munit_heuristic();
			PATHCOST = 1.0 + 1.0 / abs((lastState.get_heuristic() - HEURISTIC)); //calculo do custo do caminho do estado anterior para o estado atual
			PATHCOST = lastState.get_path_cost() + PATHCOST; // acumulo dos custos
			F = HEURISTIC + PATHCOST;
		}
		else if (x == "munit_uniform")
		{
			HEURISTIC = munit_heuristic();
			PATHCOST = 1.0 + 1.0 / abs((lastState.get_heuristic() - HEURISTIC)); //calculo do custo do caminho do estado anterior para o estado atual
			PATHCOST = lastState.get_path_cost() + PATHCOST; // acumulo dos custos
		}
		else if (x == "measurement_uniform")
		{
			HEURISTIC = measurement_heuristic();
			PATHCOST = 1.0 + 1.0 / abs((lastState.get_heuristic() - HEURISTIC)); //calculo do custo do caminho do estado anterior para o estado atual
			PATHCOST = lastState.get_path_cost() + PATHCOST; // acumulo dos custos
		}
		if (PATHCOST == std::numeric_limits<double>::infinity())
		{
			PATHCOST = std::numeric_limits<float>::max() - 1;
		}
		if (F == std::numeric_limits<double>::infinity())
		{
			F = std::numeric_limits<float>::max() - 1;
		}
	}
	std::vector<unsigned int> get_cklist() {
		return CKLIST;
	}
	double get_heuristic() { return HEURISTIC; }
	double get_f() { return F; }
	double get_deepth() { return PATHCOST; }; //funcao antiga quando o custo do caminho era a profundidade
	double get_path_cost() { return PATHCOST; }
	void set_heuristic(double x) { HEURISTIC = x; }
	void set_f(double x) { F = x; }
	void set_deepth(double x) { PATHCOST = x; };
private:
	double set_depth(std::vector<unsigned int> cklist) {
		unsigned int count = 0;
		for (size_t i = 0; i < cklist.size(); i++)
		{
			if (cklist[i] == 1)
			{
				count++;
			}
		}
		//return 1.0/count;
		return count;
	}
	double measurement_heuristic() {

		CRITICAL_DATA.measurement_criticality(CKLIST);
		return abs(CRITICAL_DATA.get_det());
	}
	double munit_heuristic() {

		CRITICAL_DATA.munit_criticality(CKLIST);
		return abs(CRITICAL_DATA.get_det());
	}
};