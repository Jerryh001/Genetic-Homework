#include "stdafx.h"
#include "HillClimbing.h"


bool HillClimbing::IsTerminate()
{
	return turn>=1000;
}



array<Data, 64> HillClimbing::CreateNeighbor(Data now)
{
	array<Data, 64> nei;
	uint32_t mask = (1 << 3) - 1;
	now.x[0] -= now.x[0] >= 3 ? 3 : now.x[0];
	now.x[1] -= now.x[1] >= 3 ? 3 : now.x[1];
	for (uint32_t i = 0; i <= 7; i++)
	{
		for (uint32_t j = 0; j <= 7; j++)
		{
			nei[i * 7 + j] = { now.x[0] + i, now.x[1] + j };
		}
	}
	return nei;
}

HillClimbing::HillClimbing()
{

}


HillClimbing::~HillClimbing()
{
}

void HillClimbing::Run()
{
	start_time = clock();
	uniform_int_distribution<uint32_t> distribution(0, (1 << 11) - 1);
	while (!IsTerminate())
	{
		turn++;
		Data local;
		local.x[0] = distribution(generator);
		local.x[1] = distribution(generator);
		Eval(local);
		while (1)
		{
			turn_climb++;
			array<Data, 64> neighbor = CreateNeighbor(local);
			bool nochange = true;
			for (Data& d : neighbor)
			{
				Eval(d);
				if (local.value > d.value)
				{
					local = d;
					nochange = false;
				}
			}
			if (nochange) break;
		}
		if (turn == 1 || local.value < best.value)
		{
			best = local;
			if (abs(best.value - 0.3979) < 1e-6)
			{
				ShowResult();
			}
		}
	}
	ShowResult();
}
void HillClimbing::ShowResult()
{
	cout << "turn=" << turn << endl
		<< "turn_climb=" << turn_climb << endl
		<< "x1=" << BinToDouble(best.x[0], 11, -5, 10) << endl
		<< "x2=" << BinToDouble(best.x[1], 11, 0, 15) << endl
		<< "min=" << best.value << endl
		<< "time=" << (clock() - start_time)*1.0 / CLOCKS_PER_SEC << endl << endl;
}
