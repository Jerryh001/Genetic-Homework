// HW4.cpp: 定義主控台應用程式的進入點。
//

#include "stdafx.h"
#include "Genetic.h"
int main(int argc,char* argv[])
{
	if (argc > 1)
	{
		Genetic g(argv[1], 200);
		g.Run();
	}
	
	
    return 0;
}

