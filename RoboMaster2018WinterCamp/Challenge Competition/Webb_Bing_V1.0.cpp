#include <cstdio>
#include <string>
#include <cstdlib>
#include <cstring>
using namespace std;

int const HIAHIA = 66666666;

struct map_node_t
{
	int RFID_j, RFID_i;
	int block_j, block_i;
	int point_weight;
}map_node[7][7];

int w_j, w_i, w_num;				//机器人当前所在坐标
int dist[37][37];				//邻接矩阵
int book[37][37], mark_block[37];				//flag
int hiahiahia;
int t_j, t_i, t_dist, temp_i, temp_j, temp_num;
int min_dist, min_hmm = HIAHIA, min_hmm_num;

extern inline void map_node_init(void);
extern inline void search_n_print(void);
extern inline void search2(void);

int main()
{
	freopen("testdata.in", "r", stdin);
	freopen("testdata.out", "w", stdout);
	
	printf("#(j1, i1)  -->  (j2, i2)     意思是把(j1, i1)的障碍块放到(j2, i2)的RFID\n");
	printf("# 后面的数字是从机器人当前位置 --> 障碍块位置 -->相应RFID. 完成这一个块的路程 和 完成后目前一共消耗的总路程\n\n");
	map_node_init();

	for (int hmm = 1; hmm <= 36; hmm++)
	{
		w_num = hmm;
		for (int emm = 1; emm <= 36; emm++)
			search2();
		if (hiahiahia < min_hmm)
		{
			min_hmm_num = hmm;
			min_hmm = hiahiahia;
		}
		memset(mark_block, 0, sizeof(mark_block));
		hiahiahia = 0;
	}
	
	w_num = min_hmm_num;
	for (int emm = 1; emm <= 36; emm++)
		search_n_print();
	
	return 0;
}

inline void map_node_init(void)
{
	for (int j = 1; j <= 6; j++)
	{
		for (int i = 1; i <= 6; i++)
		{
			scanf("%d%d", &map_node[j][i].RFID_j, &map_node[j][i].RFID_i);
			map_node[map_node[j][i].RFID_j][map_node[j][i].RFID_i].point_weight = abs(j - map_node[j][i].RFID_j) + abs(i - map_node[j][i].RFID_i);
			map_node[map_node[j][i].RFID_j][map_node[j][i].RFID_i].block_j = j;
			map_node[map_node[j][i].RFID_j][map_node[j][i].RFID_i].block_i = i;
			//printf("(%d,%d)  ", map_node[j][i].RFID_j, map_node[j][i].RFID_i);
		}
		//printf("\n");
	}

	for (int j = 1; j <= 6; j++)
	{
		for (int i = 1; i <= 6; i++)
		{
			printf("(%d,%d %d) ", map_node[j][i].RFID_j, map_node[j][i].RFID_i, map_node[j][i].point_weight);
		}
		printf("\n");
	}
	
	//这里邻接矩阵中每个点互相连线的无向图, 不存在某条边不存在, 所以没有初始化赋无穷 
	int u_j=0, v_j=0, u_i=0, v_i=0, t_j=0, t_i=0;
	for (int u = 1; u <= 36; u++)
		for (int v = 1; v <= 36; v++)
		{
			u_j = (u - 1) / 6 + 1;
			if	(! (u_i = u % 6) ) 			u_i = 6;
			v_j = (v - 1) / 6 + 1;
			if	(! (v_i = v % 6) )			v_i = 6;
			dist[u][v] = abs(u_j - v_j) + abs(u_i - v_i);
		}
	
//  //test
//	for (int j = 1; j <= 36; j++)
//	{ 
//		for (int i = 1; i <= 36; i++)
//			printf("%3d ", dist[j][i]);
//		printf("\n");
//	}

	return;
}

inline void search_n_print(void)
{
	min_dist = HIAHIA;
	for (int i = 1; i <= 36; i++)
	{
		if (mark_block[i] == 0)
		{
			if (! (t_i = i % 6) )		t_i = 6;
			t_j = (i - 1) / 6 + 1;
			t_dist = dist[w_num][i] + map_node[t_j][t_i].point_weight;

			if (t_dist < min_dist)
			{
				min_dist = t_dist;
				temp_num = i;
				temp_i = t_i;		temp_j = t_j;
			}
		}
	}
	mark_block[temp_num] = 1;
	printf("(%2d,%2d)  -->  ", temp_j, temp_i);
	printf("(%2d,%2d)", w_j = map_node[temp_j][temp_i].block_j, w_i = map_node[temp_j][temp_i].block_i);
	w_num = (w_j - 1)*6 + w_i;
	printf("      %d, %d\n", min_dist, hiahiahia += min_dist);
	
	return;
}

inline void search2(void)
{
	min_dist = HIAHIA;
	for (int i = 1; i <= 36; i++)
	{
		if (mark_block[i] == 0)
		{
			if (! (t_i = i % 6) )		t_i = 6;
			t_j = (i - 1) / 6 + 1;
			t_dist = dist[w_num][i] + map_node[t_j][t_i].point_weight;

			if (t_dist < min_dist)
			{
				min_dist = t_dist;
				temp_num = i;
				temp_i = t_i;		temp_j = t_j;
			}
		}
	}
	mark_block[temp_num] = 1;
	w_num = (map_node[temp_j][temp_i].block_j - 1)*6 + map_node[temp_j][temp_i].block_i;
	hiahiahia += min_dist;

	return;
}
