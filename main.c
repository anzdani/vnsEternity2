#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include  <time.h>

#include "optimization.h"
#include "hun_methods.h"
#include "solver.h"
#include "support.h"

int improve(int ring,clock_t start,int p,int levelMax,int iterMax,int *cBS,int *bestScore,elem_sol **currBest,int width,int heigth,pos_t * pos_centers,pos_t *pos_borders,pos_t* pos_corners,
        int numCenters,int  numBorders,int  numCorners,int perc_shake_coner,int perc_shake_border,int perc_shake_center,int maxTotal,elem_sol**best,
        int stepStarts,int maxUguale);

clock_t globalTime;
int maxTime=-1;
int maxScore=-1;

int main(int argc, char *argv[]){
    int maxTotal;
    int numCorners,numBorders,numCenters;
    int perc_shake_border=0;
    int perc_shake_center=0;
    int perc_shake_coner=0;
    int width;
    int heigth;
    int corners [4];
    int *borders;
    int *centers;
    pos_t pos_corners[4];
    pos_t *pos_borders;
    pos_t *pos_centers;
    int currBestScore=0,bestScore=BEST_SCORE;

    int fine=0;
    int p=1;
    int levelMax=LEVEL_MAX;
    int stepStarts=0;

    int iterMax = ITER_MAX;
    clock_t start,stop;
    float difference;


    switch(argc){
       case 5:
                if ( argv[1][1] == 't')
                    sscanf(argv[2],"%d", &maxTime);
                else if ( argv[1][1] == 'p')
                      sscanf(argv[2],"%d", &maxScore);
                else{
                    printf("Option not valid\n");
                    goto error;
                }
                FILE_IN = strdup(argv[3]);
                FILE_OUT = strdup(argv[4]);
                break;
        case 7:
                if ( argv[1][1] == 't')
                    sscanf(argv[2],"%d", &maxTime);
                else{
                    printf("Option not valid\n");
                    goto error;
                }
                if ( argv[3][1] == 'p')
                      sscanf(argv[4],"%d", &maxScore);
                else{
                    printf("Option not valid\n");
                    goto error;
                }
                FILE_IN = strdup(argv[5]);
                FILE_OUT = strdup(argv[6]);
                break;
        default:
             error: printf("\nUSAGE: %s [-t <time in seconds>] [-p <maximum score>] <input file> <output file>\n",argv[0]);
             return 0;
    }

    srand(time(NULL));
    parser(corners,&borders,&centers,&vector,&width,&heigth);

    numCorners=4;
    numBorders=2*(heigth-2)+2*(width-2);
    numCenters=(heigth-2)*(width-2);
    pos_borders= (pos_t *) malloc( numBorders*sizeof(pos_t) );
    pos_centers= (pos_t*) malloc(numCenters*sizeof(pos_t));
    init_positions(pos_corners,pos_borders,pos_centers,width,heigth);
    maxTotal=heigth*(width-1)+width*(heigth-1);
    printf("Max Total: %d\n",maxTotal);

    elem_sol **currBest,**best;
    currBest=allocaMatrix(width,heigth);
    best=allocaMatrix(width,heigth);
    elem_sol**neighborOfcurrBest;
    neighborOfcurrBest=allocaMatrix(width,heigth);

    globalTime=clock();
    while (!fine){
        stepStarts++;
        start=clock();
        stop=clock();
        difference=(float)(stop-globalTime)/CLOCKS_PER_SEC;
        printf("\nGlobal Time %.2f Start %d - global best %d\n",difference,stepStarts,bestScore);
        //leggiFile(vector,currBest,width,heigth);
        generateRandomSolution(currBest,vector,corners,borders,centers,width,heigth);
        currBestScore=CheckMatchingEdgesSol(currBest,width,heigth);
        //printGame(heigth, width, currBest);
        //printf ("parto da %d \n", currBestScore);

        p=LEVEL_MIN;
        fine=improve(0,start,p,levelMax,iterMax,&currBestScore,&bestScore,currBest,width,heigth,pos_centers,pos_borders,pos_corners,
        numCenters,numBorders,numCorners,perc_shake_coner,perc_shake_border,perc_shake_center,maxTotal,best,stepStarts,MAX_UGUALE);

        //printGame(heigth, width, currBest);
        if ( currBestScore > bestScore ){
            storeSolution(best,currBest,width,heigth);
            bestScore=currBestScore;
            //stop = clock();
            //difference = (stop - start)/CLOCKS_PER_SEC;
            //printf ("\nBEST edges matching = %d \t tempo: %f\n", bestScore, difference);
            //printGame(heigth, width,best);
            printer(width,heigth,best);
        }
    }
    freeThings(currBest, best, neighborOfcurrBest, pos_borders, pos_centers, borders, centers, vector, heigth);
    return 0;
}


