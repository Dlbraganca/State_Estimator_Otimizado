#include  "width_search.h"


width_search::width_search() {
};

width_search::width_search(unsigned int KLIM, unsigned int KMIN, unsigned int KMAX, unsigned int DIM, std::string CRIT_TYPE, criticality& CRITICAL_DATA) : ck_search(KLIM, KMIN, KMAX, DIM, CRIT_TYPE, CRITICAL_DATA) {
	// default constructor
};

width_search::width_search(const char* CK_PARAM_FILE, criticality& CRITICAL_DATA) : ck_search(CK_PARAM_FILE, CRITICAL_DATA) {
	// default constructor
};

width_search::width_search(const char* CK_PARAM_FILE) : ck_search(CK_PARAM_FILE) {
	// default constructor
};

/*void width_search::branch() {
	// branch the subset associated with current solution T
	std::size_t Tsize = T.size();
	unsigned int k = 0;
	Tk = T; //copy T into Tk
	for (unsigned int pos = 0; pos < dim; pos++) { // evaluate set cardinality
		k += Tk[pos];
	}
	if (k < kmax) {
		std::size_t pos = Tsize - 1;
		while (Tk[pos] == 0) {
			Tk[pos] = 1;
			bound();
			if (l_inf < 1.0) { Asp.push(Tk); }      // if l_inf == 0 child Tk is added to active subset
			else if (l_inf < 2.0) { lopt.push(Tk); } // if l_inf == 1 child Tk is added to lopt
													 // if l_inf >= 2 child Tk is discarded
			no_of_visited_solutions++;               // increase the solutions counter
			if (Tk[pos] == 1) { Tk[pos] = 0; }// reset Tk
			if (pos == 0) { break; } // vector initial position has been reached
			else { --pos; }
		}
	}
};

void width_search::save_state() {

	std::ofstream active_subsets_file("active_subsets.temp", std::ios::out | std::ios::trunc);
	std::ofstream critical_list_file("critical_list.temp", std::ios::out | std::ios::trunc);
	std::ofstream temp_report_file("temp_report.temp", std::ios::out | std::ios::trunc);
	std::vector<unsigned int> T_temp;
	std::stack<std::vector<unsigned int>> Asp_buff = Asp, lopt_buff = lopt;


	// lista de tuplas criticas
	if (critical_list_file.is_open()) { // impressao das tuplas criticas
		while (!lopt_buff.empty()) {
			T_temp = lopt_buff.top(); // get stack top element
			lopt_buff.pop(); // pop stack
			for (std::vector<unsigned int>::iterator it = T_temp.begin(); it != T_temp.end(); ++it) {
				critical_list_file << *it << " ";
			}
			critical_list_file << std::endl;
		}
		critical_list_file << 9999;// successful writing
		critical_list_file.close();
	}
	else
	{
		std::cout << "Unable to print the temporary critical tuples file" << std::endl;
	}
	// lista de conjuntos ativos
	if (active_subsets_file.is_open()) { // impressao do conjunto de soluções não visitadas
		while (!Asp_buff.empty()) {
			T_temp = Asp_buff.top(); //access first element of the list
			Asp_buff.pop(); //delete this element
			for (std::vector<unsigned int>::iterator it = T_temp.begin(); it != T_temp.end(); ++it) {
				active_subsets_file << *it << " ";
			}
			active_subsets_file << std::endl;
		}
		active_subsets_file << 9999;// successful writing
		active_subsets_file.close();
	}
	else
	{
		std::cout << "Unable to print the temporary active subsets list file" << std::endl;
	}
	// parametros de busca
	if (temp_report_file.is_open()) { // impressao parametros da busca

		temp_report_file << klim << std::endl;
		temp_report_file << kmin << std::endl;
		temp_report_file << kmax << std::endl;
		temp_report_file << dim << std::endl;
		temp_report_file << crit_type << std::endl;
		temp_report_file << no_of_visited_solutions << std::endl;
		temp_report_file << elapsed_time << std::endl;
		temp_report_file << 9999;// successful writing
		temp_report_file.close();
	}
	else
	{
		std::cout << "Unable to print the temporary report file" << std::endl;
	}
};

void width_search::load_state() {
	std::ifstream active_subsets_file("active_subsets.temp", std::ios::in);
	std::ifstream critical_list_file("critical_list.temp", std::ios::in);
	std::ifstream temp_report_file("temp_report.temp", std::ios::in);
	std::string line;
	std::vector<unsigned int> T_temp;
	std::stack<std::vector<unsigned int>> Asp_buff, lopt_buff;

	// read temp report file
	if (temp_report_file.is_open()) {
		getline(temp_report_file, line);
		klim = std::stoul(line, nullptr, 10);
		getline(temp_report_file, line);
		kmin = std::stoul(line, nullptr, 10);
		getline(temp_report_file, line);
		kmax = std::stoul(line, nullptr, 10);
		getline(temp_report_file, line);
		dim = std::stoul(line, nullptr, 10);
		getline(temp_report_file, line);
		crit_type = line;
		getline(temp_report_file, line);
		no_of_visited_solutions = std::stoul(line, nullptr, 10);
		getline(temp_report_file, line);
		elapsed_time = std::stod(line, nullptr);
		temp_report_file.close();
	}
	else { std::cout << "Unlable to open temporary report file" << std::endl; }
	// read temp active_subsets file
	if (active_subsets_file.is_open()) {
		unsigned int corrupted = 1;
		while (getline(active_subsets_file, line)) {
			if (line.compare("9999") != 0) {
				unsigned int i = 0;
				for (unsigned int str_pos = 0; str_pos < line.size(); str_pos++) {
					if (line[str_pos] = '0') {
						Tk[i] = 0;
						i++;
					}
					else if (line[str_pos] = '1') {
						Tk[i] = 1;
						i++;
					}
				}
				Asp.push(Tk);
			}
			else { corrupted = 0; }
		}
		Tk.clear();
		if (corrupted) { std::cout << "Warning Asp might be corrupted"; }
		active_subsets_file.close();
	}
	else { std::cout << "Unlable to open temporary active subsets file" << std::endl; }
	// read temp active_subsets file
	if (critical_list_file.is_open()) {
		unsigned int corrupted = 1;
		while (getline(critical_list_file, line)) {
			if (line.compare("9999") != 0) {
				unsigned int i = 0;
				for (unsigned int str_pos = 0; str_pos < line.size(); str_pos++) {
					if (line[str_pos] = '0') {
						Tk[i] = 0;
						i++;
					}
					else if (line[str_pos] = '1') {
						Tk[i] = 1;
						i++;
					}
				}
				lopt.push(Tk);
			}
			else { corrupted = 0; }
		}
		Tk.clear();
		if (corrupted) { std::cout << "Warning lopt might be corrupted"; }
		active_subsets_file.close();
	}
	else { std::cout << "Unlable to open temporary critical tuples file" << std::endl; }
};

void width_search::search() {
	// performs bb search (depth first search)
	time_t tpartial1, tpartial2;
	time(&tpartial1); // get partial begin time
	time(&tbegin); // get  begin time
	T = std::vector<unsigned int>(dim, 0);
	no_of_visited_solutions++;
	Asp.push(std::vector<unsigned int>(dim, 0));
	while (!Asp.empty()) {
		T = Asp.top();
		Asp.pop();
		branch();
		time(&tpartial2);
		if (difftime(tpartial2, tpartial1) > 3600) {
			time(&tend); // get current time
			elapsed_time = difftime(tend, tbegin);
			std::cout << "Saving search state...";
			save_state(); // save state
			std::cout << "done!!!" << std::endl;
			time(&tpartial1); // reset partial begin time
			time(&tpartial2); // reset partial end time
		}
	}
	if (lopt.empty()) {
		std::cout << " No critical ck-tuple has been found" << std::endl;
	}
	else
	{
		lopt_buff = lopt; // store in buffer in order to transfer to another applications
		time(&tend);// get total time
		elapsed_time = difftime(tend, tbegin);
		report();
	}
};

void width_search::report() {

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
		report_file << "Tipo de busca:" << "Em profundidade" << std::endl;
		report_file << "no. solucoes visitadas:" << no_of_visited_solutions << std::endl;
		report_file << "tempo decorrido:" << elapsed_time << std::endl;
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
};*/