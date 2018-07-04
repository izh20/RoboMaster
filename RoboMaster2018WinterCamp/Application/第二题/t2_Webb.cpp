//��û�ö�����Ż������ʻῼ��дһ�������в���
//��������汾Ҫ���ͼ�ĳ�����ͬ

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

struct map_node         //��ͼ��ÿ�����
{
    int y,x;
    int weigt;
    int f,g,h;
    struct map_node *father;
};

struct enemy            //���˵Ĳ���
{
	int x, y, range;
}enemy[ENEMY_MAX];

struct note_map_init            // ������Ȼ���ͼʱ�õģ������Ǹ����е���������
{
	int x, y, s;
};

int map_x, map_y, w_x, w_y, n, begin_x, begin_y, enemy_base_x, enemy_base_y;      	   //��Щ������д��ע��
struct map_node map[MAP_MAX][MAP_MAX];    		  //��ά�����ͼ

struct map_node open_list[MAP_MAX*MAP_MAX];    	 	 //Astar�Ŀ����б�
int num_open_list = 1;      		//�����б��е�Ԫ������
struct map_node close_list[MAP_MAX*MAP_MAX];   		  //����б�
int num_close_list;

struct map_node ender;
struct map_node end_path[MAP_MAX*MAP_MAX];      		  //�ҳ�����·
int num_end_path, minCost;

struct map_node temp;
struct map_node itera;          //������������F��С�ĸ���
int tx, ty, next_direction[4][2] = {   {0,1},     		  //�ƶ�������һ��for��ÿ�μ�һȻ������ĸ�����
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

	scanf("%d%d", &map_y, &map_x);			//map_y*map_x�ĵ�ͼ
	scanf("%d%d", &w_y, &w_x);			//�����������ڵ�w_y�ţ���w_x��  ��w_y,w_x��     ��Ϊ�ҽ�w��hhhh
	scanf("%d", &n);		//�з���n��������
	for (int i=1; i<=n; i++)
		scanf("%d%d%d", &enemy[i].y, &enemy[i].x, &enemy[i].range);		//��enemy.y,enemy.x����һ���з������ˣ�������ΧΪenemy.range

	ender.x = enemy_base_x = map_x - 1;			ender.y = enemy_base_y = 0;				//�з����ع̶��ڵ�һ�У����һ�У������Ͻǣ�
	itera.y = w_y;				itera.x = w_x;      //�Ȱѵ������ŵ�Դ��
	itera.weigt = map[w_y][w_x].weigt;
	itera.g = 0;
	itera.h = (abs(enemy_base_y - itera.y) + abs(enemy_base_x - itera.x)) * 10;        //��̽������ǰ����յ�������پ��룬Ҳ��������ֵ
	itera.f = itera.g + itera.h;
	itera.father = NULL;

	//������Ȼ���ͼ����ʱ��ÿ������Ȩֵ�Ǹ��Ǵ�����Ҳ�����ߵ����Ķ������
	for (int k=1; k<=n; k++)
	{
		if (enemy[k].range == 0)
		{
			map[enemy[k].y][enemy[k].x].weigt++;
			continue;
		}
		map_init(k);
	}

	//��ÿ�����ӵ����������Լ������ꡣȨֵ��һ����ÿ��һ���Ĵ��ۼ����ˣ���ʱ��Ȩֵ���ߵ�ÿ����Ҫ�Ĵ��ۡ������貹����������Դ���Ȳ��ܵ���ĵط�Ϊ0����Ȼ��������Ҫ�õ���������������������
	for (int j=0; j<map_y; j++)
		for (int i=0; i<map_x; i++)
		{
			map[i][j].y = i;    	map[i][j].x = j;
            map[i][j].weigt++;
		}

	//��ʼAstar
	open_list[0] = itera;    	   //Դ��ŵ������б���ǰ��
	while (num_open_list && b_close_list(ender)==0)           //�����б���˻��յ��ڷ���б��У�����Ѱ·����
    {
		int min_mark = 0;
		//ȡ�������б���f��С�ĵ㣬��Ϊ��ǰ��
	    for (int i=0; i < num_open_list-1; i++)
	        if (open_list[i].f < open_list[min_mark].f)
	        {
	            min_mark = i;
	        }
	    temp = open_list[num_open_list-1];
		itera = open_list[num_open_list-1] = open_list[min_mark];
		open_list[min_mark] = temp;

        rm_open_list(itera);            //�ӿ����б�ɾ����ǰ��
		close_list[num_close_list] = itera;         //���ӵ�����б���
        num_close_list++;

		//ÿ������ĸ��ھ�
		for (int d=0; d<=3; d++)
		{
			tx = itera.x + next_direction[d][0];
			ty = itera.y + next_direction[d][1];
			if (ty>=0 && ty<map_y && tx>=0 && tx<map_x)
	            if (map[ty][tx].weigt && b_close_list(map[ty][tx])==0)          //ֻ��鲻�ڷ���б��ҿ����ߵĵ�
					if(b_open_list(map[ty][tx]))            //��չ������������Ѿ��ڿ����б���
					{
		                if (itera.g+10 < map[ty][tx].g)
		                {
		                    map[ty][tx].father = &map[itera.y][itera.x];
		                    if (ty != enemy_base_y && tx != enemy_base_x)
		                    	map[ty][tx].g = map[itera.y][itera.x].g + map[ty][tx].weigt * 10;         //��ǰ�㵽���׽ڵ�ľ���
		                    else
								map[ty][tx].g = map[itera.y][itera.x].g;
		                    map[ty][tx].h = (abs(enemy_base_y - itera.y) + abs(enemy_base_x - itera.x)) * 5;       //����ֵ����������ڵ�testdata.in��5�����·��������û�ԡ����ʸļ�������ֵ�ĳ������аɣ�
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
    
	//���յ���Դ����������·
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


//���������Ϣת��Ϊ��ͼ����ά���飩�����ӣ��Ҹ���ȡ�˸����������ֽй�����Ȼ���ͼ
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
        head++;			//ÿһ��head��չ�ĸ������ϵĸ��ӣ�Ȼ��������һ�������ƶ�
	}
	for (int j=0; j<map_y; j++)
		for (int i=0; i<map_x; i++)
			if (book[j][i]!=0)
				map[j][i].weigt++;
	return;
}

//�ѿ����б��еĵ�ǰ�ڵ�ɾ��
inline void rm_open_list(struct map_node itera)
{
	int i = --num_open_list;
	while (i>=0 && open_list[i].y!=itera.y && open_list[i].x!=itera.x)
		i--;
    for (int j=i; j<num_open_list; j++)
        open_list[j] = open_list[j+1];
    return;
}

//���������б����Ƿ�������ھ�
inline int b_open_list(struct map_node neighbour)
{
    for (int i = 0; i < num_open_list - 1; i++)
        if (open_list[i].y == neighbour.y && open_list[i].x == neighbour.x)
            return 1;
    return 0;
}

//��������б����Ƿ���temp
inline int b_close_list(struct map_node temp)
{
    for (int i = 0; i < num_close_list-1; i++)
        if (close_list[i].y == temp.y &&close_list[i].x == temp.x)
            return 1;
    return 0;
}
