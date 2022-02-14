#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "mp5.h"
#include "mp10.h"

#define MY_INFINITY   1000000000

void heapify(int arr[], int n, int i, graph_t* g);
int32_t Judge_startpoint(int index, vertex_set_t* src);
void swap(int * a, int* b);

// Introductory Paragraph: My task this week is to extend last week’s program to make
// use of dynamic allocation in several ways and to attempt to pair up requests from
// a list of many requests. For this purpose, I will implement and use structures
// designed to help improve performance. In particular, I will write C subroutines
// that manage dynamic allocation of vertex sets and paths, subroutines that enable,
// calculate, and use “mini maps” based on a high level of a pyramid tree, and a
// subroutine that attempts to find a partner for a request among a linked list of
// unpaired requests.

void
find_nodes (locale_t* loc, vertex_set_t* vs, pyr_tree_t* p, int32_t nnum)
{
    // Do not modify the following line nor add anything above in the function.
    record_fn_call ();
    //Initialize some variables for simplicity
    int32_t i=p->node[nnum].id;
    int32_t j=p->node[nnum].x;
    int32_t k=p->node[nnum].y_left;
    int32_t t=p->node[nnum].y_right;
    int32_t id_index;
    //Judge whether continue
    if (4*nnum+1 >= p->n_nodes){
        if ( in_range (loc,j,k)){
            if(vs->count>=vs->id_array_size){
                int32_t *new;
                int32_t size=2*vs->id_array_size*sizeof(*vs->id);
                //If can allocate, update the pointer
                new=realloc(vs->id,size);
                if(new!=NULL){
                    vs->id=new;
                }else{
                    return;
                    //just ignore and return otherwise
                }
                vs->id_array_size*=2;
            }
            
            if (vs->count==0){
                //If nothing in the array, just add it and return
                vs->id[0]=i;
                vs->count+=1;
                return;
            }else{
                //Otherwise sort it, find the suitable place
                for ( id_index=0; id_index < vs->count ;id_index++ ){
                    if (p->node[nnum].id <= vs->id[id_index]){
                        //Find the index
                        break;
                    }
                }
                if (id_index==vs->count){
                    //If it is the largest, add it at the end
                    vs->id[vs->count]=i;
                    vs->count+=1;
                    //Store, add the count and return
                    return;
                }else{
                    int32_t last_index=vs->count;
                    while (id_index < last_index){
                        //Right shift all larger than it
                        vs->id[last_index]=vs->id[last_index-1];
                        last_index--;
                    }
                        vs->id[id_index]=i;
                        vs->count+=1;
                        //Store, add the count and return
                        return;
                }
            }
        }else{
            //If out of range, fail
            return;
        }
    }else{
        //Recursive in four direction with conditions
        if ( (loc->x <= loc->range + j) && (loc->y <= loc->range + k)){
            find_nodes (loc,vs,p,4*nnum+1);
        }
        if ( (loc->x >= j-loc->range) && (loc->y <= loc->range + t)){
            find_nodes (loc,vs,p,4*nnum+2);
        }
        if ( (loc->x <= loc->range + j) && (loc->y >= k-loc->range)){
            find_nodes (loc,vs,p,4*nnum+3);
        }
        if ( (loc->x >=  j-loc->range)  && (loc->y >= t- loc->range)){
            find_nodes (loc,vs,p,4*nnum+4);
        }
    }
    
}


void
trim_nodes (graph_t* g, vertex_set_t* vs, locale_t* loc)
{
    for (int32_t i=0;i<vs->count;i++){
        //Transverse all points, if out of range, delete it
        if ( in_range (loc,g->vertex[vs->id[i]].x,g->vertex[vs->id[i]].y)!=1){
            for (int32_t j=i;j<vs->count;j++){
                vs->id[j]=vs->id[j+1];
                //Use next to cover the previous one
            }
            vs->count-=1;
            i--;
        }
    }
}


