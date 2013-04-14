#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "support.h"
#include "hun_methods.h"

#include "solver.h"



int improvedLocal(int *bestScore,elem_sol **bestSol,
                  int width,int heigth,pos_t * pos_centers,pos_t *pos_borders,pos_t* pos_corners,
                  int numCenters,int  numBorders,int  numCorners,int p){

    elem_sol **newSol,**currSol;
    int newScore,currScore;
    int i,j;
    newSol=allocaMatrix(width,heigth);
    currSol=allocaMatrix(width,heigth);

    //printf("\t pt %d\n",*bestScore);
    int cambiato=1;
    int end;
    int numC=centersOnFrame(p,heigth,numCenters);
    numC=numCenters;
    while(cambiato){
        end=0;
        cambiato=0;
        currScore=*bestScore;
        storeSolution(currSol,bestSol,width,heigth);
        //for(i=(partenza+1)%numCenters; i!=partenza && !end; i=(i+1)%numCenters)
        for(i=0; i<numC && !end;i++ )
          //  for(j=i; j!=partenza && !end; j=(j+1)%numCenters){
            for(j=i; j<numC && !end; j++){
                newScore=currScore;
                storeSolution(newSol,currSol,width,heigth);
                //newSol[pos_centers[i].x][pos_centers[i].y].blink=1;
                //newSol[pos_centers[j].x][pos_centers[j].y].blink=1;
                //printf("Before swap %d\n",newScore);
                //printGame(heigth,width,newSol);

                swapTwoTiles(newSol,width,heigth,pos_centers[i],pos_centers[j]);
                //newScore=CheckMatchingEdgesSol(newSol,width,heigth);
                //printf("After swap %d\n",newScore);
                //printGame(heigth,width,newSol);

                localSearch3(pos_centers, numCenters, newSol, vector, width, heigth);
                newScore=CheckMatchingEdgesSol(newSol,width,heigth);
                //printf("After local %d\n",newScore);
                //printGame(heigth,width,newSol);
                if ( (newScore > *bestScore) ){
                    *bestScore=newScore;
                    cambiato=1;
                    storeSolution(bestSol,newSol,width,heigth);
                    end=1;
                }
            }
        }

    //printf("\t\t pt %d\n",*bestScore);
    return 0;
    }




int tryCenter2(element tile,int row, int column, int rotation, elem_sol **matrix, int heigth,int width, int side){
    int total1=0;
    element dummy_tile;
    dummy_tile=tile;
    rotateTile(&dummy_tile,rotation);
    if ( side%POS_C_U == 0 )   //if not in first row
        if (dummy_tile.up == matrix[row-1][column].tile.down) //check if the up is the same
            total1++;
    if ( side%POS_C_D== 0 ) //if not in last row
        if (dummy_tile.down == matrix[row+1][column].tile.up) //check if the down is the same
            total1++;
    if ( side%POS_C_L== 0 )    //if not in first column
        if (dummy_tile.left == matrix[row][column-1].tile.right)    //check if the left is the same
            total1++;
    if ( side%POS_C_R== 0 )    //if not in last column
        if (dummy_tile.right == matrix[row][column+1].tile.left)    //check if the left is the same
            total1++;
    return total1;
}

void localSearch0(pos_t *pos_centers,int numCenters, elem_sol **matrix,element *vector,int heigth,int width) {
   int i, prevRot, newRot, originalMatchingEdges, nrot, newMatchingEdges, cambio;
    cambio=0;
    int diverso=1;
    while (diverso){
        diverso=0;
        for (i=0; i<numCenters; i++){
            prevRot = matrix[pos_centers[i].x][pos_centers[i].y].rotation;
            originalMatchingEdges = CheckMatchingEdgesPerTile(pos_centers[i].x, pos_centers[i].y,width,heigth,matrix);
            for (nrot=0; nrot<4; nrot++){
                newMatchingEdges = tryCenter2(vector[matrix[pos_centers[i].x][pos_centers[i].y].idcell],pos_centers[i].x, pos_centers[i].y, nrot,matrix,heigth,width,pos_centers[i].side);
                //newMatchingEdges = tryCenter(matrix[pos_centers[i].x][pos_centers[i].y].tile,pos_centers[i].x, pos_centers[i].y, nrot,matrix,heigth,width);
                if (newMatchingEdges > originalMatchingEdges){
                    cambio = 1;
                    newRot = nrot;
                    originalMatchingEdges = newMatchingEdges;
                    }
                }
            if (cambio){ //first improvement
                diverso=1;
                cambio=0;
                //nel caso in cui io abbia fatto il cambio
                //matrix[pos_centers[i].x][pos_centers[i].y].blink=1;
                //printGame(heigth, width,matrix);
                matrix[pos_centers[i].x][pos_centers[i].y].rotation =newRot;
                matrix[pos_centers[i].x][pos_centers[i].y].tile=vector[matrix[pos_centers[i].x][pos_centers[i].y].idcell];
                rotateTile(&matrix[pos_centers[i].x][pos_centers[i].y].tile,newRot);
                //printGame(heigth, width,matrix);
                //matrix[pos_centers[i].x][pos_centers[i].y].blink=0;
                }
            }
    }
}

