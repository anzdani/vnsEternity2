#include <stdio.h>
#include <stdlib.h>


#include "support.h"

int tryCenter(element tile,int row, int column, int rotation, elem_sol **matrix, int heigth,int width){
    int total1=0;
    element dummy_tile;
    dummy_tile=tile;
    rotateTile(&dummy_tile,rotation);
    if (row != 0)   //if not in first row
        if (dummy_tile.up == matrix[row-1][column].tile.down) //check if the up is the same
            total1++;
    if (row != (heigth-1)) //if not in last row
        if (dummy_tile.down == matrix[row+1][column].tile.up) //check if the down is the same
            total1++;
    if (column != 0)    //if not in first column
        if (dummy_tile.left == matrix[row][column-1].tile.right)    //check if the left is the same
            total1++;
    if (column != (width-1))    //if not in last column
        if (dummy_tile.right == matrix[row][column+1].tile.left)    //check if the left is the same
            total1++;
    return total1;
}

void init_positions(pos_t *pos_corners, pos_t *pos_borders, pos_t *pos_centers, int width, int heigth){
    int l,j,i;
    pos_corners[0].x=0;
    pos_corners[0].y=0;
    pos_corners[0].side=UL;

    pos_corners[1].x=0;
    pos_corners[1].y=width-1;
    pos_corners[1].side=UR;

    pos_corners[2].x=heigth-1;
    pos_corners[2].y=0;
    pos_corners[2].side=DL;

    pos_corners[3].x=heigth-1;
    pos_corners[3].y=width-1;
    pos_corners[3].side=DR;


    l=0;
    for( j=1; j<width-1; j++){
        /** Bordo UP  ruota 1 **/
        pos_borders[l].x=0;
        pos_borders[l].y=j;
        pos_borders[l].side=UP;
        l++;
        /** Bordo Down ruota 3 **/
        pos_borders[l].x=heigth-1;
        pos_borders[l].y=j;
        pos_borders[l].side=DOWN;
        l++;
        }
    for (i=1; i<heigth-1; i++){
        /** Bordo LEFT  ruota 0 **/
        pos_borders[l].x=i;
        pos_borders[l].y=0;
        pos_borders[l].side=LEFT;
        l++;
        /** Bordo RIGHT  ruota 2 **/
        pos_borders[l].x=i;
        pos_borders[l].y=width-1;
        pos_borders[l].side=RIGHT;
        l++;
        }

        l=0;

        int k=1;
        int m=1;
        while ( k <= heigth/2+1) {
            i=k;
            for(j=m;j<width-m;j++){
                    pos_centers[l].x=i;
                    pos_centers[l].y=j;
                    pos_centers[l++].side=POS_C_L*POS_C_U;
                }
            j--;
            pos_centers[l-1].side=POS_C_L*POS_C_U*POS_C_R;
            for(i=k+1;i<heigth-k;i++){
                    pos_centers[l].x=i;
                    pos_centers[l].y=j;
                    pos_centers[l++].side=POS_C_U*POS_C_R;
                }
            i--;
            pos_centers[l-1].side=POS_C_U*POS_C_R*POS_C_D;
            for(j=width-2-m;j>m-1;j--){
                    pos_centers[l].x=i;
                    pos_centers[l].y=j;
                    pos_centers[l++].side=POS_C_R*POS_C_D;
                }
            j++;
            pos_centers[l-1].side=POS_C_R*POS_C_D*POS_C_L;
            for(i=heigth-2-k;i>k;i--){
                    pos_centers[l].x=i;
                    pos_centers[l].y=j;
                    pos_centers[l++].side=POS_C_D*POS_C_L;
                }
            pos_centers[l-1].side=POS_C_D*POS_C_L*POS_C_U;
            k++;
            m++;
        }
        pos_centers[l-1].side=POS_C_D*POS_C_L*POS_C_U*POS_C_R;
    /*
    l=0;
    for(i=1;i<heigth-1;i++)
            for(j=1;j<width-1;j++){
                pos_centers[l].x=i;
                pos_centers[l].y=j;
                pos_centers[l++].side=POS_CENTRALE;
            }
    */
        /*printf("\n  -   side RLUD");
        for (i=0;i<=l;i++)
            printf("\n%d - %d side %04d",pos_centers[i].x,pos_centers[i].y, pos_centers[i].side);
        */
}

