#include<vector>

class state {

protected:
	std::vector<unsigned int> CKLIST;
	unsigned int HEURISTIC;
	unsigned int DEEP;
public:
	state() {};
	~state() {};
	state(std::vector<unsigned int> cklist) {
		CKLIST = cklist;
		DEEP = set_depth(CKLIST);
		HEURISTIC = 0;
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
	void heuristic();

	std::vector<unsigned int> get_cklist() {
		return CKLIST;
	}
	unsigned int get_heuristic() {
		return HEURISTIC;
	}
};