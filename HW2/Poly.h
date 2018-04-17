#pragma once
#include"stdafx.h"
namespace Poly
{
	struct Data
	{
		array<uint32_t, 2> x;
		double value;
	};
	void Eval(Data& d);
	double EvaluatePoly(const double& x1, const double& x2);
	double BinToDouble(const uint32_t & in, const int& bits, const double & lower, const double & upper);
	uint32_t DoubleToBin(const double &in, const int & bits, const double & lower, const double & upper);
}