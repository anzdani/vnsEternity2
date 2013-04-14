#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "support.h"
#include "hun_methods.h"

void speedStarting(elem_sol **matrix,int width,int heigth,pos_t *pos_centers, pos_t *pos_borders, pos_t *pos_corners, int numCenters, int numBorders, int numCorners){

    int total=CheckMatchingEdgesSol(matrix,heigth,width);
    int i=0;
    int n;
    while (i<REPEAT_HUN){
        n=rand()%100;
        if( n < HUN_PERC_CORNERS)
            total=neighborHun(VALUE_K_CORNER_START,CORNER,matrix, width, heigth,  pos_centers, pos_borders, pos_corners,numCenters, numBorders,numCorners, total);
        else if( n < HUN_PERC_BORDERS)
            total=neighborHun(VALUE_K_BORDER_START,BORDER,matrix, width, heigth,  pos_centers, pos_borders, pos_corners,numCenters, numBorders,numCorners, total);
        else
            total=neighborHun(VALUE_K_CENTER_START,CENTER,matrix, width, heigth,  pos_centers, pos_borders, pos_corners,numCenters, numBorders,numCorners, total);
        i++;
    }

}

int neighborHun(int k,int type,elem_sol ** matrix, int width, int heigth,
                pos_t * pos_centers, pos_t * pos_borders, pos_t * pos_corners, int numCenters, int numBorders, int numCorners, int total) {

    int candidates[MAX_K];
    pos_t pos[MAX_K];
    int rot[MAX_K][MAX_K];
    int a[MAX_K];
    hungarian_problem_t prob;
    int **cc;
    int i,j;
    cc=malloc(sizeof(int*)*MAX_K);
    for(i=0;i<MAX_K;i++)
        cc[i]=malloc(sizeof(int)*MAX_K);


    switch(type){
        case 0:
            selectTiles(k,numCorners, pos_corners, type,matrix,candidates,pos);
            break;
        case 1:
            selectTiles(k,numBorders, pos_borders, type,matrix,candidates,pos);
            break;
        case 2:
            selectTiles(k,numCenters, pos_centers, type,matrix,candidates,pos);
            break;
    }

    computeCost(type,k,pos,matrix,width,heigth,cc,rot);
    //printGame(heigth,width,matrix);
    /*
    printf("   ");
    for(i=0; i<k; i++)
        printf("%2d ",candidates[i]);
    printf("\n");
     for(i=0; i<k; i++){
        printf("%2d ",candidates[i]);
        for(j=0; j<k; j++)
            printf("%2d ",cc[i][j]);
        printf("\n");
     }
    */
    hungarian_init(&prob,(int **)cc,k,k,HUNGARIAN_MODE_MAXIMIZE_UTIL);
    hungarian_solve(&prob);
    int newTotal=total;
    for(i=0;i<k;i++){
        newTotal-=CheckMatchingEdgesPerTile(pos[i].x, pos[i].y,width,heigth,matrix);
    }
    for(i=0; i<k; i++)
        for(j=0; j<k; j++)
            if ( prob.assignment[i][j] ) {
                a[i]=j;
                break;
            }
    for(i=0;i<k;i++){
        newTotal+=cc[i][a[i]];
        //printf("%d va in %d\n", candidates[i],candidates[a[i]]);
    }
    //printGame(heigth,width,matrix);
    //printf("Benefit: %d",newTotal -total);
    if (newTotal > total){
        total=newTotal;
        for(i=0;i<k;i++){
            //printGame(heigth,width,matrix);
            matrix[pos[a[i]].x][pos[a[i]].y].idcell=candidates[i];
            matrix[pos[a[i]].x][pos[a[i]].y].rotation=rot[i][a[i]];
            matrix[pos[a[i]].x][pos[a[i]].y].tile=vector[candidates[i]];
            rotateTile(&matrix[pos[a[i]].x][pos[a[i]].y].tile, matrix[pos[a[i]].x][ pos[a[i]].y].rotation);
            matrix[pos[a[i]].x][pos[a[i]].y].matching_edges=cc[i][a[i]];
            //printGame(heigth,width,matrix);
            }
    }
    //printGame(heigth,width,matrix);
    deselect(type,k,matrix,pos);
    //printf("\n tot %d - %d \n", total,check(matrix,width,heigth));
    hungarian_free(&prob);

    for (i=0; i<MAX_K; i++)
        free(cc[i]);
    free(cc);
    return total;
}

