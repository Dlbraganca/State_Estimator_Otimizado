#pragma once
/*#ifndef CK_SEARCH_H
#include "ck_search.h"
#endif // !CK_SEARCH_H
*/
#ifndef CK_SEARCH_H
#include "ck_search.h"
#endif // !CK_SEARCH_H

class width_search : public ck_search {
protected:
	//members
	std::stack<std::vector<unsigned int>> Asp;
public:
	//methods
	width_search();
	~width_search() {};
	width_search(unsigned int KLIM, unsigned int KMIN, unsigned int KMAX, unsigned int DIM, std::string CRIT_TYPE, criticality&);
	width_search(const char*, criticality&);
	width_search(const char*);

};