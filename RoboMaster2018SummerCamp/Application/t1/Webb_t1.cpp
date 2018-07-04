#include <cstdio>
#include <cmath>
#include <algorithm>

using namespace std;

const int INF = 0x3f3f3f3f;
const int MAP_MAX = 8, NODE_MIN = 5, NODE_MAX = 15, NODE_WEIGHT_MAX = 6;              //�������ֺ�data_rand.cppһ��

struct node
{
	int x, y;
}node[NODE_MAX + 1] = {7, 0};

int dis[NODE_MAX + 1][NODE_MAX + 1];                //�ڽӾ���
int dp[1<<NODE_MAX][NODE_MAX];
int t, n, ans = INF;

int main()
{
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
		{
			scanf("%d", &t);
			if (t)		node[++n].x = i, node[n].y = j;
		}
	for (int i = 0; i <= n; i++)
		for (int j = 0; j <= n; j++)
   			dis[i][j] = abs(node[i].x - node[j].x) + abs(node[i].y - node[j].y);

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

	for (int i = 1;i <= n; i++)
		ans = min(dp[(1<<n) - 1][i], ans);
	printf("%d", ans);
	return 0;
}