void deselect(int t,int k,elem_sol **matrix,pos_t *pos){
   int i;
    switch (t){
        case 0:
                for(i=0; i<k; i++)
                    matrix[pos[i].x][pos[i].y].blink=0;
                break;
        case 1:
                for(i=0; i<k; i++){
                    matrix[pos[i].x][pos[i].y].blink=0;
                    if( pos[i].side == UP || pos[i].side == DOWN ){
                        matrix[pos[i].x][pos[i].y+1].blink=0;
                        matrix[pos[i].x][pos[i].y-1].blink=0;
                    }
                    if( pos[i].side == LEFT || pos[i].side == RIGHT ){
                        matrix[pos[i].x+1][pos[i].y].blink=0;
                        matrix[pos[i].x-1][pos[i].y].blink=0;
                    }
                }
                break;
        case 2:
           for(i=0; i<k; i++){
                matrix[pos[i].x][pos[i].y].blink=0;
                matrix[pos[i].x][pos[i].y+1].blink=0;
                matrix[pos[i].x][pos[i].y-1].blink=0;
                matrix[pos[i].x+1][pos[i].y].blink=0;
                matrix[pos[i].x-1][pos[i].y].blink=0;
            }
    }
}

void computeCost(int num,int k,pos_t *pos,elem_sol **matrix,int width,int heigth, int **cc,int rot[MAX_K][MAX_K]){
        int i,j,max,rot_max,match,r;
        switch(num){
            case CORNER: //corners
                    for(i=0;i<k;i++){
                        for(j=0;j<k;j++){
                            max=0;
                            r=pos[j].side;
                            match=tryCenter(vector[matrix[pos[i].x][pos[i].y].idcell],pos[j].x, pos[j].y,r,matrix,heigth,width);
                            //match=tryCenter(matrix[pos[i].x][pos[i].y].tile,pos[j].x, pos[j].y,r,matrix,heigth,width);
                           if ( match >= max){
                                max=match;
                                rot_max=r;
                            }
                            cc[i][j]=max;
                            rot[i][j]=rot_max;
                            }
                        }
                    break;
            case BORDER:
                     for(i=0;i<k;i++){
                        for(j=0;j<k;j++){
                            max=0;
                            r=pos[j].side;
                            if(ONLY_BORDER)
                                 match=tryBorder(vector[matrix[pos[i].x][pos[i].y].idcell],pos[j].x, pos[j].y,r,matrix,heigth,width);
                                //match=tryBorder(matrix[pos[i].x][pos[i].y].tile,pos[j].x, pos[j].y,r,matrix,heigth,width);
                           else
                                match=tryCenter(vector[matrix[pos[i].x][pos[i].y].idcell],pos[j].x, pos[j].y,r,matrix,heigth,width);
                               // match=tryCenter(matrix[pos[i].x][pos[i].y].tile,pos[j].x, pos[j].y,r,matrix,heigth,width);
                            if ( match >= max){
                                max=match;
                                rot_max=r;
                            }
                            cc[i][j]=max;
                            rot[i][j]=rot_max;
                            }
                        }
                    break;
            case CENTER:
                    for(i=0;i<k;i++){
                        for(j=0;j<k;j++){
                            max=0;
                            for(r=0; r<4; r++){
                                match=tryCenter(vector[matrix[pos[i].x][pos[i].y].idcell],pos[j].x, pos[j].y,r,matrix,heigth,width);
                                //match=tryCenter(matrix[pos[i].x][pos[i].y].tile,pos[j].x, pos[j].y,r,matrix,heigth,width);
                                if ( match > max){
                                    max=match;
                                    rot_max=r;
                                }
                            }
                            cc[i][j]=max;
                            rot[i][j]=rot_max;
                            }
                    }
                break;
            }
}

