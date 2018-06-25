#include "stdafx.h"
#include "Schedule.h"



int Schedule::InitRuleValue(int type, int jobnum)
{
	Job& j = dataset->job[jobnum - 1];
	switch (type)
	{
	case LFT:
		return j.LFT;
	case LST:
		return j.LST;
	case SLK:
		return j.SLK;
	}
}

Schedule Schedule::Mutation()
{
	Schedule ans = *this;
	uniform_int_distribution<> di(1, code.size() - 3);
	uniform_real_distribution<> dr;
	double mu = 0.05;
	if (dr(e) < mu)
	{
		int loc = di(e);
		if (Swapble(code[loc], code[loc + 1]))
		{
			swap(ans.code[loc], ans.code[loc + 1]);
		}
	}
	if (dr(e) < mu)
	{
		ans.parallel = !parallel;
	}
	if (dr(e) < mu)
	{
		ans.backward = !backward;
	}
	return ans;
}

bool Schedule::Swapble(const int & front, const int & back)
{
	auto& job = dataset->job;
	auto& des = job[front - 1].descendant;
	return des.find(back-1) == des.end();
}

bool Schedule::JobPutable(const vector<int>& sj, const vector<int>& qj)
{
	for (int k = 0; k < dataset->resource.size(); k++)
	{
		if (sj[k] < qj[k])
		{
			return false;
		}
	}
	return true;
}

int Schedule::SerialDecode()
{
	//set of already scheduled job
	set<int> D = { 1 };
	//FT[i] real finish time of job i
	vector<int> FT(code.size(), 0);
	//u[i][j][k] resouse k transfer from i to j
	vector<vector<vector<int>>> u(code.size(), vector<vector<int>>(code.size(), vector<int>(dataset->resource.size())));
	//v[i][k] 事件i完成後還能使用的資源k的數量
	vector<vector<int>> v(code.size(), vector<int>(dataset->resource.size()));
	v[0] = dataset->resource;
	//Z[j][k] 能提供一些資源k給事件j的集合
	vector<vector<set<int>>> Z(code.size(), vector<set<int>>(dataset->resource.size()));
	//gamma[j][k] 事件i給l的資源k 能夠在其間借給事件j 的(i,l)集合
	vector<vector<set<pair<int, int>>>> gamma(code.size(), vector<set<pair<int, int>>>(dataset->resource.size()));;

	//delta[i][j][k] 資源k從i轉移到j的時間
	const vector<vector<vector<int>>>& delta = dataset->transfer_matrix;

	//s[j][k] 能提供給j第k資源的最大資源數
	vector<vector<int>> s(code.size(), vector<int>(dataset->resource.size()));

	for (int lamda = 1; lamda < dataset->job.size(); lamda++)
	{
		Job& j = dataset->job[code[lamda] - 1];
		int t = 0;
		for (auto& pi : j.parent)
		{
			if (FT[pi] > t)
			{
				t = FT[pi];
			}
		}
		while (1)///j not in D
		{
			for (int k = 0; k < dataset->resource.size(); k++)
			{
				Z[j.ID - 1][k].clear();
				for (int l = 0; l <= lamda - 1; l++)
				{
					Job& i = dataset->job[code[l] - 1];
					gamma[j.ID - 1][k].clear();
					if (v[i.ID - 1][k] > 0 && FT[i.ID - 1] + delta[i.ID - 1][j.ID - 1][k] <= t)
					{
						Z[j.ID - 1][k].insert(i.ID);
					}
					for (int e = 0; e < lamda - 1; e++)
					{
						Job& m = dataset->job[code[e] - 1];
						if (m.ID != i.ID&&u[i.ID - 1][m.ID - 1][k] > 0 && FT[i.ID - 1] + delta[i.ID - 1][j.ID - 1][k] <= t && t <= FT[m.ID - 1] - j.duration - m.duration - delta[j.ID - 1][m.ID - 1][k])
						{
							gamma[j.ID - 1][k].emplace(i.ID, m.ID);
						}
					}
				}
				int& sjk = s[j.ID - 1][k];
				sjk = 0;
				for (auto& i : Z[j.ID - 1][k])
				{
					sjk += v[i - 1][k];
				}
				for (auto& im : gamma[j.ID - 1][k])
				{
					sjk += u[im.first - 1][im.second - 1][k];
				}
			}
			if (JobPutable(s[j.ID - 1], j.resource_cost))
			{
				for (int k = 0; k < dataset->resource.size(); k++)
				{
					vector<pair<int, int>> gammajk_sort(gamma[j.ID - 1][k].begin(), gamma[j.ID - 1][k].end());
					sort(gammajk_sort.begin(), gammajk_sort.end(), [&](auto& ima, auto&  imb) {return u[ima.first - 1][ima.second - 1] > u[imb.first - 1][imb.second - 1]; });
					vector<int> Zjk_sort(Z[j.ID - 1][k].begin(), Z[j.ID - 1][k].end());
					sort(Zjk_sort.begin(), Zjk_sort.end(), [&](auto& ia, auto& ib) {return delta[ia - 1][j.ID - 1][k] < delta[ib - 1][j.ID - 1][k]; });
					for (int l = 0; l < gammajk_sort.size() && v[j.ID - 1][k] < j.resource_cost[k]; l++)
					{
						pair<int, int>& im = gammajk_sort[l];
						int x = min(u[im.first - 1][im.second - 1][k], j.resource_cost[k] - v[j.ID - 1][k]);
						u[im.first - 1][im.second - 1][k] -= x;
						u[im.first - 1][j.ID - 1][k] += x;
						u[j.ID - 1][im.second - 1][k] += x;
						v[j.ID - 1][k] += x;
					}
					for (int e = 0; e < Zjk_sort.size() && v[j.ID - 1][k] < j.resource_cost[k]; e++)
					{
						Job& i = dataset->job[Zjk_sort[e] - 1];
						int x = min(v[i.ID - 1][k], j.resource_cost[k] - v[j.ID - 1][k]);
						v[i.ID - 1][k] -= x;
						u[i.ID - 1][j.ID - 1][k] += x;
						v[j.ID - 1][k] += x;
					}
				}
				D.insert(j.ID);
				FT[j.ID - 1] = t + j.duration;
				break;
			}
			else
			{
				t++;
			}
		}
	}
	sort(FT.begin(), FT.end());
	return FT.back();
}

