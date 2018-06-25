#pragma once
#include"stdafx.h"
#include "DataSet.h"
enum InitRule{LFT,LST,SLK};
class Schedule
{
	vector<int> code;
	bool parallel;
	bool backward;
	int decodevalue = -1;
	int InitRuleValue(int type, int jobnum);
	
	DataSet* dataset;
	bool Swapble(const int& a, const int& b);
	bool JobPutable(const vector<int>& sj, const vector<int>& qj);
	int SerialDecode();
	int ParallelDecode();
public:
	void RandomInit();
	Schedule();
	Schedule(DataSet& data);
	~Schedule();
	int Decode();
	Schedule Crossover(Schedule recevier);
	Schedule Mutation();
};