void selectTiles(int k,int lim, pos_t *holes,int t,elem_sol **matrix,int *candidates,pos_t *pos){
    //pos_t temp;
    int i,num,cnt;

    for(i=0;i<k; i++){
        num=rand() % (lim);
        cnt=0;
        while ( matrix[holes[num].x][holes[num].y].blink != 0){
            num=rand()%(lim);
            cnt++;
        }

        candidates[i]=matrix[holes[num].x][holes[num].y].idcell;
        pos[i]=holes[num];
        matrix[pos[i].x][pos[i].y].blink=1;
        if ( t == 2){
            matrix[pos[i].x][pos[i].y+1].blink=2;
            matrix[pos[i].x][pos[i].y-1].blink=2;
            matrix[pos[i].x+1][pos[i].y].blink=2;
            matrix[pos[i].x-1][pos[i].y].blink=2;
        }
        if( t == 1) {
            if( pos[i].side == UP || pos[i].side == DOWN ){
                    matrix[pos[i].x][pos[i].y+1].blink=2;
                    matrix[pos[i].x][pos[i].y-1].blink=2;
            }else{
                if( pos[i].side == LEFT || pos[i].side == RIGHT ){
                        matrix[pos[i].x+1][pos[i].y].blink=2;
                        matrix[pos[i].x-1][pos[i].y].blink=2;
                }
            }
        }
        /*
        temp=holes[num];
        holes[num]=holes[lim-1];
        holes[lim-1]=temp;
        lim--;
        */
    }
    return;
}

int tryBorder(element tile,int row, int column, int rotation, elem_sol **matrix, int heigth,int width){
    int total=0;
    element dummy_tile;
    dummy_tile=tile;
    rotateTile(&dummy_tile,rotation);
    if (row == 0 || row == heigth-1 ) {
        if (dummy_tile.left == matrix[row][column-1].tile.right) //check if the up is the same
            total++;
        if (dummy_tile.right == matrix[row][column+1].tile.left) //check if the up is the same
            total++;
    }else{
        if(column == 0 || column == heigth-1){
            if (dummy_tile.up == matrix[row-1][column].tile.down) //check if the up is the same
            total++;
        if (dummy_tile.down == matrix[row+1][column].tile.up) //check if the up is the same
            total++;
        }
    }
    return total;
}

void doLocalSearchHun(int type,int k,int num,pos_t *holes,elem_sol **matrix,int heigth,int width,int *betterTotal){
    pos_t pos[MAX_K];
    int candidates[MAX_K];
    mossa_t mossa[MAX_K];
    int **cc;
    int rot[MAX_K][MAX_K];
    int i;
    char c;
    int changed=0;
    int ottimo;
    int possible;
    int total=*betterTotal;
    cc=malloc(sizeof(int*)*MAX_K);
     for(i=0;i<MAX_K;i++)
        cc[i]=malloc(sizeof(int)*MAX_K);

    ottimo=0;
    possible=0;
    changed=1;
    int j=0;
    while (changed){
        j++;
        changed=0;
        changed=selectTilesR(k, num, holes, pos,type,candidates,mossa,&possible,0,0,total,betterTotal,&ottimo,cc,rot,width,heigth,matrix);
        if(!possible){
            printf("Non sono riuscito a selezionare %d tiles\n", k);
            c=getchar();
            return;
        }
        if (changed){
            //printf("%d - %d\n",type ,j);
            //printf("\n%d-Before %d - %d -%d ",type,CheckMatchingEdgesSol(matrix,heigth,width),total,*betterTotal);
            if (DEBUG){
                printf("\nBefore %d - %d -%d ",CheckMatchingEdgesSol(matrix,heigth,width),total,*betterTotal);
                for(i=0; i<k; i++)
                    matrix[ mossa[i].newpos.x][mossa[i].newpos.y].blink=1;
                printGame(heigth,width,matrix);
                c=getchar();
            }
            total=*betterTotal;
            for(i=0; i<k; i++){
                if (DEBUG){
                    printf("Mossa %d: tile %d in x: %d, y: %d  da %d,%d con rot %d \n",i,mossa[i].idcell,mossa[i].newpos.x,mossa[i].newpos.y,
                       mossa[i].oldpos.x,mossa[i].oldpos.y,mossa[i].rot);
                }
                matrix[mossa[i].newpos.x][mossa[i].newpos.y].idcell=mossa[i].idcell;
                matrix[mossa[i].newpos.x][ mossa[i].newpos.y].rotation=mossa[i].rot;
                matrix[mossa[i].newpos.x][ mossa[i].newpos.y].tile=vector[mossa[i].idcell];
                rotateTile(&matrix[mossa[i].newpos.x][mossa[i].newpos.y].tile, mossa[i].rot);
                matrix[mossa[i].newpos.x][mossa[i].newpos.y].matching_edges=mossa[i].matching_edges;
            }
            //printf("\n%d-After %d - %d",type,CheckMatchingEdgesSol(matrix,heigth,width), *betterTotal);
            if(DEBUG){
                printf("\nAfter %d - %d",CheckMatchingEdgesSol(matrix,heigth,width), *betterTotal);
                printGame(heigth,width,matrix);
                for(i=0; i<k; i++)
                    matrix[ mossa[i].newpos.x][mossa[i].newpos.y].blink=0;
                    c=getchar();
            }
        }
    }

    for (i=0; i<MAX_K; i++)
        free(cc[i]);
    free(cc);
}

