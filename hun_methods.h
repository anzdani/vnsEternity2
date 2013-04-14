#ifndef HUN_METH_INCLUDED
#define HUN_METH_INCLUDED

#include "optimization.h"
#include "hungarian.h"

void speedStarting(elem_sol **matrix,int width,int heigth,pos_t * pos_centers, pos_t * pos_borders, pos_t * pos_corners,int numCenters, int numBorders, int numCorners);
int  neighborHun(int k,int type,elem_sol ** matrix, int width, int heigth, pos_t * pos_centers, pos_t * pos_borders, pos_t * pos_corners,int numCenters, int numBorders, int numCorners, int total);
void deselect(int t,int k,elem_sol **matrix,pos_t *pos);
void computeCost(int num,int k,pos_t *pos,elem_sol **matrix,int width,int heigth, int **cc,int rot[MAX_K][MAX_K]);
void selectTiles(int k,int lim, pos_t *holes,int t,elem_sol **matrix,int *candidates,pos_t *pos);
int tryBorder(element tile,int row, int column, int rotation, elem_sol **matrix, int heigth,int width);
void doLocalSearchHun(int type,int k,int num,pos_t *holes,elem_sol **matrix,int heigth,int width,int *betterTotal);
int selectTilesR(int k,int lim, pos_t *holes,pos_t *pos,int t,int *candidates,mossa_t *mossa,int *possibile, int i,int base, int total,int *max,int *ottimo,int **cc,int rot[MAX_K][MAX_K],int width,int heigth,elem_sol **matrix);

#endif
