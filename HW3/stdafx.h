// stdafx.h : 可在此標頭檔中包含標準的系統 Include 檔，
// 或是經常使用卻很少變更的
// 專案專用 Include 檔案
//

#pragma once

#include "targetver.h"
#include<random>
#include<array>
#include<functional>
#include<ctime>
#include<cstdlib>
#include<algorithm>
#include<iostream>
#include<string>
using namespace std;
static default_random_engine e{ static_cast<long unsigned int>(time(nullptr)) };
static uniform_real_distribution<> _d;
static auto random = bind(_d, e);

