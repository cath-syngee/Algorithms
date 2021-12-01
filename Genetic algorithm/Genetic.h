#pragma once
#include <vector>

class Genetic
{
private:
	int iter_amount;
	int amount_Cities;
	int length;
	int amountGenotypes;
	std::pair<int, int> result;

	std::vector<std::vector<int>> roads;
	std::vector<int> path;
	std::vector<int> objectValues;
	std::vector<double> fitnessValues;

public:
	Genetic(int, int, std::vector<std::vector<int>>);
	double fitnessFunction(std::vector<int>);
	void crossoverMutation(std::vector<int>&, std::vector<int>&);
	void reduction(std::vector<std::vector<int>>&);
	std::pair<int, int> objectFunction(std::vector<std::vector<int>>);	//целевая функция
	void solve();
	std::vector<int> getPath();
	int getAnswer();
};