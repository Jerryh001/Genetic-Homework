// main.cpp: 定義主控台應用程式的進入點。
//

#include "stdafx.h"
#include"HillClimbing.h"
#include"SimAnnealing.h"
#include"GeneticPoly.h"
int main()
{
	cout << "Genetic" << endl;
	GeneticPoly g;
	g.Run();
	cout << "HillClimbing" << endl;
	HillClimbing h;
	h.Run();
	cout << "SimAnnealing" << endl;
	SimAnnealing s;
	s.Run();
	system("pause");
    return 0;
}

