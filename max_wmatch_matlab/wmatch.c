/* N-cubed weighted matching */
/* Implementation of H. Gabow's Ph.D. thesis, Stanford Univ. 1973 */
/* Written by Edward Rothberg  7/85 */
/* For complete details, please refer to the original paper */

/* Send either a Euclidean graph or an adjacency list graph. */
/* Returns an array, the ith entry being the mate of vertex 'i'. */
/* A zero entry indicates an unmatched vertex. */

/* To add new type, see readgraph.c */

#include <stdio.h>
#include <stdlib.h>

#include "match.defs"
 #include "graphtypes.h"
#include "pairs.c"
#include "pointer.c"
#include "readgraph.c"
#include "term.c"
#include "unpairs.c"


int *Weighted_Match (gptr,type,maximize)
Graph gptr; /* changed from in gptr to graph by xiuyuan CHENG */
int type, maximize;

{   int g, j, w, outcome, loop=1;

    /* set up internal data structure */
    SetUp(gptr,type);
    Initialize(maximize);
    /* printf("SetUp Initialize ok.\n"); */
    
    for(;;) {
    /*	    printf("Augment #%d\n",loop++); */
        
	DELTA = 0;
	for (v=1; v<=U; ++v)
	    if (MATE[v] == DUMMYEDGE)
		POINTER (DUMMYVERTEX, v, DUMMYEDGE);
	for(;;) {
	    i = 1;
	    for (j=2; j<=U; ++j)
		if (NEXT_D[i] > NEXT_D[j])
		    i = j;
	    DELTA = NEXT_D[i];
	    if (DELTA == LAST_D){
            /* printf("for loop to done.\n"); */
            goto done;
        
        }
	    v = BASE[i];
	    if (LINK[v] >= 0) {
		PAIR (&outcome);
		if (outcome == 1)
		    break;
		}
	    else {
		w = BMATE (v);
		if (LINK[w] < 0) {
		    POINTER (v, w, OPPEDGE(NEXTEDGE[i]));
		    }
		else UNPAIR (v, w);
	        }
	    }

	LAST_D -=DELTA;
	SET_BOUNDS();
	g = OPPEDGE(e);
	REMATCH (BEND(e), g);
	REMATCH (BEND(g), e);
	}
    
done:
    SET_BOUNDS();
    UNPAIR_ALL();
    for (i=1; i<=U;++i) {
	MATE[i] = END[MATE[i]];
	if (MATE[i]==DUMMYVERTEX)
	    MATE[i]=0;
	}

 /*   mexPrintf("in Weighted_Match: for loop.\n");
    
    for (v=1; v<=U; ++v)
        mexPrintf("MATE[%d] = %d \n", v, MATE[v]);
*/
/*    FreeUp(); */
    return(MATE);
}


void Initialize(maximize)

{   int i, allocsize;
    double max_wt= -MAXWT, min_wt=MAXWT;

    DUMMYVERTEX = U+1;
    DUMMYEDGE = U+2*V+1;
    END[DUMMYEDGE] = DUMMYVERTEX;

    for (i=U+2; i<=U+2*V; i+=2) {
	if (WEIGHT[i] > max_wt)
	    max_wt = WEIGHT[i];
	if (WEIGHT[i] < min_wt)
	    min_wt = WEIGHT[i];
	}
    if (!maximize) {
	if (U%2!=0) {
	    printf("Must have an even number of vertices to do a\n");
	    printf("minimum complete matching.\n");
	    exit(0);
	    }
	max_wt += 2; /* Don't want all zero weight */
	for (i=U+1; i<=U+2*V; i++)
	    WEIGHT[i] = max_wt-WEIGHT[i];
	max_wt = max_wt-min_wt;
	}
    LAST_D = max_wt/2.0;

    allocsize = (U+2)*sizeof(int);
    MATE     = (int *) malloc(allocsize);
    LINK     = (int *) malloc(allocsize);
    BASE     = (int *) malloc(allocsize);
    NEXTVTX  = (int *) malloc(allocsize);
    LASTVTX  = (int *) malloc(allocsize);
    
    NEXTEDGE = (int *) malloc(allocsize);
    
    Y        = (double *) malloc(  (U+2)*sizeof(double));
    NEXT_D   = (double *) malloc(  (U+2)*sizeof(double));
    
    
    allocsize = (U+2*V+2)*sizeof(int);
    NEXTPAIR = (int *) malloc(allocsize);

    for (i = 1; i <= U+1; ++i) {
	MATE[i] = DUMMYEDGE;
	NEXTEDGE[i] = DUMMYEDGE;
	NEXTVTX[i] = 0;
	LINK[i] = -DUMMYEDGE;
	BASE[i] = i;
	LASTVTX[i] = i;
        
	Y[i] = LAST_D;
	NEXT_D[i] = LAST_D;
	}
}

int FreeUp()
{
    free(LINK);
    free(BASE);
    free(NEXTVTX);
    free(LASTVTX);
    free(Y);
    free(NEXT_D);
    free(NEXTEDGE);
    free(NEXTPAIR);
    
    free(A);
    free(END);
    free(WEIGHT);
    
    free(MATE); /*stop the memory leak, use FreeUp at the end of main function */
    return(1);
}


int FreeGraph( Graph g, int n){
    Edge edge;
    int i,j;
    
    /* free all the edges */
    for (i = 1; i <= n; i++ ) {
        for (j = 1; j <= n; j++ ) {
            edge = FindEdge(g, i, j);
            RemoveEdge(g, edge); /*also free the edge */     
        }
    }
     

    /* free the graph, free all the nodes */
    free(g);
     
    return(1);
}

