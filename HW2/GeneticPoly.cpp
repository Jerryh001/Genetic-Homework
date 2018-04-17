#include "stdafx.h"
#include "GeneticPoly.h"

void GeneticPoly::OnePointCross(Data & a, Data & b)
{
	uniform_int_distribution<uint32_t> rand_pos(1, 11 - 1);
	for (int i = 0; i < 2; i++)
	{
		uint32_t pos = rand_pos(generator);
		uint32_t mask = (1 << pos) - 1;
		uint32_t temp = a.x[i] & mask;
		a.x[i] = (a.x[i] - temp) | b.x[i] & mask;
		b.x[i] = b.x[i] & (~mask) + temp;
	}
}

void GeneticPoly::Run()
{
	start_time = clock();
	Initial();
	Evaluate();
	while (!IsTerminate())
	{
		turn++;
		Select();
		Crossover();
		Mutation();
		Evaluate();
	}
	ShowResult();
}
void GeneticPoly::Initial()
{
	uniform_int_distribution<uint32_t> distribution(0, (1 << 11) - 1);
	for (Data& d : data)
	{
		d.x[0] = distribution(generator);
		d.x[1] = distribution(generator);
	}
}

void GeneticPoly::Evaluate()
{
	for (Data& d : data)
	{
		Eval(d);
	}
	sort(data.begin(), data.end(), [&](Data a, Data b) {return a.value > b.value; });
	if (turn == 0 || best.value > data.back().value)
	{
		best = data.back();
		if (abs(best.value - 0.3979) < 1e-6)
		{
			ShowResult();
		}
	}
}

void GeneticPoly::Select()
{
	array<double, population> score;
	double cent = (data.front().value - data.back().value) / 99;
	double basevalue = data.front().value + cent;
	if (cent < 1e-9)
	{
		for (int i = 0; i < population; i++)
		{
			score[i] = i + 1;
		}
	}
	else
	{
		score[0] = 1;
		for (int i = 1; i < population; i++)
		{
			score[i] = (basevalue - data[i].value) / cent + score[i - 1];
		}
	}

	uniform_real_distribution<double> distribution(0, score.back());
	array<Data, population> temp;
	for (int i = 0; i < population; i++)
	{
		double roll = distribution(generator);
		for (int j = 0; j < population; j++)
		{
			if (score[j] >= roll)
			{
				temp[i] = data[j];
				break;
			}
		}
	}
	data = temp;
}


void GeneticPoly::Crossover()
{
	random_shuffle(data.begin(), data.end());
	uniform_real_distribution<double> rand_cross(0, 1);

	for (int i = 0; i < population; i += 2)
	{
		if (rand_cross(generator) > Pc) continue;
		OnePointCross(data[i], data[i + 1]);
	}
}

void GeneticPoly::Mutation()
{
	uniform_real_distribution<double> rand_mut(0, 1);
	for (Data& d : data)
	{
		for (uint32_t& x : d.x)
		{
			uint32_t mask = 1;
			for (int i = 0; i < 11; i++)
			{
				if (rand_mut(generator) <= Pm)
				{
					x ^= mask;
				}
				mask <<= 1;
			}
		}
	}
}

bool GeneticPoly::IsTerminate()
{
	return turn >= 10000;
}

GeneticPoly::GeneticPoly()
{
}


GeneticPoly::~GeneticPoly()
{
}

void GeneticPoly::ShowResult()
{
	cout << "turn=" << turn << endl
		<< "x1=" << BinToDouble(best.x[0], 11, -5, 10) << endl
		<< "x2=" << BinToDouble(best.x[1], 11, 0, 15) << endl
		<< "min=" << best.value << endl
		<< "time=" << (clock() - start_time)*1.0 / CLOCKS_PER_SEC << endl << endl;

}