int CheckMatchingEdgesSol(elem_sol **matrix,int heigth,int width) {

int total1=0, row, column;
for (row=0; row<heigth-1; row++)
    for (column=0; column<width-1; column++)
        {
        if (matrix[row][column].tile.right== matrix[row][column+1].tile.left)
            total1++;
        if (matrix[row][column].tile.down== matrix[row+1][column].tile.up)
            total1++;
        }
for( row=0; row<heigth-1;row++)
    if (matrix[row][heigth-1].tile.down== matrix[row+1][heigth-1].tile.up)
            total1++;
for (column=0; column<width-1; column++)
    if (matrix[heigth-1][column].tile.right== matrix[heigth-1][column+1].tile.left)
        total1++;
return total1;
}


void leggiFile(element *vector,elem_sol **matrix,int width,int heigth){
    FILE *fp;
    int n=0;
    int id,r;
    fp=fopen(FILE_OUT,"r");
    while ( n < width*heigth){
        fscanf(fp,"%d %d",&id,&r);
        printf("\n%d %d",id,r);
        matrix[n/heigth][n%heigth].idcell=id;
        matrix[n/heigth][n%heigth].tile=vector[id];
        matrix[n/heigth][n%heigth].rotation=r;
        rotateTile(& matrix[n/heigth][n%heigth].tile,r);
        n++;
    }
}


int CheckMatchingEdgesPerTile (int row, int column,int width,int heigth,elem_sol **matrix){
    int total1=0;
    if (row != 0)   //if not in first row
        if (matrix[row][column].tile.up == matrix[row-1][column].tile.down) //check if the up is the same
            total1++;
    if (row != (heigth-1)) //if not in last row
        if (matrix[row][column].tile.down == matrix[row+1][column].tile.up) //check if the down is the same
            total1++;
    if (column != 0)    //if not in first column
        if (matrix[row][column].tile.left == matrix[row][column-1].tile.right)    //check if the left is the same
            total1++;
    if (column != (width-1))    //if not in last column
        if (matrix[row][column].tile.right == matrix[row][column+1].tile.left)    //check if the left is the same
            total1++;
    return total1;
}

int check(elem_sol **matrix,int width,int heigth){
    int i,j,total=0;
    for(i=0;i<heigth;i++)
        for(j=0; j<width;j++)
            matrix[i][j].matching_edges=CheckMatchingEdgesPerTile (i, j,width,heigth,matrix);
    total=CheckMatchingEdgesSol(matrix,heigth,width);
    return total;
}

elem_sol **allocaMatrix(int width,int heigth){
    elem_sol **matrix;
    int i;
    matrix=(elem_sol **) malloc(heigth*sizeof(elem_sol *));
    for (i=0; i<heigth; i++)
        matrix[i]=(elem_sol *) malloc(width*sizeof(elem_sol));
    return matrix;
}

