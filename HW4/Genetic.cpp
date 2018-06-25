#include "stdafx.h"
#include "Genetic.h"


void Genetic::Crossover()
{
	new_population.clear();
	random_shuffle(population.begin(), population.end());
	for (int i = 0; i < population.size() / 2; i++)
	{
		new_population.emplace_back(population[2 * i].Crossover(population[2 * i + 1]));
		new_population.emplace_back(population[2 * i+1].Crossover(population[2 * i]));
	}
	for (auto& s : new_population)
	{
		s.Mutation();
		s.Decode();
	}
}

void Genetic::Selection()
{
	possible_solution.insert(possible_solution.end(), new_population.begin(), new_population.end());
	population.insert(population.end(), new_population.begin(), new_population.end());
	sort(population.begin(), population.end(), [](auto& a, auto& b) {return a.Decode() < b.Decode(); });
	population.resize(population_size);
}

void Genetic::Print(int level)
{
	cout << "level="<<level << " best="<< population.front().Decode()<<endl;
}

Genetic::Genetic(const string & datapath, int popsize):dataset(datapath)
{
	population_size = popsize;
	population.clear();
	population.reserve(population_size);
	for (int i = 0; i < population_size; i++)
	{
		population.emplace_back(dataset);
	}
	for (auto& s : population)
	{
		s.RandomInit();
		s.Decode();
	}
	possible_solution = population;
}

void Genetic::Run()
{
	int printlevel=1;
	while (possible_solution.size() <= 5000)
	{
		Crossover();
		Selection();
		if (possible_solution.size() >= printlevel * 1000)
		{
			Print(printlevel);
			printlevel++;
		}
	}
}

Genetic::~Genetic()
{
}
