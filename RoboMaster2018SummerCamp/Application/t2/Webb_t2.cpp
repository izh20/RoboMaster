#include <cstdio>
#include <cmath>
#include <algorithm>

using namespace std;

const double INF = 0x3f3f3f3f;
const int MAP_MAX = 8, NODE_MIN = 5, NODE_MAX = 15, NODE_WEIGHT_MAX = 6;              //�������ֺ�data_rand.cppһ��

struct node
{
	int x, y;
}node[NODE_MAX + 1] = {7, 0};

int map[MAP_MAX + 1][MAP_MAX + 1];              //����ĵ�ͼ
double dis[NODE_MAX + 1][NODE_MAX + 1];                //�ڽӾ���
double dp[1<<NODE_MAX][NODE_MAX];
double ans = INF;

int main()
{
	int n = 0;
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
		{
			scanf("%d", &map[i][j]);
			if (map[i][j])		node[++n].x = i, node[n].y = j;
		}
	map[7][0] = 1;              //���ױ����Եĳ��ٶ�...

	for (int i = 0; i <= n; i++)
		for (int j = 0; j <= n; j++)
			dis[i][j] = (double)(abs(node[i].x - node[j].x) + abs(node[i].y - node[j].y)) / (double)map[node[i].x][node[i].y];
			
	for (int k = 0; k <= n; k++)
		for (int i = 0; i <= n; i++)
			for (int j = 0; j <= n; j++)
				dis[i][j] = min(dis[i][k] + dis[k][j], dis[i][j]);

	for (int s = 0; s < (1<<n); s++)                //ö�����е�״̬(״̬����2^(n)-1), ��iλ�����i+1����. 1������ʹ�
		for (int i = 1; i <= n; i++)
			if (s & (1 << (i-1)))
			{
				if (s == (1 << (i-1)))		dp[s][i] = dis[0][i];               //״̬s��ֻ�߹�����i
				else
				{
					dp[s][i] = INF;             //�����������
					for (int j = 1; j <= n; j++)
						if ((i != j) && (s & (1 << (j-1))))             //ö�ٲ��ǳ���i����������
							dp[s][i] = min(dp[s][i], dp[s^(1 << (i-1))][j] + dis[j][i]);                //��û��������i��״̬��Ѱ�Һ��ʵ��м��jʹ�þ������
				}
			}
	for (int i = 1; i <= n; i++)
		ans = min(dp[(1<<n) - 1][i], ans);
	printf("%lf", ans);
	return 0;
}
