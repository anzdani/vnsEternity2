#ifndef SUPPORT_INCLUDED
#define SUPPORT_INCLUDED

#include "optimization.h"
int tryCenter(element tile,int row, int column, int rotation, elem_sol **matrix, int heigth,int width);
void init_positions(pos_t *pos_corners, pos_t *pos_borders, pos_t *pos_centers, int width, int heigth);
int CheckMatchingEdgesSol(elem_sol **matrix,int heigth,int width);
int CheckMatchingEdgesPerTile (int row, int column,int width,int heigth,elem_sol **matrix);
int check(elem_sol **matrix,int width,int heigth);
elem_sol **allocaMatrix(int width,int heigth);
int parser(int *corners,int **borders,int **centers,element **vector,int *w,int *h);
int printer (int width,int heigth,elem_sol **matrix);
void storeSolution(elem_sol **dst,elem_sol **src,int width,int heigth);
void rotateTile (element *tile, int rotation);
void printGame(int heigth,int width,elem_sol **matrix);
void printGame2(int heigth,int width,elem_sol **matrix);
void freeThings(elem_sol ** m1, elem_sol ** m2, elem_sol ** m3, pos_t * v1, pos_t * v2, int * v3, int * v4, element * v5, int heigth);
void leggiFile(element *vector,elem_sol **matrix,int width,int heigth);
int bin(int n);
int different(elem_sol **m1,elem_sol **m2,int heigth,int width);
int centersOnFrame(int p,int heigth,int numCenters);
void computePercShake(int binCorner,int binBorder,int binCenter,int *perc_shake_corner,int *perc_shake_border,int *perc_shake_center);
#endif
