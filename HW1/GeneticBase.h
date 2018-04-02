#pragma once
class GeneticBase
{
protected:
	long long int turn=0;
	double Pc = 0;
	double Pm = 0;
	virtual void Initial() = 0;
	virtual void Evaluate() = 0;
	virtual void Select() = 0;
	virtual void Alter() = 0;
	virtual bool IsTerminate()=0;
public:
	GeneticBase();
	~GeneticBase();
	void Run();
	virtual void ShowResult() = 0;
};

