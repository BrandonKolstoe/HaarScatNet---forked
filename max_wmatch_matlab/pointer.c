/* Assign a pointer link to a vertex.  Edge e joins a vertex in blossom */
/* u to a linked vertex. */

int POINTER (u,v,e)
int u,v,e;

{   int i;
    double del;

#ifdef DEBUG
    printf("Pointer u,v,e=%d %d %d-%d\n",u,v,END[OPPEDGE(e)],END[e]);
#endif

    LINK[u] = -DUMMYEDGE;
    NEXTVTX[LASTVTX[u]] = DUMMYVERTEX;
    NEXTVTX[LASTVTX[v]] = DUMMYVERTEX;
    
    if (LASTVTX[u] != u) {
	i = MATE[NEXTVTX[u]];
	del = -SLACK(i) / 2;
	}
    else del = LAST_D;

    i = u;
    while (i != DUMMYVERTEX) {
	Y[i] += del;
	NEXT_D[i] += del;
	i = NEXTVTX[i];
	}
    if (LINK[v] < 0) {
	LINK[v] = e;
	NEXTPAIR[DUMMYEDGE] = DUMMYEDGE;
	SCAN (v, DELTA);
	return NULL;
	}
    else {
	LINK[v] = e;
	return NULL;
	}
    
    return(1);
}


/* Scan each vertex in the blossom whose base is x */

int SCAN (x, del)
int x;
double del;

{   int u;
    double del_e;

#ifdef DEBUG
    printf("Scan del=%f x=%d\n",del,x);
#endif

    newbase = BASE[x];
    stopscan = NEXTVTX[LASTVTX[x]];
    while (x != stopscan) {
	Y[x] += del;
	NEXT_D[x] = LAST_D;
	pairpoint = DUMMYEDGE;
	e = A[x];
	while (e != 0) {
	    neighbor = END[e];
	    u = BASE[neighbor];
	    if (LINK[u] < 0) {
		if (LINK[BMATE (u)] < 0 || LASTVTX[u] != u) {
		    del_e = SLACK (e);
		    if (NEXT_D[neighbor] > del_e) {
			NEXT_D[neighbor] = del_e;
			NEXTEDGE[neighbor] = e;
			}
		    }
		}
	    else if (u != newbase) {
		INSERT_PAIR();
		}
	    e = A[e];
	    }
	x = NEXTVTX[x];
	}
    NEXTEDGE[newbase] = NEXTPAIR[DUMMYEDGE];
    
    return(1);
}