int parser (int *corners,int **b,int **c,element **vett,int *w,int *h) {
int width;
int heigth;
element *vector;
int *borders;
int *centers;

FILE* fin=fopen (FILE_IN,"r");

if ( fin== NULL)
    {
    printf("\nimpossible to open file in %s\n", FILE_IN);
    exit(0);
    }
fscanf(fin,"%d %d", &width, &heigth);
vector=(element *) malloc(width*heigth*sizeof(element));
borders= (int *) malloc((width*2 + heigth*2 -8)*sizeof(int));
centers= (int *) malloc(( (width-2) * (heigth-2) )*sizeof(int));

int i;

int numborders=0, numcorners=0, numcenters=0;

for (i=0; i< width*heigth; i++)
    {
        fscanf(fin,"%d %d %d %d", &vector[i].down, &vector[i].left, &vector[i].up, &vector[i].right);
        //printf("\nacquisita cella %d %d %d %d", vector[i].down, vector[i].left, vector[i].up, vector[i].right);
        int l=0;
        if (vector[i].left == 0)
            l++;
        if (vector[i].down == 0)
            l++;
        if (vector[i].up == 0)
            l++;
        if (vector[i].right == 0)
            l++;
        if (l==1)
            {
            vector[i].type=border;
            borders[numborders]=i;
            numborders++;
            }

        else if (l==2){
            vector[i].type=angle;
            corners[numcorners]=i;
            numcorners++;
            }
        else{
            vector[i].type=center;
            centers[numcenters]=i;
            numcenters++;
            }
    }
fclose(fin);
*w=width;
*h=heigth;
*b=borders;
*c=centers;
*vett=vector;
return 0;

}

int printer (int width,int heigth,elem_sol **matrix) {

FILE* fout=fopen(FILE_OUT,"w");
if (fout==NULL)
    {
     printf("\nimpossible to open file out %s\n", FILE_OUT);
     exit(0);
    }

int i,j;

for (i=0;i<heigth; i++)
    for (j=0;j<width; j++)
        fprintf(fout, "%d %d\n", matrix[i][j].idcell, matrix[i][j].rotation);
fclose(fout);
return 0;

}

void storeSolution(elem_sol **dst,elem_sol **src,int width,int heigth){
    int i,j;
    for (i=0; i<heigth; i++)
        for(j=0; j<width; j++)
            dst[i][j]=src[i][j];
}

void rotateTile (element *tile, int rotation){
int left,right,up,down;

switch(rotation){
        case 0:
            return;
        case 1:
            left=tile->down;
            down=tile->right;
            right=tile->up;
            up=tile->left;
            break;
        case 2:
            left=tile->right;
            down=tile->up;
            right=tile->left;
            up=tile->down;
            break;
        case 3:
            left=tile->up;
            down=tile->left;
            right=tile->down;
            up=tile->right;
            break;
    }
    tile->left=left;
    tile->up=up;
    tile->down=down;
    tile->right=right;
return;
}

void printGame(int heigth,int width,elem_sol **matrix){
    int f,i,j,l;

    printf("\n");
     for( f=0; f<heigth; f++)
        printf("---------");
    printf("\n");
    for(i=0; i< heigth; i++){
        for(l=0; l<3 ; l++){
                switch(l){
                case 0:
                    for(j=0;j<heigth;j++){
                        printf("|  \\");
                        printf("%c[%dm",COLOR,BASE-matrix[i][j].tile.up);
                        printf(" %03d", matrix[i][j].tile.up);
                        printf("%c[m",COLOR);
                        printf("/  |");
                    }
                    printf("\n");
                    break;
                case 1:
                    for(j=0;j<heigth;j++){
                        printf("%c[%dm",COLOR,BASE-matrix[i][j].tile.left);
                        printf("|%03d",matrix[i][j].tile.left);
                        printf("%c[m",COLOR);
                        if( matrix[i][j].blink == 1 ){
                            printf("%c[%dm",COLOR,GREENFG);
                            printf("%c[%dm",COLOR,BLINK);
                        }
                        if(  matrix[i][j].blink != 0 && matrix[i][j].blink%2 == 0 )
                            printf("%c[%dm",COLOR,REDFG);
                        printf(" %03d",matrix[i][j].idcell);
                        //printf(" X ");
                        printf("%c[m",COLOR);
                        printf("%c[%dm",COLOR,BASE-matrix[i][j].tile.right);
                        printf("%03d|",matrix[i][j].tile.right);
                        printf("%c[m",COLOR);
                    }
                    printf("\n");
                    break;
                case 2:
                    for(j=0;j<heigth;j++){
                        printf("|  /");
                        printf("%c[%dm",COLOR,BASE-matrix[i][j].tile.down);
                        printf(" %03d", matrix[i][j].tile.down);
                        printf("%c[m",COLOR);
                        printf("\\  |");
                    }
                    printf("\n");
                    break;
            }
        }
    }
    for( f=0; f<heigth; f++)
        printf("---------");
    printf("\n");
}


