#pragma once
#include "GeneticBase.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <array>
#include<random>
#include<ctime>
using namespace std;
struct Data
{
	array<uint32_t, 2> x;
	double value;
};
class GeneticPoly :
	public GeneticBase
{
private:
	default_random_engine generator{ static_cast<long unsigned int>(time(0)) };
	static const int population = 10;
	Data best;
	bool bestupdated = false;
	array<Data, population> data;
	double EvaluatePoly(const double& x1, const double& x2);//11,11
	double BinToDouble(const uint32_t&, const int& bits, const double& lower, const double& upper);
	uint32_t DoubleToBin(const double&, const int& bits, const double& lower, const double& upper);
	void OnePointCross(Data& a, Data& b);
protected:
	virtual void Initial();
	virtual void Evaluate();
	virtual void Select();
	virtual void Alter();
	void Crossver();
	void Mutation();
	virtual bool IsTerminate();
public:
	GeneticPoly();
	~GeneticPoly();
	virtual void ShowResult();
};