void localSearch3(pos_t *pos_centers,int numCenters, elem_sol **matrix,element *vector,int heigth,int width) {

int i, prevRot, newRot, originalMatchingEdges, nrot, newMatchingEdges, cambio;
cambio=0;
int diverso=1;
//int partenza=rand()%numCenters;
while (diverso){
    diverso=0;
    for (i=0; i<numCenters; i++){
    //for(i=(partenza+1)%numCenters;i!=partenza; i=(i+1)%numCenters){
        prevRot = matrix[pos_centers[i].x][pos_centers[i].y].rotation;
        //matrix[pos_centers[i].x][pos_centers[i].y].matching_tiles
        originalMatchingEdges = CheckMatchingEdgesPerTile(pos_centers[i].x, pos_centers[i].y,width,heigth,matrix);
        for (nrot=0; nrot<4; nrot++){
            newMatchingEdges = tryCenter(vector[matrix[pos_centers[i].x][pos_centers[i].y].idcell],pos_centers[i].x, pos_centers[i].y, nrot,matrix,heigth,width);
            //newMatchingEdges = tryCenter(matrix[pos_centers[i].x][pos_centers[i].y].tile,pos_centers[i].x, pos_centers[i].y, nrot,matrix,heigth,width);
            if (newMatchingEdges > originalMatchingEdges){
                cambio = 1;
                newRot = nrot;
                originalMatchingEdges = newMatchingEdges;
                }
            }
        if (cambio){ //first improvement
            diverso=1;
            cambio=0;
            //nel caso in cui io abbia fatto il cambio
            //matrix[pos_centers[i].x][pos_centers[i].y].blink=1;
            //printGame(heigth, width,matrix);
            matrix[pos_centers[i].x][pos_centers[i].y].rotation =newRot;
            matrix[pos_centers[i].x][pos_centers[i].y].tile=vector[matrix[pos_centers[i].x][pos_centers[i].y].idcell];
            rotateTile(&matrix[pos_centers[i].x][pos_centers[i].y].tile,newRot);
            //printGame(heigth, width,matrix);
            //matrix[pos_centers[i].x][pos_centers[i].y].blink=0;
            }
        }
    }
}



void swapTwoTiles(elem_sol ** matrix,  int width, int heigth, pos_t posTile1, pos_t posTile2) {
elem_sol temp = matrix[posTile1.x][posTile1.y];
matrix[posTile1.x][posTile1.y] = matrix[posTile2.x][posTile2.y];
matrix[posTile2.x][posTile2.y] = temp;
}

void shake(int p,elem_sol ** matrix, int width, int heigth, pos_t * pos_centers, pos_t * pos_borders,pos_t * pos_corners,
           int numCenters, int numBorders, int numCorners,int perc_shake_coner,int perc_shake_border,int perc_shake_center) {

int total;
int n=rand()%100;
int num,i;

    if(n<perc_shake_coner){
        swapTiles(matrix, width, heigth, pos_corners, numCorners);
    }
    else if(n<perc_shake_border){
        swapTiles(matrix,  width, heigth, pos_borders, numBorders);
    }
    else if (n<perc_shake_center){
        num=0;
        if (p >= heigth/2)
            num=numCenters;
        else
            for(i=0; i<p;i++)
                num+=4*(heigth-2)-8*i-4;

       swapTiles(matrix,  width, heigth, pos_centers, num);
       //printf("\n p: %d - num %d",p,num);
    }
    else {
        n=rand()%100;
        total=CheckMatchingEdgesSol(matrix, heigth, width);
        if( n < SHAKE_HUN_PERC_BORDERS)
            total=neighborHun(VALUE_K_BORDER,BORDER,matrix, width, heigth,  pos_centers, pos_borders, pos_corners,numCenters, numBorders,numCorners, total);
        else
            total=neighborHun(VALUE_K_CENTER,CENTER,matrix, width, heigth,  pos_centers, pos_borders, pos_corners,numCenters, numBorders,numCorners, total);
    }
}

