#pragma once
#include "stdafx.h"
#include "Poly.h"
using namespace Poly;
class HillClimbing
{
	clock_t start_time;
	default_random_engine generator{ static_cast<long unsigned int>(time(0)) };
	Data best;
	uint32_t turn = 0;
	uint32_t turn_climb = 0;
	bool IsTerminate();
	array<Data, 64> CreateNeighbor(Data now);
public:
	HillClimbing();
	~HillClimbing();
	void ShowResult();
	void Run();
};