int improve(int ring,clock_t start,int p,int levelMax,int iterMax,int *currBestScore, int *bestScore,elem_sol **currBest,int width,int heigth,pos_t * pos_centers,pos_t *pos_borders,pos_t* pos_corners,
        int numCenters,int  numBorders,int  numCorners,int perc_shake_coner,int perc_shake_border,int perc_shake_center,int maxTotal,elem_sol**best,
        int stepStarts,int maxUguale){

        int stepTot=0;
        int iter,neighborOfcurrBestScore;
        int prev;
        clock_t stop;
        float difference;
        int uguale;
        int fine=0,i;
        elem_sol**neighborOfcurrBest;
        neighborOfcurrBest=allocaMatrix(width,heigth);

        int num,binCenter,binCorner,binBorder;
        binCorner=bin(numCorners);
        binBorder=bin(numBorders);

        ring++;
        //if(ring>2) {
          //  printf("Ring %d",ring);
            //char c=getchar();
            //}
    fine=0;
    uguale=0;
    int cnt=0;
    while(!fine && cnt < MAX_COUNT){
        cnt++;
        levelMax=heigth/2;
        p=LEVEL_MIN;
        while ( p< levelMax && !fine){
            iter=0;
            num=centersOnFrame(p,heigth,numCenters);
            binCenter=bin(num);
            computePercShake(binCorner,binBorder,binCenter,&perc_shake_coner,&perc_shake_border,&perc_shake_center);

            //uguale=0;
            //printf("p %d\n",p);
            while(iter < iterMax*p && !fine){
                stepTot++;
                neighborOfcurrBestScore=*currBestScore;
                storeSolution(neighborOfcurrBest,currBest,width,heigth);

               for (i=0; i<p; i++){
                    shake(p,neighborOfcurrBest, width, heigth, pos_centers, pos_borders, pos_corners, numCenters, numBorders, numCorners,perc_shake_coner,perc_shake_border,perc_shake_center);
                    //speedStarting(neighborOfcurrBest, width, heigth, pos_centers, pos_borders, pos_corners, numCenters, numBorders, numCorners);
                }
                neighborOfcurrBestScore=CheckMatchingEdgesSol(neighborOfcurrBest,width,heigth);
                //printf("ring %d p %d iter %d- Before %d \n",ring,p,iter,neighborOfcurrBestScore);

                if(p>1)
                    improvedLocal(&neighborOfcurrBestScore,neighborOfcurrBest,
                              width,heigth,pos_centers,pos_borders,pos_corners,numCenters,numBorders,numCorners,p);

                //improve(ring,start,LEVEL_MIN,LEVEL_MIN+1, iterMax,&neighborOfcurrBestScore,*currBestScore,neighborOfcurrBest,width,heigth,pos_centers,pos_borders,pos_corners,
                //numCenters,numBorders,numCorners,perc_shake_coner,perc_shake_border,perc_shake_center,maxTotal,best,stepStarts,maxUguale/p);
                //printf("ring %d p %d iter %d- After %d -%d\n",ring,p,iter,neighborOfcurrBestScore,CheckMatchingEdgesSol(neighborOfcurrBest,width,heigth));

              if(LOCAL_ROTATE){
                    localSearch3(pos_centers, numCenters, neighborOfcurrBest, vector, width, heigth);
                }
                //speedStarting(neighborOfcurrBest, width, heigth, pos_centers, pos_borders, pos_corners, numCenters, numBorders, numCorners);

                neighborOfcurrBestScore=CheckMatchingEdgesSol(neighborOfcurrBest,width,heigth);
                //printf("Dopo local ring %d p %d iter %d- After %d \n",ring,p,iter,neighborOfcurrBestScore);
                //if(ring==0) printf("Sono tornato!\n");

                if (DOLOCAL){
                    if (FROM_CORNERS_TO_CENTERS) {
                        doLocalSearchHun(CORNER,VALUE_K_CORNER,numCorners,pos_corners,neighborOfcurrBest,heigth,width,&neighborOfcurrBestScore);
                        doLocalSearchHun(BORDER,VALUE_K_BORDER,numBorders,pos_borders,neighborOfcurrBest,heigth,width,&neighborOfcurrBestScore);
                        doLocalSearchHun(CENTER,VALUE_K_CENTER,numCenters,pos_centers,neighborOfcurrBest,heigth,width,&neighborOfcurrBestScore);
                    }
                    else {
                    doLocalSearchHun(CENTER,VALUE_K_CENTER,numCenters,pos_centers,neighborOfcurrBest,heigth,width,&neighborOfcurrBestScore);
                    doLocalSearchHun(BORDER,VALUE_K_BORDER,numBorders,pos_borders,neighborOfcurrBest,heigth,width,&neighborOfcurrBestScore);
                    doLocalSearchHun(CORNER,VALUE_K_CORNER,numCorners,pos_corners,neighborOfcurrBest,heigth,width,&neighborOfcurrBestScore);
                    }
                }

                if(DOLOCAL_REPEAT){
                    do{
                        do{
                            doLocalSearchHun(CORNER,VALUE_K_CORNER,numCorners,pos_corners,neighborOfcurrBest,heigth,width,&neighborOfcurrBestScore);
                            prev=neighborOfcurrBestScore;
                            doLocalSearchHun(BORDER,VALUE_K_BORDER,numBorders,pos_borders,neighborOfcurrBest,heigth,width,&neighborOfcurrBestScore);
                        }while (prev !=neighborOfcurrBestScore );
                    doLocalSearchHun(CENTER,VALUE_K_CENTER,numCenters,pos_centers,neighborOfcurrBest,heigth,width,&neighborOfcurrBestScore);
                    }while (prev != neighborOfcurrBestScore);
                }

                stop = clock();
                difference = ((float)(stop - globalTime))/CLOCKS_PER_SEC;

                if( maxTime>0 && difference >= maxTime){
                    goto end;
                }

                if ( (neighborOfcurrBestScore > *currBestScore)  ||
                    ( /*p==1 &&*/uguale < maxUguale && (neighborOfcurrBestScore == *currBestScore) && different(neighborOfcurrBest,currBest,heigth,width)) ){
                 if (neighborOfcurrBestScore == *currBestScore)
                    uguale ++;


                 //   if(ring >= 3 ){
                    //printf ("\nRing %d nedges matching = %d \t currBestScore = %d \t bestScore = %d\n", ring,neighborOfcurrBestScore, *currBestScore, bestScore);
                    //printf("\nTime %f - Passaggio %d: Sono all'iter %d e shake è fatta %d volte A %3d B %3d C %3d  pt uguale per %d- Sono ripartito %d volte\n",
                    //        difference,stepTot, iter, p, perc_shake_coner,perc_shake_border,perc_shake_center,uguale,stepStarts);
                   // }
                    //printGame(heigth, width,neighborOfcurrBest);


                    //printf("p %d\n",p);

                    if (neighborOfcurrBestScore > *currBestScore){
                        p=LEVEL_MIN;
                        uguale=0;
                        //printGame(heigth, width, currBest);
                        //printf("\a\a\a");
                        p=LEVEL_MIN;iter=0;
                        num=centersOnFrame(p,heigth,numCenters);
                        binCenter=bin(num);
                        computePercShake(binCorner,binBorder,binCenter,&perc_shake_coner,&perc_shake_border,&perc_shake_center);
                        *currBestScore=neighborOfcurrBestScore;
                        /*
                        stop = clock();
                        difference = ((float)(stop - start))/CLOCKS_PER_SEC;
                        printf("\nTime %f - Best Score: %d\n",difference,*currBestScore);
                        */
                        }


                    *currBestScore=neighborOfcurrBestScore;
                    if ( (*currBestScore) > *bestScore ){
                        storeSolution(best,currBest,width,heigth);
                        *bestScore= (*currBestScore);

                        stop = clock();
                        difference = ((float)(stop - start))/CLOCKS_PER_SEC;
                        float differenceGlobal = ((float)(stop - globalTime))/CLOCKS_PER_SEC;
                        printf("\nGlobal Time %.2f - Time %.2f for %d instance - Best Score: %d\n",differenceGlobal,difference,stepStarts,*bestScore);

                    }

                    storeSolution(currBest,neighborOfcurrBest,width,heigth);
                    printer(width,heigth,currBest);
/*
                    stop = clock();
                    difference = ((float)(stop - globalTime))/CLOCKS_PER_SEC;
                    printf("\nGlobal Time %.2f\n",difference);
*/
                    if (stepTot >= MAXITERTOT || *bestScore == maxTotal ||
                        ( maxScore >0 && *bestScore >=maxScore) || ( maxTime>0 &&  difference>=maxTime) ){
                        end:
                        printf("\nDone!\n");
                        storeSolution(best,currBest,width,heigth);
                        printf("\nBest score = best %d in %f seconds\nResult matrix following:\n", *bestScore,difference);
                        printGame2(heigth, width, best);
                        printer(width,heigth,best);
                        fine = 1;
                        }
                }
                iter++;
                }
            p+=1;
            }
    }
    return fine;
}

