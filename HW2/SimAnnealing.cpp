#include "stdafx.h"
#include "SimAnnealing.h"


bool SimAnnealing::IsTerminate()
{
	return T < 0;
}

Data SimAnnealing::CreateNeighbor(Data now)
{
	uint32_t mask = (1 << (int)round(neighbor_rand(generator)*11));
	now.x[0] ^= mask;
	mask = (1 << (int)round(neighbor_rand(generator) * 11));
	now.x[1] ^= mask;
	return now;
}

SimAnnealing::SimAnnealing():neighbor_rand(3.5)
{
}


SimAnnealing::~SimAnnealing()
{
}

void SimAnnealing::ShowResult()
{
	cout << "turn=" << turn << endl
		<< "x1=" << BinToDouble(best.x[0], 11, -5, 10) << endl
		<< "x2=" << BinToDouble(best.x[1], 11, 0, 15) << endl
		<< "min=" << best.value << endl
		<< "time=" << (clock() - start_time)*1.0 / CLOCKS_PER_SEC << endl << endl;
}

void SimAnnealing::Run()
{
	start_time = clock();
	uniform_int_distribution<uint32_t> distribution(0, (1 << 11) - 1);
	Data now;
	now.x[0] = distribution(generator);
	now.x[1] = distribution(generator);
	T = 1;
	Eval(now);
	while (!IsTerminate())
	{
		turn++;
		Data nei = CreateNeighbor(now);
		Eval(nei);
		if (nei.value < now.value)
		{
			now = nei;
		} else if (distribution(generator)<(exp(now.value-nei.value))*T*distribution.b())
		{
			now = nei;
		}
		T -= 0.000001;

		if (turn == 1 || best.value > now.value)
		{
			best = now;
			if (abs(best.value - 0.3979) < 1e-6)
			{
				ShowResult();
			}
		}
	}
	ShowResult();
}