int Schedule::ParallelDecode()
{
	return 0;
}

void Schedule::RandomInit()
{
	uniform_int_distribution<> d(0, 2);
	vector<int> undone_parent(dataset->job.size(), 0);
	InitRule rule = (InitRule)d(e);
	for (Job& j : dataset->job)
	{
		undone_parent[j.ID - 1] = j.parent.size();
	}
	//code.push_back(1);
	//undone_parent[0] = -1;
	while (1)
	{
		vector<int> selectable;
		for (int i = 1; i <= undone_parent.size(); i++)
		{
			if (undone_parent[i - 1] == 0)
			{
				selectable.push_back(i);
			}
		}
		if (selectable.empty()) break;
		vector<int> value;
		int v_sum = 0;
		int v_max = 0;
		for (int& ji : selectable)
		{
			int val = InitRuleValue(rule, ji);
			value.emplace_back(val);
			if (val > v_max)
			{
				v_max = val;
			}
		}
		for (auto& rj : value)
		{
			rj = 1 + v_max - rj;
			v_sum += rj;
		}
		uniform_int_distribution<> d2(1, v_sum);
		int roll = d2(e);
		for (int i = 0; i < selectable.size(); i++)
		{
			if (roll > value[i])
			{
				roll -= value[i];
			}
			else
			{
				code.push_back(selectable[i]);
				undone_parent[selectable[i] - 1] = -1;
				vector<Job>& J = dataset->job;
				for (auto& si : J[selectable[i] - 1].successor)
				{
					undone_parent[si - 1] --;
				}
				break;
			}
		}
		uniform_int_distribution<> d3(0, 1);
		backward = d3(e);
		parallel = d3(e);
	}
}

Schedule::Schedule()
{
}

Schedule::Schedule(DataSet& data)
{
	dataset = &data;
}

Schedule::~Schedule()
{
}

int Schedule::Decode()
{
	if (decodevalue < 0)
	{
		decodevalue=SerialDecode();
		//decodevalue=parallel ? ParallelDecode() : SerialDecode();
	}
	return decodevalue;
}

Schedule Schedule::Crossover(Schedule recevier)
{
	uniform_int_distribution<> d(1, code.size() - 2);
	vector<vector<int>> segment;
	int n1, n2;
	n1 = d(e);
	do { n2 = d(e); } while (n2 == n1);
	if (n1 > n2) swap(n1, n2);
	vector<int> pattern(code.begin() + n1, code.begin() + n2 + 1);
	int stage = 0;
	int patcount = 0;
	vector<int> remain;
	for (auto it = recevier.code.begin(); it != recevier.code.end(); it++)
	{
		switch (stage)
		{
		case 0:
			if (find(pattern.begin(), pattern.end(), *it) != pattern.end())
			{
				segment.emplace_back(recevier.code.begin(), it);
				patcount++;
				stage++;
			}
			break;
		case 1:
			if (find(pattern.begin(), pattern.end(), *it) == pattern.end())
			{
				remain.push_back(*it);
			}
			else
			{
				patcount++;
			}
			break;
		}
		if (patcount == pattern.size())
		{
			segment.emplace_back(pattern);
			segment.emplace_back(it + 1, recevier.code.end());
			stage = 2;
			break;
		}
	}
	if (stage != 2) throw exception();
	vector<int> parents;
	vector<int> successors;
	vector<int> frees;
	for (int& ji : remain)
	{
		bool isfree = true;
		for (int& pi : pattern)
		{
			if (!Swapble(ji, pi))
			{
				parents.push_back(ji);
				isfree = false;
				break;
			}
			else if (!Swapble(pi, ji))
			{
				successors.push_back(ji);
				isfree = false;
				break;
			}
		}
		if (isfree)
		{
			frees.push_back(ji);
		}
	}
	segment.insert(segment.begin() + 1,parents);
	segment.insert(segment.begin() + 3, successors);
	segment.insert(segment.begin() + 4, frees);
	/*for (int& ji : frees)
	{
		int lower = segment.size() - 1, upper = 1;
		for (int i = 1; i < segment.size() - 1; i++)
		{
			bool unmoveable = false;
			for (auto& v : segment[i])
			{
				if (!Swapble(ji, v))
				{
					unmoveable = true;
					break;
				}
			}
			if (unmoveable)
			{
				break;
			}
			else
			{
				upper++;
			}
		}
		for (int i = segment.size() - 2; i > 0; i--)
		{
			bool unmoveable = false;
			for (auto& v : segment[i])
			{
				if (!Swapble(v, ji))
				{
					unmoveable = true;
					break;
				}
			}
			if (unmoveable)
			{
				break;
			}
			else
			{
				lower--;
			}
		}
		uniform_int_distribution<> d(lower, upper + 1);
		segment.insert(segment.begin() + d(e), vector<int>(1, ji));
	}*/
	Schedule ans = recevier;
	ans.code.clear();
	ans.decodevalue = -1;
	for (auto& s : segment)
	{
		ans.code.insert(ans.code.end(), s.begin(), s.end());
	}
	return ans;
}
