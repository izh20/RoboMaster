// Webb Bing 20180721
#include "stdafx.h"

#include <cstdio>
#include <cstdlib>

struct CastleNode {
  int i, j;
} castle_node[17];
int next_direction[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
int map[9][9], map_end[9][9], mark[9][9], c_n, b_end;
void DFS(int i, int j, int color, int b_begin);

int main() {
  std::freopen("testdata.in", "r", stdin);
  for (int i = 1; i <= 8; i++)
    for (int j = 1; j <= 8; j++) {
      scanf("%d", &map[i][j]);
      if (map[i][j]) castle_node[++c_n].i = i, castle_node[c_n].j = j;
    }
  mark[castle_node[1].i][castle_node[1].j] = 1;
  DFS(castle_node[1].i, castle_node[1].j,
      map[castle_node[1].i][castle_node[1].j], 1);
  system("color 71");
  for (int i = 1; i <= 8; i++)
    for (int j = 1; j <= 8; j++)
      if (map[i][j]) {
        switch (map[i][j]) {
          case 1: {
            printf("\033[41;35m%d%c\033[5m", map[i][j], " \n"[j == 8]);
            break;
          }
          case 2: {
            printf("\033[43;35m%d%c\033[5m", map[i][j], " \n"[j == 8]);
            break;
          }
          case 3: {
            printf("\033[44;35m%d%c\033[5m", map[i][j], " \n"[j == 8]);
            break;
          }
          case 4: {
            printf("\033[42;35m%d%c\033[5m", map[i][j], " \n"[j == 8]);
            break;
          }
        }
      } else {
        switch (map_end[i][j]) {
          case 1: {
            printf("\033[41;30m%d%c\033[5m", map_end[i][j], " \n"[j == 8]);
            break;
          }
          case 2: {
            printf("\033[43;30m%d%c\033[5m", map_end[i][j], " \n"[j == 8]);
            break;
          }
          case 3: {
            printf("\033[44;30m%d%c\033[5m", map_end[i][j], " \n"[j == 8]);
            break;
          }
          case 4: {
            printf("\033[42;30m%d%c\033[5m", map_end[i][j], " \n"[j == 8]);
            break;
          }
        }
      }
  return 0;
}

void DFS(int i, int j, int color, int b_begin) {
  int ti, tj;
  if (map[i][j] && b_begin == 0) {
    if (map[i][j] == color) {
      for (int i = 1; i <= 16; i++)
        if (mark[castle_node[i].i][castle_node[i].j] == 0) {
          mark[castle_node[i].i][castle_node[i].j] = 1;
          DFS(castle_node[i].i, castle_node[i].j,
              map[castle_node[i].i][castle_node[i].j], 1);
          mark[castle_node[i].i][castle_node[i].j] = 0;
          return;
        }
      for (int i = 1; i <= 8; i++)
        for (int j = 1; j <= 8; j++)
          if (mark[i][j] == 0) return;
      b_end = 1;
    } else {
      return;
    }
  } else {
    for (int k = 0; k <= 3; k++) {
      ti = i + next_direction[k][0], tj = j + next_direction[k][1];
      if (ti >= 1 && ti <= 8 && tj >= 1 && tj <= 8 && mark[ti][tj] == 0) {
        mark[ti][tj] = 1;
        DFS(ti, tj, color, 0);
        mark[ti][tj] = 0;
        if (b_end == 1) {
          map_end[i][j] = color;
          return;
        }
      }
    }
  }
  return;
}