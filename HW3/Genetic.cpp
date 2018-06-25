#include "stdafx.h"
#include "Genetic.h"


Genetic::Genetic()
{
}


Genetic::~Genetic()
{
}

void Genetic::Run()
{
	Init();
	Evaluate();
	while (!IsEnd())
	{
		turn++;
		Select();
		Cross();
		Mutation();
		Evaluate();
	}
	cout << output << endl;
}

bool Genetic::IsEnd()
{
	return turn >= 1000;
}

void Genetic::Init()
{
	best.Evaluate();
	//nothing to do
}

void Genetic::Evaluate()
{
	for (GData& d : data)
	{
		d.Evaluate();
		if (d.GetFitness() < best.GetFitness())
		{
			output = to_string(turn)+","+to_string(d.GetValue(0))+","+ to_string(d.GetValue(1))+","+ to_string(d.GetFitness());
			best = d;
		}
	}
}

void Genetic::Select()
{
	uniform_int_distribution<> d(0,data.size()-1);
	auto randomi = bind(d, e);
	auto temp=data;
	for (int i = 0; i < data.size(); i++)
	{
		GData& a = data[randomi()];
		GData& b = data[randomi()];
		temp[i] = (a.GetFitness() < b.GetFitness()) ? a : b;
	}
}

void Genetic::Cross()
{
	random_shuffle(data.begin(), data.end());
	for (int i = 0; i < data.size(); i += 2)
	{
		::Cross(data[i], data[i + 1], Pc);
	}
}

void Genetic::Mutation()
{
	for (GData& d : data)
	{
		d.Mutation(Pm);
	}
}
