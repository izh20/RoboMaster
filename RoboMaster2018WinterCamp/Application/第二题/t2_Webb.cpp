//还没用二叉堆优化，请问会考完写一个补发行不行
//现在这个版本要求地图的长宽相同

#include <cstdio> 
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <string>
#include <cstring>
#include <queue>
#include <algorithm>
using namespace std;

const int MAP_MAX = 40;
const int ENEMY_MAX = 20;
const int ENEMY_RANGE_MAX = 10;

struct map_node         //地图的每个格格
{
    int y,x;
    int weigt;
    int f,g,h;
    struct map_node *father;
};

struct enemy            //敌人的步兵
{
	int x, y, range;
}enemy[ENEMY_MAX];

struct note_map_init            // 广度优先画地图时用的，就是那个队列的数据类型
{
	int x, y, s;
};

int map_x, map_y, w_x, w_y, n, begin_x, begin_y, enemy_base_x, enemy_base_y;      	   //这些在下面写的注释
struct map_node map[MAP_MAX][MAP_MAX];    		  //二维矩阵地图

struct map_node open_list[MAP_MAX*MAP_MAX];    	 	 //Astar的开放列表
int num_open_list = 1;      		//开放列表中的元素数量
struct map_node close_list[MAP_MAX*MAP_MAX];   		  //封闭列表
int num_close_list;

struct map_node ender;
struct map_node end_path[MAP_MAX*MAP_MAX];      		  //找出来的路
int num_end_path, minCost;

struct map_node temp;
struct map_node itera;          //迭代器，保存F最小的格子
int tx, ty, next_direction[4][2] = {   {0,1},     		  //移动，就是一个for，每次加一然后就是四个方向
									   {1,0},
									   {0,-1},
									   {-1,0} };

extern inline void map_init(int k);
extern inline void rm_open_list(struct map_node itera);
extern inline int b_close_list(struct map_node temp);
extern inline int b_open_list(struct map_node neighbour);

