#pragma once
#include "stdafx.h"
#include"Poly.h"
using namespace Poly;
class GeneticPoly
{
private:
	clock_t start_time;
	uint32_t turn=0;
	default_random_engine generator{ static_cast<long unsigned int>(time(0)) };
	static const int population = 10;
	Data best;
	double Pc = 0.7;
	double Pm = 0.05;
	bool bestupdated = false;
	array<Data, population> data;
	void OnePointCross(Data& a, Data& b);
protected:
	void Initial();
	void Evaluate();
	void Select();
	void Crossover();
	void Mutation();
	bool IsTerminate();
public:
	GeneticPoly();
	~GeneticPoly();
	void Run();
	void ShowResult();
};

