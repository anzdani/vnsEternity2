#ifndef OPTIMIZATION_INCLUDED
#define OPTIMIZATION_INCLUDED

#include <time.h>

//#define FILE_IN "Files/pieces_10x10.txt"
//#define FILE_OUT "Files/sol.txt"

#define DEBUG 0

#define DIFFERENT_USED 1

/********* VNS Parameters *****/
//numero di shaking da fare ??
#define LEVEL_MIN   1
#define LEVEL_MAX   10 //12
#define DOLOCAL         0
#define DOLOCAL_REPEAT  0
#define LOCAL_ROTATE    1
#define ITER_MAX    10000//10000
#define BEST_SCORE    30
#define MAX_UGUALE  50//100
#define MAX_COUNT   10
#define MAXITERTOT 500000000

/*********** SHAKE phase parameters ***********/
//intervalli di frequenza di SHAKE da 0-100
//a 1 se si vuole cambiare le percetuali all'aumentare di iter
#define VARIABLE_PERC_SHAKE 0

#define SHAKE_PERC_CORNER 4
#define SHAKE_PERC_BORDER 32    //??
#define SHAKE_PERC_CENTER 100
//il resto è per SHAKE con Hun  - serve ??
#define SHAKE_HUN_PERC_BORDERS 100
//il resto è per SHAKE con Hun center

/******* DO LOCAL PARAMETER *******************/
#define FROM_CORNERS_TO_CENTERS 0
#define VALUE_K_CORNER 4
#define VALUE_K_BORDER 3
#define VALUE_K_CENTER 2
#define MAX_K 10
/*********** ComputeCost parameters ***********/
#define ONLY_BORDER 0
#define PERC_CEN_BENEFIT 1
#define PERC_BORD_BENEFIT 1

/********* SPEED HUN STARTING phase parameters *****/
#define START_WITH_HUN 0
#define REPEAT_HUN 10
#define VALUE_K_CORNER_START 4
#define VALUE_K_BORDER_START 4
#define VALUE_K_CENTER_START 4
//intervalli di frequenza di HUN da 0-100
#define HUN_PERC_CORNERS 4
#define HUN_PERC_BORDERS 32
//il resto è per CENTER


#define CORNER  0
#define BORDER  1
#define CENTER  2

#define POS_CENTRALE 4
#define POS_C_D     3
#define POS_C_U     5
#define POS_C_L     7
#define POS_C_R     11

#define DOWN 0
#define LEFT 1
#define UP 2
#define RIGHT 3

#define DL 0
#define UL 1
#define UR 2
#define DR 3

#define BLINK 5
#define REDFG 31
#define GREENFG 32
#define BOLD 1
#define COLOR 27
#define BASE 47


char * FILE_IN;
char * FILE_OUT;


typedef enum tipo_s {angle, border, center} type_cell;

typedef struct element_s{
int left;
int right;
int up;
int down;
type_cell type;
} element;

 element * vector;

typedef struct element_sol_s {
element tile;
int idcell;
int blink;
int rotation;   //clockwise
int matching_edges; //n of matching edges per tile (improve calculations over optimum)
} elem_sol;

typedef struct pos pos_t;
struct pos{
    int x;
    int y;
    int side;
};

typedef struct mossa{
    pos_t newpos;
    pos_t oldpos;
    int idcell;
    int matching_edges;
    int rot;
}mossa_t;



#endif // OPTIMIZATION_INCLUDED