void swapTiles(elem_sol ** matrix,  int width, int heigth, pos_t * pos, int numElem) {

int temp1 = rand()%numElem;
pos_t posTile1 = pos[temp1];
int temp2 = rand()%numElem;
while (temp1== temp2)
    temp2 = rand()%numElem;
pos_t posTile2 = pos[temp2];

if (posTile1.side <= 3){
    matrix[posTile1.x][posTile1.y].tile=vector[matrix[posTile1.x][posTile1.y].idcell];
    rotateTile(&matrix[posTile1.x][posTile1.y].tile, posTile2.side);
    matrix[posTile1.x][posTile1.y].rotation=posTile2.side;

    matrix[posTile2.x][posTile2.y].tile=vector[matrix[posTile2.x][posTile2.y].idcell];
    rotateTile(&matrix[posTile2.x][posTile2.y].tile, posTile1.side);
    matrix[posTile2.x][posTile2.y].rotation=posTile1.side;
    }
/*
else
    {
    int n=rand()%4;
    matrix[posTile1.x][posTile1.y].tile=vector[matrix[posTile1.x][posTile1.y].idcell];
    rotateTile(&matrix[posTile1.x][posTile1.y].tile, n);
    matrix[posTile1.x][posTile1.y].rotation=n;
    matrix[posTile2.x][posTile2.y].tile=vector[matrix[posTile2.x][posTile2.y].idcell];
    rotateTile(&matrix[posTile2.x][posTile2.y].tile, n);
    matrix[posTile2.x][posTile2.y].rotation=n;
    }
*/
elem_sol temp = matrix[posTile1.x][posTile1.y];
matrix[posTile1.x][posTile1.y] = matrix[posTile2.x][posTile2.y];
matrix[posTile2.x][posTile2.y] = temp;

/*
matrix[posTile1.x][posTile1.y].blink=1;
matrix[posTile2.x][posTile2.y].blink=1;
printGame(heigth, width, matrix, vector);
printf("\nHo fatto lo swap tra cella %d e cella %d", matrix[posTile1.x][posTile1.y].idcell, matrix[posTile2.x][posTile2.y].idcell);
matrix[posTile1.x][posTile1.y].blink=0;
matrix[posTile2.x][posTile2.y].blink=0;*/
}




