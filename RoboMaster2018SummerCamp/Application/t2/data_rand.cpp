#include<bits/stdc++.h>
using namespace std;

#define random(a,b) ((a)+rand()%((b)-(a)+1))

stringstream ss;
int seed;

const int MAP_MAX = 8;          	//地图边长都是8
const int NODE_MIN = 5;              //最少有NODE_MIN个权值不为0的点
const int NODE_MAX = 15;
const int NODE_WEIGHT_MAX = 6;              //每个点权值最大为

int map[MAP_MAX * MAP_MAX + 1], mark[MAP_MAX * MAP_MAX + 1];
int node_num;

int main( int argc, char *argv[] )
{
    int seed=time(NULL);
    if(argc > 1)
    {
        ss.clear();
        ss<<argv[1];
        ss>>seed;
    }
    srand(seed);

	node_num = random(NODE_MIN, NODE_MAX);
	for (int i=1, t=1; i <= node_num; i++)
	{
		do
		{
			t = random(1, 64);
		}
		while (mark[t] == 1);
		mark[t] = 1;
	}
	
	for (int i = 1; i <= 64; i++)
	{
		if ((mark[i] == 1) && (i != 57))
		{
			printf("%d%c", random(1, NODE_WEIGHT_MAX), " \n"[(i%8)==0]);
		}
		else    printf("0%c", " \n"[(i%8)==0]);
	}

    return 0;
}


//if (mark[i] == 1)
//		{
//			printf("%d", random(1, NODE_WEIGHT_MAX));
//		}
//		else    printf("0");
//		if (i%8)    printf(" ");
//		else printf("\n");
