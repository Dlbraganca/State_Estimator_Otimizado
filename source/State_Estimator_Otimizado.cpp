
#include <iostream>
#include <string>
#include <fstream>
#include "/UFF/IC/Estimação de Estado/State_Estimator/State_Estimator_Otimizado/headers/basic_ai_search.h"

void main() {

	basic_ai_search search;

	std::ifstream parameters_file("ck_directives.txt", std::ifstream::in); //construtor 
	std::string param_str;

	if (parameters_file.is_open()) {
		std::cout << "Parameter file has been successfully opened. Reading search method ..."; 
		getline(parameters_file, param_str);   //leitura do file "ck_directives.txt - salvando na variavel param_str(tipo de busca)
		parameters_file.close();
		std::cout << "Done !!!" << std::endl;
		basic_ai_search search ("ck_directives.txt", param_str); //contrutor chamando o controlador dos tipos de busca - passando o tipo de busca lida no txt
	}
	else
	{
		std::cout << "Unable to open the search parameters file" << std::endl;
	}
}