void printGame2(int heigth,int width,elem_sol **matrix){
    int f,i,j,l;

    printf("\n");
     for( f=0; f<heigth; f++)
        printf("---------");
    printf("\n");
    for(i=0; i< heigth; i++){
        for(l=0; l<3 ; l++){
                switch(l){
                case 0:
                    for(j=0;j<heigth;j++){
                        printf("| \\");
                        printf("%c[%dm",COLOR,BASE-matrix[i][j].tile.up);
                        printf(" %02d", matrix[i][j].tile.up);
                        printf("%c[m",COLOR);
                        printf("/ |");
                    }
                    printf("\n");
                    break;
                case 1:
                    for(j=0;j<heigth;j++){
                        printf("%c[%dm",COLOR,BASE-matrix[i][j].tile.left);
                        printf("|%02d",matrix[i][j].tile.left);
                        printf("%c[m",COLOR);
                        if( matrix[i][j].blink == 1 ){
                            printf("%c[%dm",COLOR,GREENFG);
                            printf("%c[%dm",COLOR,BLINK);
                        }
                        if(  matrix[i][j].blink != 0 && matrix[i][j].blink%2 == 0 )
                            printf("%c[%dm",COLOR,REDFG);
                        printf(" %02d",matrix[i][j].idcell);
                        //printf(" X ");
                        printf("%c[m",COLOR);
                        printf("%c[%dm",COLOR,BASE-matrix[i][j].tile.right);
                        printf("%02d|",matrix[i][j].tile.right);
                        printf("%c[m",COLOR);
                    }
                    printf("\n");
                    break;
                case 2:
                    for(j=0;j<heigth;j++){
                        printf("| /");
                        printf("%c[%dm",COLOR,BASE-matrix[i][j].tile.down);
                        printf(" %02d", matrix[i][j].tile.down);
                        printf("%c[m",COLOR);
                        printf("\\ |");
                    }
                    printf("\n");
                    break;
            }
        }
    }
    for( f=0; f<heigth; f++)
        printf("---------");
    printf("\n");
}
void freeThings(elem_sol ** m1, elem_sol ** m2, elem_sol ** m3, pos_t * v1, pos_t * v2, int * v3, int * v4, element * v5, int heigth) {

int i;
for (i=0; i<heigth; i++)
    {
    free(m1[i]);
    free(m2[i]);
    free(m3[i]);
    }
free(v1);
free(v2);
free(v3);
free(v4);
free(v5);


}

int different(elem_sol **m1,elem_sol **m2,int heigth,int width){
    int i,j;
    for(i=0; i<heigth; i++)
        for(j=0; j<width; j++)
            if( m1[i][j].idcell != m2[i][j].idcell || ( m1[i][j].idcell == m2[i][j].idcell && m1[i][j].rotation != m2[i][j].rotation) )
                return 1;
    return 0;
}

int bin(int n){
    return n*(n-1)/2;
}

int centersOnFrame(int p,int heigth,int numCenters){
    int i,num=0;
    if (p >= heigth/2)
        num=numCenters;
    else
        for(i=0; i<p;i++)
            num+=4*(heigth-2)-8*i-4;
    return num;
}


void computePercShake(int binCorner,int binBorder,int binCenter,int *perc_shake_corner,int *perc_shake_border,int *perc_shake_center){
    int sum=binCenter+binBorder+binCorner;
    *perc_shake_corner=1;
    *perc_shake_border=100*binBorder/sum;
    *perc_shake_center=100;
    //printf("a: %d %d%% b: %d %d%% c: %d %d%%\n",binCorner,*perc_shake_corner,binBorder,*perc_shake_border,binCenter,*perc_shake_center);
    return;
}

