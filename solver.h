#ifndef SOLVER_INCLUDED
#define SOLVER_INCLUDED

#include "optimization.h"


void swapTwoTiles(elem_sol ** matrix,  int width, int heigth, pos_t posTile1, pos_t posTile2);
void localSearch0(pos_t *pos_centers,int numCenters, elem_sol **matrix,element *vector,int heigth,int width);
void localSearch3(pos_t *pos_centers,int numCenters, elem_sol **matrix,element *vector,int heigth,int width) ;
void shake(int p,elem_sol ** matrix, int width, int heigth, pos_t * pos_centers, pos_t * pos_borders,pos_t * pos_corners, int numCenters, int numBorders, int numCorners,int perc_shake_coner,int perc_shake_border,int perc_shake_center);
void swapTiles(elem_sol ** matrix,  int width, int heigth, pos_t * pos, int numElem);
void initialSol(elem_sol **matrix,element *vector,int width,int heigth);
void generateRandomSolution (elem_sol **matrix, element *vector,int *corners,int *borders,int *centers,int width,int heigth);
int improvedLocal(int *bestScore,elem_sol **bestSol,
                  int width,int heigth,pos_t * pos_centers,pos_t *pos_borders,pos_t* pos_corners,
                  int numCenters,int  numBorders,int  numCorners,int p);
#endif
