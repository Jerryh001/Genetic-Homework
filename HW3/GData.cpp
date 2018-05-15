#include "stdafx.h"
#include "GData.h"


void GData::ConvertToValue()
{
	double sum_x1 = 0;
	for (double& xi : x[0])
	{
		sum_x1 += xi;
	}
	value[0] = round(sum_x1 * 0.6*1000000)/1000000;

	double x2_max = min(value[0], 6-value[0]) / sqrt(3);
	double sum_x2 = 0;
	for (double& xi : x[1])
	{
		sum_x2 += xi;
	}
	value[1] = round(sum_x2 * x2_max / 10*1000000)/1000000;
}

void GData::CalculateFitness()
{
	if (value[0] < 2)
	{
		fitness = value[1] + pow(value[1] - value[0], 2) / 100000 - 1;
	}
	else if (value[0] < 4)
	{
		fitness = (pow(value[0] - 3, 2) - 9)*pow(value[1], 3) / (27 * sqrt(3));
	}
	else
	{
		fitness = (pow(value[0] - 2, 3) - 11) / 3 + value[1];
	}
	fitness = round(fitness * 1000000) / 1000000;
}

GData::GData()
{
	for (int i=0;i<2;i++)
	{
		GeneInit(i);
	}
}

void GData::Evaluate()
{
	ConvertToValue();
	CalculateFitness();
}


double GData::GetValue(unsigned int index)
{
	return value[index];
}

double GData::GetFitness()
{
	return fitness;
}



void GData::Mutation(const double& p)
{
	for (int i = 0; i < 2; i++)
	{
		if (random() <= p)
		{
			GeneInit(i);
		}
	}
	
}

void GData::GeneInit(int index, double offset)
{
	double value = random() * 10;
	x[index].fill(0);
	while (value > 0)
	{
		for (auto& xx : x[index])
		{
			double rand = random()*(1 - xx);
			if (rand >= value)
			{
				rand = value;
				xx += rand;
				value = 0;
				break;
			}
			else
			{
				xx += rand;
				value -= rand;
			}

		}
	}
}

void Cross(GData & a, GData & b, const double& p)
{
	if (random() <= p)
	{
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				if (random() <= 0.5)
				{
					swap(a.x[i][j], b.x[i][j]);
				}
			}
		}
	}
}