int main()
{
	freopen("testdata.in","r",stdin);
	freopen("testdata.out","w",stdout);

	scanf("%d%d", &map_y, &map_x);			//map_y*map_x的地图
	scanf("%d%d", &w_y, &w_x);			//己方机器人在第w_y排，第w_x列  （w_y,w_x）     因为我叫w啊hhhh
	scanf("%d", &n);		//敌方有n个机器人
	for (int i=1; i<=n; i++)
		scanf("%d%d%d", &enemy[i].y, &enemy[i].x, &enemy[i].range);		//（enemy.y,enemy.x）有一个敌方机器人，攻击范围为enemy.range

	ender.x = enemy_base_x = map_x - 1;			ender.y = enemy_base_y = 0;				//敌方基地固定在第一行，最后一列（即右上角）
	itera.y = w_y;				itera.x = w_x;      //先把迭代器放到源点
	itera.weigt = map[w_y][w_x].weigt;
	itera.g = 0;
	itera.h = (abs(enemy_base_y - itera.y) + abs(enemy_base_x - itera.x)) * 10;        //试探法，求当前点和终点的曼哈顿距离，也就是启发值
	itera.f = itera.g + itera.h;
	itera.father = NULL;

	//广度优先画地图。这时候每个格格的权值是覆盖次数，也就是走到这格的额外代价
	for (int k=1; k<=n; k++)
	{
		if (enemy[k].range == 0)
		{
			map[enemy[k].y][enemy[k].x].weigt++;
			continue;
		}
		map_init(k);
	}

	//让每个格子的坐标是他自己的坐标。权值加一，把每走一步的代价加上了，这时的权值是走到每格需要的代价。可以设补给禁区，资源岛等不能到达的地方为0。虽然根本不需要用到哈哈哈哈哈哈哈哈哈哈
	for (int j=0; j<map_y; j++)
		for (int i=0; i<map_x; i++)
		{
			map[i][j].y = i;    	map[i][j].x = j;
            map[i][j].weigt++;
		}

	//开始Astar
	open_list[0] = itera;    	   //源点放到开放列表最前面
	while (num_open_list && b_close_list(ender)==0)           //开放列表空了或终点在封闭列表中，代表寻路结束
    {
		int min_mark = 0;
		//取出开放列表中f最小的点，作为当前点
	    for (int i=0; i < num_open_list-1; i++)
	        if (open_list[i].f < open_list[min_mark].f)
	        {
	            min_mark = i;
	        }
	    temp = open_list[num_open_list-1];
		itera = open_list[num_open_list-1] = open_list[min_mark];
		open_list[min_mark] = temp;

        rm_open_list(itera);            //从开放列表删除当前点
		close_list[num_close_list] = itera;         //并加到封闭列表中
        num_close_list++;

		//每个点的四个邻居
		for (int d=0; d<=3; d++)
		{
			tx = itera.x + next_direction[d][0];
			ty = itera.y + next_direction[d][1];
			if (ty>=0 && ty<map_y && tx>=0 && tx<map_x)
	            if (map[ty][tx].weigt && b_close_list(map[ty][tx])==0)          //只检查不在封闭列表并且可以走的点
					if(b_open_list(map[ty][tx]))            //扩展到的这个格子已经在开放列表中
					{
		                if (itera.g+10 < map[ty][tx].g)
		                {
		                    map[ty][tx].father = &map[itera.y][itera.x];
		                    if (ty != enemy_base_y && tx != enemy_base_x)
		                    	map[ty][tx].g = map[itera.y][itera.x].g + map[ty][tx].weigt * 10;         //求当前点到父亲节点的距离
		                    else
								map[ty][tx].g = map[itera.y][itera.x].g;
		                    map[ty][tx].h = (abs(enemy_base_y - itera.y) + abs(enemy_base_x - itera.x)) * 5;       //启发值，这个我现在的testdata.in设5是最佳路径，其他没试。请问改计算启发值的常数就行吧？
		                    map[ty][tx].f = map[ty][tx].g + map[ty][tx].h;
					  		rm_open_list(map[ty][tx]);
							open_list[num_open_list] = map[ty][tx];
	    					num_open_list++;
				        }
					}
		            else
		            {
		                map[ty][tx].father = &map[itera.y][itera.x];
		                map[ty][tx].g = map[itera.y][itera.x].g + map[ty][tx].weigt * 10;
		                map[ty][tx].h = (abs(enemy_base_y - itera.y) + abs(enemy_base_x - itera.x)) * 5;
		                map[ty][tx].f = map[ty][tx].g + map[ty][tx].h;
	  					open_list[num_open_list] = map[ty][tx];
	    				num_open_list++;
		            }
		}
    }
    
	//从终点向源点整理最后的路
	itera = map[ender.y][ender.x];
    while (1)
    {
        end_path[num_end_path] = itera;
        minCost += itera.weigt;
        num_end_path++;
        if (itera.y == w_y &&itera.x == w_x)
            break;
		itera=map[itera.father->y][itera.father->x];
    }

    for (int flag=0,j=0; j<map_y; j++)
    {
        for (int i=0; i<map_x; i++)
        {
            for (int w=0; w<num_end_path; w++)
                if (end_path[w].y==j && end_path[w].x==i)
                {
                    printf("... ");
					flag = 1;
                    break;
                }
            if (flag == 0)
            	printf("O ");
			flag = 0;
        }
        printf("\n");
    }
    printf("\n%d",minCost);
    return 0;
}


//将输入的信息转换为地图（二维数组）的样子，我给他取了个好听的名字叫广度优先画地图
inline void map_init(int k)
{
	int tx, ty, mark;
	int book[MAP_MAX][MAP_MAX] = {0};
	struct note_map_init que[MAP_MAX*MAP_MAX];
	int head=1, tail=1;

	que[tail].x = enemy[k].x;		que[tail].y = enemy[k].y;
	que[tail].s = 0;
	book[enemy[k].y][enemy[k].x] = 1;
	tail++;

	while (head < tail)
	{
		for (int d=0; d<=3; d++)
		{
			tx = que[head].x + next_direction[d][0];
			ty = que[head].y + next_direction[d][1];
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
        head++;			//每一个head扩展四个方向上的格子，然后再向下一个格子移动
	}
	for (int j=0; j<map_y; j++)
		for (int i=0; i<map_x; i++)
			if (book[j][i]!=0)
				map[j][i].weigt++;
	return;
}

//把开放列表中的当前节点删除
inline void rm_open_list(struct map_node itera)
{
	int i = --num_open_list;
	while (i>=0 && open_list[i].y!=itera.y && open_list[i].x!=itera.x)
		i--;
    for (int j=i; j<num_open_list; j++)
        open_list[j] = open_list[j+1];
    return;
}

//搜索开放列表中是否有这个邻居
inline int b_open_list(struct map_node neighbour)
{
    for (int i = 0; i < num_open_list - 1; i++)
        if (open_list[i].y == neighbour.y && open_list[i].x == neighbour.x)
            return 1;
    return 0;
}

//搜索封闭列表中是否有temp
inline int b_close_list(struct map_node temp)
{
    for (int i = 0; i < num_close_list-1; i++)
        if (close_list[i].y == temp.y &&close_list[i].x == temp.x)
            return 1;
    return 0;
}
