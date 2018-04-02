#include "GeneticBase.h"



GeneticBase::GeneticBase()
{
}


GeneticBase::~GeneticBase()
{
}

void GeneticBase::Run()
{
	Initial();
	Evaluate();
	while (!IsTerminate())
	{
		turn++;
		Select();
		Alter();
		Evaluate();
	}
}
