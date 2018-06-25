#pragma once
#include"stdafx.h"
struct Job
{
	int ID=0;
	set<int> ancestor;
	set<int> parent;
	set<int> successor;
	set<int> descendant;



	int duration=0;
	vector<int> resource_cost;
	int EST=0, EFT=0, LST=0, LFT=0, SLK=0;
	int layer=0;
};
class DataSet
{
	friend class Schedule;
	string file;
	vector<Job> job;
	vector<vector<vector<int>>> transfer_matrix;
	vector<int> resource;
	vector<vector<int>> layer;
	void ReadFile();
	void CalcTime();
	void RelateCreate();
	void SkipAfter(istream& in, const string& s = string());
public:
	DataSet(string filepath);
	~DataSet();
};