int32_t
dijkstra (graph_t* g, heap_t* h, vertex_set_t* src, vertex_set_t* dest,
          path_t* path)
{
    int32_t i= g->n_vertices;
    h->n_elts=g->n_vertices;
    
    //Initialize the heap here
    for ( int32_t heap_times=0; heap_times<i ; heap_times++){
        h->elt[heap_times]=heap_times;
    }
    //Initialize the from_src here

    for(int32_t heap_times=0;heap_times<h->n_elts;heap_times++)
    {
        //Find out all the starting points
        if(Judge_startpoint(h->elt[heap_times],src)==1)
        {
            g->vertex[h->elt[heap_times]].from_src=0;

            for(int32_t j=heap_times+1;j<h->n_elts;j++)
            {
                h->elt[j-1]=h->elt[j];
            }
            h->n_elts-=1;
            heap_times-=1;
        }
        else
        {
            g->vertex[h->elt[heap_times]].from_src=MY_INFINITY;
            //Set other points from_src to infinity
        }
        
    }


   
    int32_t startpoint_visited=0;
    //Set all startpoint visited
    

    for(int32_t i=0;i<src->count;i++)
    {
        for(int32_t j=0;j<g->vertex[src->id[i]].n_neighbors;j++)
        {
            if(g->vertex[g->vertex[src->id[i]].neighbor[j]].from_src>g->vertex[src->id[i]].from_src+g->vertex[src->id[i]].distance[j])
            {
                g->vertex[g->vertex[src->id[i]].neighbor[j]].from_src=g->vertex[src->id[i]].from_src+g->vertex[src->id[i]].distance[j];
                g->vertex[g->vertex[src->id[i]].neighbor[j]].pred=src->id[i];
                //Set pred for finding path
            }


        }
    }


    while (h->n_elts>0){
       i=h->n_elts/2-1;
        while (i>=0){
            heapify(h->elt,h->n_elts,i,g);
            i--;           
        }
        startpoint_visited+=1;
        
        //Find all neighbors


        for (int32_t j=0;j<g->vertex[h->elt[0]].n_neighbors;j++){
            //Initialize some variables for simplicity
            int32_t curr_from_src=g->vertex[h->elt[0]].from_src;
            int32_t neig_index=g->vertex[h->elt[0]].neighbor[j];
            int32_t neig_from_src=g->vertex[neig_index].from_src;
            int32_t dist_from_curr_to_neig=g->vertex[h->elt[0]].distance[j];
        //Update their src if less-length path found
            if (curr_from_src+dist_from_curr_to_neig<neig_from_src){
                g->vertex[neig_index].from_src=curr_from_src+dist_from_curr_to_neig;
                g->vertex[neig_index].pred=h->elt[0];
                //printf("%d\n",g->vertex[neig_index].from_src);
                //This is code for debugging
            }
        }
        h->elt[0]=h->elt[h->n_elts];
        h->n_elts-=1;


        
    }


    //Set path here
    int32_t least_index;
    int32_t least_path=MY_INFINITY;
    for ( int32_t path_times=0;path_times<dest->count; path_times++ ){
        if (g->vertex[dest->id[path_times]].from_src<least_path){
            least_path=g->vertex[dest->id[path_times]].from_src;
            least_index=dest->id[path_times];
        }
    }
    path->tot_dist=least_path;
    //printf("%d\n",least_index);
    //printf("%d\n",least_path);
    //These are codes for debugging


    int32_t temp;
    temp=least_index;
    path->n_vertices=0;
    int32_t index=0;
    
    //Find path here
    path->n_vertices++;
    
    while (g->vertex[temp].from_src!=0){
        //if (path->n_vertices>MAX_PATH_LENGTH){
        if(NULL==(path->id=malloc(sizeof(int32_t)*path->n_vertices))){
            return 0;
        }else{
            while (index<path->n_vertices){
                path->id[index]=temp;
                index+=1;
                temp=g->vertex[index].pred;
            }
            
        }
    }

    return 1;
}


//Help Functions begin here:


// This function is used to heaplify.
// The code here is based on mp7 sort.c
// And modify for mp9 to achieve functionality.
            
void
heapify(int32_t *arr, int n, int i, graph_t* g)
{
    int smallest = i;  // Initialize smallest as root
    int l = 2*i + 1;   // left = 2*i + 1
    int r = 2*i + 2;   // right = 2*i + 2
             
    // If left child is smaller than root
    if (l < n && g->vertex[arr[l]].from_src < g->vertex[arr[smallest]].from_src){
        smallest = l;
    }
             
    // If right child is smaller than smallest so far
    if (r < n && g->vertex[arr[r]].from_src < g->vertex[arr[smallest]].from_src){
        smallest = r;
    }
             
    // If smallest is not root
    if (smallest != i)
    {
        swap(&arr[i], &arr[smallest]);
            
    // Recursively heapify the affected sub-tree
        heapify(arr, n, smallest,g);
    }
}

// This function is used to heaplify.
// The code here is based on mp7 sort.c
// And modify for mp9 to achieve functionality.
            
void
swap(int * a, int* b)
{
    //Swap two inputs
    int t = *a;
    *a = *b;
    *b = t;
}


int32_t
Judge_startpoint(int index, vertex_set_t* src)
{
    for (int32_t i=0;i<src->count;i++)
    {
        //If it is start point(in the startpoint set), return 1
        if(index==src->id[i]){
            return 1;
        }
    }
    //Otherwise, fail
    return 0;
}
