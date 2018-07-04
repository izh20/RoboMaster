#include <cstdio> 
#include <cstdlib>
#include <iostream>
#include <string>
#include <queue>
#include <algorithm>

const int MAP_MAX = 40;
const int ENEMY_MAX = 40;
const int ENEMY_RANGE_MAX = 10;

struct enemy
{
	int x, y, range;
}enemy[ENEMY_MAX];

struct note_map_init
{
	int x,y,s;
};

int map_x, map_y, w_x, w_y, n, begin_x, begin_y;
int map[MAP_MAX][MAP_MAX];
int enemy_base_x, enemy_base_y;
int next[4][2] = { {0,1},
				   {1,0},
				   {0,-1},
				   {-1,0} };

//将输入的信息转换为地图（二维数组）的样子
inline void map_init(int k)
{
	int tx, ty, mark;
	int book[MAP_MAX][MAP_MAX] = {0}; 
	struct note_map_init que[MAP_MAX*MAP_MAX];
	int head=1, tail=1;
	
	que[tail].x = enemy[k].x;		que[tail].y = enemy[k].y;
	que[tail].s = 0;		book[enemy[k].y][enemy[k].x] = 1;
	tail++;
	
	while (head < tail)
	{
		for (int d=0; d<=3; d++)
		{
			tx = que[head].x + next[d][0];
			ty = que[head].y + next[d][1];
			if(book[ty][tx]!=0 || tx<0 || tx>=map_x || ty<0 || ty>=map_y)
                continue;
            que[tail].s = que[head].s + 1;
            if (que[tail].s > enemy[k].range)
            {
            	mark = 1;
            	break;
			}
			que[tail].x = tx;		que[tail].y = ty;
            book[ty][tx] = 1;
            tail++;
		}
		if (mark == 1)	break;
        head++;//每一个head扩展四个方向上的点，（四个点），然后再向下一个点移动
	}
	for (int j=0; j<map_y; j++)
		for (int i=0; i<map_x; i++)
			if (book[j][i]!=0)
				map[j][i]++;
	return; 
}

inline int t1_bfs()
{
	struct note_map_init que[MAP_MAX*MAP_MAX];
	int book[MAP_MAX][MAP_MAX] = {0};
	int head = 1, tail = 1, mark = 0, tx, ty;
	que[tail].x = w_x;      que[tail].y = w_y;
	que[tail].s = 0;        book[w_y][w_x] = 1;
	tail++;
	
	while (head < tail)
	{
		for (int d=0; d<=3; d++)
		{
			tx = que[head].x + next[d][0];
			ty = que[head].y + next[d][1];
			if(map[ty][tx]!=0 || book[ty][tx]!=0 || tx<0 || tx>=map_x || ty<0 || ty>=map_y)
                continue;
			if ((ty == enemy_base_y) && (tx == enemy_base_x))
			{
				mark = 1;
				return que[head].s + 1;
			}
            que[tail].s = que[head].s + 1;
			que[tail].x = tx;		que[tail].y = ty;
            book[ty][tx] = 1;
            tail++;
		}
		if (mark != 0)
			break;
		head++;
	}
	return -1;
}

int main()
{
	freopen("testdata.in","r",stdin);
	freopen("testdata.out","w",stdout);
	
	int t1_step;
	
	//INPUT
	scanf("%d%d", &map_y, &map_x);		//map_y*map_x的方格
	scanf("%d%d", &w_y, &w_x);		//己方机器人在第w_y排，第w_x列  （w_y,w_x）
	scanf("%d", &n);		//敌方有n个机器人
	for (int i=1; i<=n; i++)
		scanf("%d%d%d", &enemy[i].y, &enemy[i].x, &enemy[i].range);		//（enemy.y,enemy.x）有一个敌方机器人，攻击范围为enemy.range
	
	
	//initialise map
	enemy_base_x = map_x - 1;		enemy_base_y = 0;		//敌方基地固定在第一行，最后一列（即右上角）
	
	//测试用
	map[enemy_base_y][enemy_base_x] = 999;
	map[w_y][w_x] = 88;
	for (int i=1; i<=n; i++)
	{
		map[enemy[i].y][enemy[i].x] = enemy[i].range;
	}
	for (int j=0; j<map_y; j++)
	{
		for (int i=0; i<map_x; i++)
			printf("%3d",map[j][i]);
		printf("\n");
	}printf("\n\n");
	for (int j=0; j<map_y; j++)
		for (int i=0; i<map_x; i++)
			map[j][i] = 0;


	for (int k=1; k<=n; k++)
	{
		if (enemy[k].range == 0)
		{
			map[enemy[k].y][enemy[k].x]++;
			continue;
		}
		map_init(k);
	}

	t1_step = t1_bfs();
	
//	for (int j=0; j<map_y; j++)
//	{
//		for (int i=0; i<map_x; i++)
//			printf("%3d",map[j][i]);
//		printf("\n");
//	}
	printf("%d",t1_step);
	return 0;
}   // 输出：minCost  
