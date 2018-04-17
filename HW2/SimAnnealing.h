#pragma once
#include"stdafx.h"
#include"Poly.h"
using namespace Poly;
class SimAnnealing
{
	clock_t start_time;
	default_random_engine generator{ static_cast<long unsigned int>(time(0)) };
	exponential_distribution<double> neighbor_rand;
	Data best;
	double T;
	uint32_t turn = 0;
	bool IsTerminate();
	Data CreateNeighbor(Data now);
public:
	SimAnnealing();
	~SimAnnealing();
	void ShowResult();
	void Run();
};

