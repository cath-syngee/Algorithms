#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include "Genetic.h"

int main()
{
	int am_cities, iterationAmount;
	int res = 0;
	std::ifstream file("matrix.txt");

	file >> am_cities;
	iterationAmount = 20 * am_cities;

	std::vector<std::vector<int>> routes(am_cities, std::vector<int>(am_cities));

	for (int i = 0; i < am_cities; i++)
	{
		for (int j = 0; j < am_cities; j++)
		{
			file >> routes[i][j];
			res += routes[i][j];
		}
	}

	Genetic gens(am_cities, iterationAmount, routes);
	int gen_answer;
	std::vector<int> gen_path;

	gens.solve();
	gen_path = gens.getPath();
	gen_answer = gens.getAnswer();

	std::cout << "Answer: " << gen_answer << std::endl;
	std::cout << "Path: ";
	std::for_each(gen_path.begin(), gen_path.end(), [](int &p) {std::cout << p << " "; });
	std::cout << std::endl;
	system("pause");
	
	return 0;
}