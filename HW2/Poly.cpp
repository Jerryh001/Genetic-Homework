#include"stdafx.h"
#include "Poly.h"
namespace Poly
{
	void Eval(Data& d)
	{
		double x1 = BinToDouble(d.x[0], 11, -5, 10);
		double x2 = BinToDouble(d.x[1], 11, 0, 15);
		d.value = EvaluatePoly(x1, x2);
	}
	double EvaluatePoly(const double& x1, const double& x2)
	{
		const double a = 1;
		const double b = 5.1 / ((4 * M_PI) *(4 * M_PI));
		const double c = 5 / M_PI;
		const double d = 6;
		const double e = 10;
		const double f = 1 / (8 * M_PI);
		return a * (x2 - b * x1*x1 + c * x1 - d)*(x2 - b * x1*x1 + c * x1 - d) + e * (1 - f)*cos(x1) + e;
	}

	double BinToDouble(const uint32_t & in, const int& bits, const double & lower, const double & upper)
	{
		return round((((upper - lower) / (1 << bits)*(in&((1 << bits) - 1))) + lower) * 100) / 100;
	}

	uint32_t DoubleToBin(const double &in, const int & bits, const double & lower, const double & upper)
	{
		return round((in - lower) / ((upper - lower) / (1 << bits)));
	}
}