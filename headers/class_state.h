#include<vector>
#include<string>
#include"./criticality.h"

class state {

protected:
	std::vector<unsigned int> CKLIST;
	double DEEPTH;
	double HEURISTIC = 0;
	criticality CRITICAL_DATA;
public:
	state() {};
	~state() {};
	state(std::vector<unsigned int> cklist, std::string param_str) {
		CKLIST = cklist;
	}
	state(std::vector<unsigned int> cklist, criticality& critical_data,std::string x) {
		CKLIST = cklist;
		CRITICAL_DATA = critical_data;
		DEEPTH = set_depth(CKLIST);
		if (x == "measurement")
		{
			HEURISTIC = measurement_heuristic();
		}
		else if (x == "munit")
		{
			HEURISTIC = munit_heuristic();
		}
		HEURISTIC = HEURISTIC + DEEPTH;
	}
	std::vector<unsigned int> get_cklist() {
		return CKLIST;
	}
	double get_heuristic() {
		return HEURISTIC;
	}
	void set_heuristic(double x) { HEURISTIC = x; }
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
		return CRITICAL_DATA.get_det();
	}
	double munit_heuristic() {

		CRITICAL_DATA.munit_criticality(CKLIST);
		return CRITICAL_DATA.get_det();
	}
};