#pragma once
#include"stdafx.h"
class GData
{
	array<double, 10> x[2];
	double value[2];
	double fitness;
	void ConvertToValue();
	void CalculateFitness();
	void GeneInit(int index, double offset = 0);
public:
	GData();
	void Evaluate();
	double GetValue(unsigned int index);
	double GetFitness();
	void Mutation(const double& p);
	

	friend void Cross(GData& a, GData& b, const double& p);
};
void Cross(GData& a, GData& b, const double& p);
