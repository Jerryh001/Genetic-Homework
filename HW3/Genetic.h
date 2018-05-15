#pragma once
#include"stdafx.h"
#include"GData.h"
class Genetic
{
	double Pc = 0.8;
	double Pm = 0.07;
	
	array<GData,500> data;
	GData best;
	int turn = 0;
public:
	string output;
	Genetic();
	~Genetic();
	void Run();
	bool IsEnd();
	void Init();
	void Evaluate();
	void Select();
	void Cross();
	void Mutation();
};

