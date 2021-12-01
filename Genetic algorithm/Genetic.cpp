#include "Genetic.h"
#include <numeric>	//std::accumulate
#include <random>
#include <iostream>
#include <unordered_set>

Genetic::Genetic(int am_Cities, int it_am, std::vector<std::vector<int>> routes) :amount_Cities(am_Cities), amountGenotypes(it_am * 6),
roads(routes), path(amount_Cities), objectValues(amountGenotypes), fitnessValues(amountGenotypes), iter_amount(it_am / 4) {}

int Genetic::getAnswer()
{
	return length;
}

std::vector<int> Genetic::getPath()
{
	return path;
}

double Genetic::fitnessFunction(std::vector<int> individual)
{
	std::vector<int> cost(individual.size() - 1);
	for (int i = 0; i < cost.size(); i++)
	{
		cost[i] = roads[individual[i]][individual[i + 1]];
	}

	return 1 / std::accumulate(cost.begin(), cost.end(), 0.0);
}

std::pair<int, int> Genetic::objectFunction(std::vector<std::vector<int>> Population)	//returns <min_length, min_path_index>
{
	std::pair<int, int> min_route;	//first - value, second - i
	std::vector<int> cost(Population[0].size() - 1);

	for (int i = 0; i < Population.size(); i++)
	{
		for (int j = 0; j < cost.size(); j++)
		{
			cost[j] = roads[Population[i][j]][Population[i][j + 1]];
		}
		objectValues[i] = std::accumulate(cost.begin(), cost.end(), 0.0);

		if (i == 0)
		{
			min_route.first = objectValues[0];
			min_route.second = 0;
		}
		else if (objectValues[i] < min_route.first)
		{
			min_route.first = objectValues[i];
			min_route.second = i;
		}
	}

	return min_route;
}

void Genetic::crossoverMutation(std::vector<int> &parent1, std::vector<int> &parent2)
{
	std::vector<int> child1, child2;
	std::vector<int> tmp_vec1 = parent1;
	std::vector<int> tmp_vec2 = parent2;
	std::reverse(tmp_vec1.begin(), tmp_vec1.end());
	std::reverse(tmp_vec2.begin(), tmp_vec2.end());
	
	tmp_vec1.erase(tmp_vec1.cbegin() + tmp_vec1.size() / 2 + 1, tmp_vec1.cend());
	tmp_vec2.erase(tmp_vec2.cbegin(), tmp_vec2.cbegin() + tmp_vec2.size() / 2 - 1);

	std::unordered_set<int> tmp_set1, tmp_set2;
	tmp_set1.insert(tmp_vec1.begin(), tmp_vec1.end());
	tmp_set1.insert(tmp_vec2.begin(), tmp_vec2.end());
	tmp_set2.insert(tmp_vec2.begin(), tmp_vec2.end());
	tmp_set2.insert(tmp_vec1.begin(), tmp_vec1.end());

	std::copy(tmp_set1.begin(), tmp_set1.end(), std::back_inserter(child1));
	std::copy_if(tmp_set2.begin(), tmp_set2.end(), std::back_inserter(child1), [tmp_set2](int x) { auto search = tmp_set2.find(x); return search == tmp_set2.end(); });		//tmp_set2.contains(x) in C++20

	///Mutation
	std::vector<bool> visited(amount_Cities);

	for (int i = 0; i < child1.size(); i++)
	{
		visited[child1[i]] = true;
	}

	for (int i = 0; i < visited.size(); i++)
	{
		if (visited[i] == false) child1.push_back(i);
	}

	child2 = child1;
	child2[child2.size() - 1] = child2[child2.size() - 2];
	child2[child2.size() - 2] = child1[child2.size() - 1];
	///Adding and Reduction
	parent1 = child1;
	parent2 = child2;
}

void Genetic::reduction(std::vector<std::vector<int>>& Population)
{
	std::vector<std::pair<double, int>> best(fitnessValues.size()); //<value, i>
	bool opt;

	for (int i = 0; i < best.size(); i++)
	{
		best[i].first = fitnessValues[i];
		best[i].second = i;
	}

	std::sort(best.begin(), best.end(), std::greater<>());
	best.resize(best.size() / 2);

	for (int i = 0; i < Population.size(); i++)
	{
		opt = false;

		for (int b = 0; b < best.size(); b++)
		{
			if (best[b].second == i) opt = true;
			break;
		}

		if (!opt) Population.erase(Population.begin() + i);
	}

	std::copy(Population.begin(), Population.end(), std::back_inserter(Population));
}

void Genetic::solve()
{
	std::vector<std::vector<int>> Population(amountGenotypes, std::vector<int>(amount_Cities)); //popul-n[i,j]: i - am_genTypes, j - am_gens

	for (int i = 0; i < amountGenotypes; i++)
	{
		std::iota(Population[i].begin(), Population[i].end(), 0);				//fills vector with 0..amount_Cities-1 (sorted)
		std::random_shuffle(Population[i].begin(), Population[i].end());		//change to std::ranges::shuffle in C++20

		fitnessValues[i] = fitnessFunction(Population[i]);
	}

	for (int i = 0; i < iter_amount; i++)
	{
		reduction(Population);

		for (int j = 0; j < (Population.size() / 2); j++)	//new population
		{
			crossoverMutation(Population[j], Population[Population.size() - 1 - j]);
		}

		for (int f = 0; f < Population.size(); f++)
		{
			fitnessValues[f] = fitnessFunction(Population[f]);
		}
	}

	result = objectFunction(Population);
	length = result.first;
	path = Population[result.second];
}