int selectTilesR(int k,int lim, pos_t *holes,pos_t *pos,int t,int *candidates,mossa_t *mossa,int *possibile,
                int i,int base, int total,int *max, int *ottimo,int **cc,int rot[MAX_K][MAX_K],int width,int heigth,elem_sol **matrix){

int j, num;
int newTotal = 0;
num=0;
int cambiato = 0;
int fatto = 0;
hungarian_problem_t prob;

if (DEBUG)
    printf("Chiamata: k:%d lim:%d t:%d ncr:%d base:%d\n", k,lim,t,i,base);
if ( base + k-i  > lim)
    return 0;

if (i == k){
        int a[MAX_K];
        *possibile=1;

        if (k!=1)
            k=k;
        computeCost(t,k,pos,matrix,width,heigth,cc,rot);
        // Applico l'algoritmo ungherese per trovare l'assegnazione che da' il massimo benefit
        hungarian_init(&prob,(int **)cc,k,k,HUNGARIAN_MODE_MAXIMIZE_UTIL);
        //hungarian_print_rating(&prob);
        hungarian_solve(&prob);
        //hungarian_print_assignment(&prob);
        //nel vettore prob.a ho l'assegnazione:   tile i va scambiata con prob.a[i]
        // Calcolo il nuovo totale con la nuova assegnazione
        newTotal=total;
        for(i=0;i<k;i++){
            // Visto che non aggiorno tutte le 4 caselle in fase di aggiornamento devo ricalcolare il matchingedges
            // posso gia avere questo valore quando considero la tile non spostata i=0 j=0 r=0
            newTotal-=CheckMatchingEdgesPerTile(pos[i].x, pos[i].y,width,heigth,matrix);
        }
        for(i=0; i<k; i++)
            for(j=0; j<k; j++)
                if ( prob.assignment[i][j] ) {
                    a[i]=j;
                    break;
                }

        for(i=0;i<k;i++){
            //aggiungo al totale il nuovo costo, uso cc come vettore e quindi uso i come riga e prob.a[i] come colonna
            newTotal+=cc[i][a[i]];
        }
        int benefit=0;
        benefit=newTotal-total;
        if(DEBUG)
            printf("K %d t %d benefit %d\n", k, t, benefit);
        if (newTotal > *max){
            for(i=0;i<k;i++){
                if (DEBUG)
                    printf("\tCella:%d (%d-%d) <->hole  %d (%d-%d) con rotazione %d\n",candidates[i], pos[i].x, pos[i].y,
                        candidates[a[i]],pos[a[i]].x,pos[a[i]].y, rot[i][a[i]]);
                // PROBLEMA NON DEVO SALVARE TUTTA LA MATRICE MA SOLO CIO CHE CAMBIA

                mossa[i].newpos=pos[a[i]];
                mossa[i].oldpos=pos[i];
                mossa[i].idcell=candidates[i];
                mossa[i].rot=rot[i][a[i]];
                mossa[i].matching_edges=cc[i][a[i]];

                //problema poi ho total modificato ma quando eseguo newTotal-=CheckMatchingEdgesPerTile(pos[i].x, pos[i].y);
                // non mi devo riferire al vecchio total
                }
            *max = newTotal;
            cambiato=1;
            *ottimo=0;
            if (t==2){
                if (benefit >= k*4*PERC_CEN_BENEFIT)
                    *ottimo=1;
            }else{
                if (benefit >= k*2*PERC_BORD_BENEFIT)
                    *ottimo=1;
            }
        }
        if (DEBUG){
            printf("\n OldTot %d Better %d New %d", total, *max, newTotal);
            printGame(heigth,width,matrix);
        }
        hungarian_free(&prob);

        return cambiato;
        }

    num=base;
    while (num < lim){
        if ( matrix[holes[num].x][holes[num].y].blink == 0 ){
            fatto = 1;
            candidates[i]=matrix[holes[num].x][holes[num].y].idcell;
            pos[i]=holes[num];
            matrix[pos[i].x][pos[i].y].blink=1;
            if ( t == 2){
                matrix[pos[i].x][pos[i].y+1].blink+=2;
                matrix[pos[i].x][pos[i].y-1].blink+=2;
                matrix[pos[i].x+1][pos[i].y].blink+=2;
                matrix[pos[i].x-1][pos[i].y].blink+=2;
                }
            if( t == 1){
                if( pos[i].side == UP || pos[i].side == DOWN ){
                    //if( vector[matrix[pos[i].x][pos[i].y+1].idcell].type!=angle )
                        matrix[pos[i].x][pos[i].y+1].blink+=2;
                    //if( vector[matrix[pos[i].x][pos[i].y-1].idcell].type!=angle )
                        matrix[pos[i].x][pos[i].y-1].blink+=2;
                    }
                else{
                    if( pos[i].side == LEFT || pos[i].side == RIGHT ){
                       // if( vector[matrix[pos[i].x+1][pos[i].y].idcell].type!=angle )
                            matrix[pos[i].x+1][pos[i].y].blink+=2;
                       // if( vector[matrix[pos[i].x-1][pos[i].y].idcell].type!=angle )
                            matrix[pos[i].x-1][pos[i].y].blink+=2;
                        }
                    }
                }

            cambiato+=selectTilesR(k,lim,holes,pos,t,candidates,mossa,possibile,i+1, num+1, total,max,ottimo,cc,rot,width,heigth,matrix);

            //backtrack su t e num
            if ( t == 2){
                matrix[pos[i].x][pos[i].y+1].blink-=2;
                matrix[pos[i].x][pos[i].y-1].blink-=2;
                matrix[pos[i].x+1][pos[i].y].blink-=2;
                matrix[pos[i].x-1][pos[i].y].blink-=2;
                }
            if( t == 1){
                if( pos[i].side == UP || pos[i].side == DOWN ){
                    //if( vector[matrix[pos[i].x][pos[i].y+1].idcell].type!=angle )
                        matrix[pos[i].x][pos[i].y+1].blink-=2;
                    //if( vector[matrix[pos[i].x][pos[i].y-1].idcell].type!=angle )
                        matrix[pos[i].x][pos[i].y-1].blink-=2;
                    }
                else{
                    if( pos[i].side == LEFT || pos[i].side == RIGHT ){
                       // if( vector[matrix[pos[i].x+1][pos[i].y].idcell].type!=angle )
                            matrix[pos[i].x+1][pos[i].y].blink-=2;
                       // if( vector[matrix[pos[i].x-1][pos[i].y].idcell].type!=angle )
                            matrix[pos[i].x-1][pos[i].y].blink-=2;
                        }
                    }
                }
            matrix[pos[i].x][pos[i].y].blink=0;
            if (*ottimo)
                return cambiato;
            }

        num++;
        }
   /*
    if(k!=1)
        cambiato=selectTilesR(k,lim,holes,t,0,i, base+1);
    */

return cambiato;
}