/*
void initialSol(elem_sol **matrix,element *vector,int width,int heigth){
    int l,j,i;
    matrix[0][0].idcell=2;
    rotateTile(&vector[matrix[0][0].idcell],1);
    matrix[0][heigth-1].idcell=0;
   rotateTile(&vector[matrix[0][heigth-1].idcell],2);
    matrix[heigth-1][0].idcell=1;
    rotateTile(&vector[matrix[heigth-1][0].idcell],0);
    matrix[heigth-1][heigth-1].idcell=3;
    rotateTile(&vector[matrix[heigth-1][heigth-1].idcell],3);

    l=4;
    for( j=1; j<heigth-1; j++){
        matrix[0][j].idcell=l++;
        rotateTile(&vector[matrix[0][j].idcell],2);
        matrix[heigth-1][j].idcell=l++;
        rotateTile(&vector[matrix[heigth-1][j].idcell],0);
    }
    for (i=1; i<heigth-1; i++){
        matrix[i][0].idcell=l++;
        rotateTile(&vector[matrix[i][0].idcell],1);
        matrix[i][heigth-1].idcell=l++;
        rotateTile(&vector[matrix[i][heigth-1].idcell],3);
    }
    for(i=1;i<heigth-1;i++)
        for(j=1;j<heigth-1;j++)
            matrix[i][j].idcell=l++;
}
*/
void generateRandomSolution (elem_sol **matrix, element *vector,int *corners,int *borders,int *centers,int width,int heigth) {

int num, i=0;
//corner solver

int lim=3;
int row=0, column=0;
int tempnum;
element temp;

for (i=0; i<4; i++)
    {
    num=rand() % (lim+1);  //generate random number between 0 and 3 to fill the corners
    matrix[row][column].idcell=corners[num];
    matrix[row][column].tile=vector[corners[num]];

    tempnum=corners[num];
    corners[num]=corners[lim];
    corners[lim]=tempnum;
    lim--;
    temp=matrix[row][column].tile;
        if (row==0)
            {
            if (column == 0){
                matrix[row][column].rotation= UL;//1
                matrix[row][column].tile = temp;
                rotateTile(&matrix[row][column].tile, UL);
                column=width-1;
                }
                else if (column==width-1)
                    {
                    matrix[row][column].rotation= UR;
                    matrix[row][column].tile = temp;
                    rotateTile(&matrix[row][column].tile, UR);//2
                    row=heigth-1;
                    }
            }
        else if (row == heigth-1)
            {
            if (column==0)
                {
                matrix[row][column].rotation= DL;//0
                matrix[row][column].tile = temp;
                rotateTile(&matrix[row][column].tile, DL);
                }
            else if (width-1)
                {
                matrix[row][column].rotation= DR;//2
                matrix[row][column].tile = temp;
                rotateTile(&matrix[row][column].tile, DR);
                column=0;
                }
            }

    }


//border solver

lim = 2*width + 2*heigth -8 -1;
column=0;
for (i=0; i<2; i++)
    {
        for (row=1; row<heigth-1; row++)
            {
             num=rand() % (lim+1); //generate random number between 0 and 3 to fill the corners
            matrix[row][column].idcell=borders[num];
            matrix[row][column].tile=vector[borders[num]];
            tempnum=borders[num];
            borders[num]=borders[lim];
            borders[lim]=tempnum;
            lim--;
            temp=matrix[row][column].tile;
            if (column==0)
                {
                matrix[row][column].rotation= LEFT;
                matrix[row][column].tile = temp;
                rotateTile(&matrix[row][column].tile, LEFT);//1
                }
            else if(column==width-1)
                {
                matrix[row][column].rotation= RIGHT;
                matrix[row][column].tile = temp;
                rotateTile(&matrix[row][column].tile, RIGHT);//3
                }
            }
    column=width-1;
    }
row=0;
for (i=0; i<2; i++)
    {
        for (column=1; column<width-1; column++)
            {
            num=rand() % (lim+1); //generate random number between 0 and 3 to fill the corners
            matrix[row][column].idcell=borders[num];
            matrix[row][column].tile=vector[borders[num]];
            tempnum=borders[num];
            borders[num]=borders[lim];
            borders[lim]=tempnum;
            lim--;
            temp=matrix[row][column].tile;
            if (row == 0)
               {
                matrix[row][column].rotation= UP;//2
                matrix[row][column].tile = temp;
                rotateTile(&matrix[row][column].tile, UP);
               }
            else if (row== width-1)
                {
                matrix[row][column].rotation= DOWN;
                matrix[row][column].tile = temp;
                rotateTile(&matrix[row][column].tile, DOWN);
                }
            }
    row=heigth-1;
    }
//center solver

lim= (width-2)*(heigth-2)-1;
for (row=1; row<heigth-1; row++)
    {
        for (column=1; column<width-1; column++)
            {
            num=rand() % (lim+1);  //generate random number between 0 and 3 to fill the corners
            matrix[row][column].idcell=centers[num];
            matrix[row][column].tile = vector[centers[num]];
            matrix[row][column].rotation=0;
            tempnum=centers[num];
            centers[num]=centers[lim];
            centers[lim]=tempnum;
            lim--;
            }
    }
//greedy rendomized center solver

/*
lim= (width-2)*(heigth-2)-1;
for (row=1; row<heigth-1; row++)
    {
        for (column=1; column<width-1; column++)
            {
             num=rand() % (lim+1);  //generate random number between 0 and 3 to fill the corners
            matrix[row][column].idcell=centers[num];
            temp = vector[centers[num]];
            int rot, totalLink, bestLink;
            bestLink = -1;
            for (i=0; i<4; i++)
                {
                totalLink=0;
                rotateTile (&temp, 1);
                if (temp.left == matrix[row][column-1].tile.right)
                    totalLink++;
                if (temp.up == matrix[row-1][column].tile.down)
                    totalLink++;
                if (column == width-1) //siamo nella penultima colonna
                    if (temp.right == matrix[row][column+1].tile.left)
                        totalLink++;
                if (row == heigth -1)   //siamo nella penultima riga
                    if (temp.down == matrix[row+1][column].tile.down)
                        totalLink++;
                if (totalLink > bestLink)
                    {
                    rot = i;
                    }
                }
            temp = vector[centers[num]];
            rotateTile(&temp, rot);
            matrix[row][column].tile = temp;
            matrix[row][column].rotation=rot;
            tempnum=centers[num];
            centers[num]=centers[lim];
            centers[lim]=tempnum;
            lim--;

            }
    }
*/
}
