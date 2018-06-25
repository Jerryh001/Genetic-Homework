#pragma once
#include"stdafx.h"
#include"DataSet.h"
#include"Schedule.h"
class Genetic
{
	int population_size;
	DataSet dataset;
	vector<Schedule> population;
	vector<Schedule> new_population;
	vector<Schedule> possible_solution;
	void Crossover();
	void Selection();
	void Print(int level);
public:
	Genetic(const string& datapath,int popsize=200);
	void Run();
	~Genetic();
};

