#include "stdafx.h"
#include "DataSet.h"


void DataSet::ReadFile()
{
	ifstream in(file);
	string s;
	int trash;

	int job_size;
	int res_size;

	SkipAfter(in, "RESOURCES");
	while(!isdigit(in.peek()))
	{
		in.get();
	}
	in >> res_size;//TODO:?

	SkipAfter(in, "PROJECT INFORMATION:");
	SkipAfter(in);///skip one line
	in >> trash >> job_size;

	SkipAfter(in, "PRECEDENCE RELATIONS:");
	SkipAfter(in);///skip one line
	for (int i = 1; i <= job_size + 2; i++)
	{
		Job job;
		int succ_size;
		in >> job.ID >> trash >> succ_size;
		for (int j = 0; j < succ_size; j++)
		{
			int sID;
			in >> sID;
			job.successor.insert(sID);
		}
		this->job.push_back(job);
	}
	SkipAfter(in, R"(REQUESTS/DURATIONS:)");
	SkipAfter(in);///skip one line
	SkipAfter(in);///skip one line
	for (int i = 1; i <= job.size(); i++)
	{
		int num, duration, cost;
		in >> num >> trash >> duration;
		Job& J = job[num - 1];
		J.duration = duration;
		for (int j = 0; j < res_size; j++)
		{
			in >> cost;
			J.resource_cost.push_back(cost);
		}
	}
	SkipAfter(in, "RESOURCEAVAILABILITIES:");
	SkipAfter(in);///skip one line
	for (int i = 0; i < res_size; i++)
	{
		int cap;
		in >> cap;
		resource.push_back(cap);
	}
	job.front().resource_cost = resource;
	job.back().resource_cost = resource;
	vector<vector<vector<int>>> matrix(job.size(), vector<vector<int>>(job.size(), vector<int>(res_size)));
	for (int i = 1; i <= resource.size(); i++)
	{
		SkipAfter(in, "TRANSFERTIMES R "+to_string(i)+":");
		SkipAfter(in);///skip one line
		SkipAfter(in);///skip one line
		//vector<vector<int>> matrix;
		
		for (int a = 1; a <= job.size(); a++)
		{
			in >> trash;
			for (int b = 1; b <= job.size(); b++)
			{
				int t;
				in >> matrix[a-1][b-1][i-1];
			}
		}
	}
	transfer_matrix = matrix;
}

void DataSet::CalcTime()
{
	queue<int> que;
	que.push(1);
	job[0].layer = 1;
	while (!que.empty())
	{
		Job& head = job[que.front()-1];
		que.pop();
		for (auto& i : head.successor)
		{
			if (job[i - 1].layer <= head.layer)
			{
				job[i - 1].layer = head.layer + 1;
				que.push(i);
			}
		}
	}
	vector<int> order;
	for (int i = 1; i <= job.size(); i++)
	{
		order.emplace_back(i);
	}
	sort(order.begin(), order.end(), [this](int a, int b) {return job[a - 1].layer < job[b - 1].layer; });
	for (int i = 0; i < order.size(); i++)
	{
		Job& now = job[order[i]-1];
		now.EFT = now.EST + now.duration;
		for (auto& si : now.successor)
		{
			Job& s = job[si - 1];
			if (s.EST < now.EFT)
			{
				s.EST = now.EFT;
			}
		}
	}
	for (int i = order.size() - 1; i >= 0; i--)
	{
		Job& now = job[order[i] - 1];
		if (now.successor.empty())
		{
			now.LFT = now.EFT;
			now.LST = now.EST;
			now.SLK = 0;
		}
		else
		{
			int small = INT_MAX;
			for (auto& si : now.successor)
			{
				Job& s = job[si - 1];
				if (s.LST < small)
				{
					small = s.LST;
				}
			}
			now.LFT = small;
			now.LST = now.LFT - now.duration;
			now.SLK = now.LST - now.EST;
		}
	}
}

void DataSet::RelateCreate()
{
	for (Job& j : job)///parent
	{
		for (auto& si : j.successor)
		{
			job[si - 1].parent.insert(j.ID);
		}
	}
	vector<int> order;
	for (int i = 1; i <= job.size(); i++)
	{
		order.emplace_back(i);
	}
	sort(order.begin(), order.end(), [this](int a, int b) {return job[a - 1].layer < job[b - 1].layer; });
	for (int i = 0; i < order.size(); i++)///ancestor
	{
		Job& j = job[order[i] - 1];
		for (auto& pi : j.parent)
		{
			Job& jp = job[pi - 1];
			j.ancestor.insert(jp.ancestor.begin(),jp.ancestor.end());
			j.ancestor.insert(pi);
		}
	}
	for (int i = order.size()-1; i >=0; i--)///descendant
	{
		Job& j = job[order[i] - 1];
		for (auto& si : j.successor)
		{
			Job& js = job[si - 1];
			j.descendant.insert(js.descendant.begin(), js.descendant.end());
			j.descendant.insert(si);
		}
	}
}

void DataSet::SkipAfter(istream & in, const string & s)
{
	string str;
	do
	{
		getline(in, str);
	} while (str != s&&s!="");
}

DataSet::DataSet(string filepath):file(filepath)
{
	ReadFile();
	CalcTime();
	RelateCreate();
}


DataSet::~DataSet()
{

}
