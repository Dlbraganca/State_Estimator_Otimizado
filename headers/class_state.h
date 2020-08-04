#include<vector>
#include<string>

class state {

protected:
	std::vector<unsigned int> CKLIST;
	unsigned int HEURISTIC;
	unsigned int DEEPTH;
public:
	state() {};
	~state() {};
	state(std::vector<unsigned int> cklist, std::string param_str) {
		CKLIST = cklist;
		if (param_str.compare("bb_ids"))
		{
			DEEPTH = set_depth(CKLIST);
		}
		else if (param_str.compare("bb_a*s"))
		{
			DEEPTH = set_depth(CKLIST);
			HEURISTIC = DEEPTH + heuristic();
		}
	}
	std::vector<unsigned int> get_cklist() {
		return CKLIST;
	}
	unsigned int get_heuristic() {
		return HEURISTIC;
	}
private:
	unsigned int set_depth(std::vector<unsigned int> cklist) {
		unsigned int count = 0;
		for (size_t i = 0; i < cklist.size(); i++)
		{
			if (cklist[i] == 1)
			{
				count++;
			}
		}
		return count;;
	}
	unsigned int heuristic() {
		return 0;
	}

};