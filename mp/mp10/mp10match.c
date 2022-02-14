#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "mp5.h"
#include "mp10.h"


//
// These variables hold the heads of two singly-linked lists of 
// requests.  
//
// The avaialble list consists of unpaired requests: partner 
// should be NULL, and next is used to form the list.   
// 
// The shared list consists of groups with non-empty start and end 
// vertex intersections.  Only one of the requests in the group is in 
// the list.  The others are linked through the first's partner field 
// and then through the next field of the others in the group.  The 
// next field of the first request in a group is used to form the
// shared list.
//
// Note: for MP2, you should only build groups of two in the shared
// list.
//

static request_t* available = NULL;
static request_t* shared = NULL;


int32_t
handle_request (graph_t* g, pyr_tree_t* p, heap_t* h, request_t* r)
{
    //Declare all the global variables used
    vertex_set_t* src_vs;
    vertex_set_t* dst_vs;
    path_t* path;
    request_t* avai=available;
    request_t* new_avai=available; 


    //If any one of them cannot be allocated, fail and return 0
    if( ( NULL==( src_vs=new_vertex_set() ) ) || ( NULL==( dst_vs=new_vertex_set() ) )|| ( NULL==( path=new_path()) ) ){
        return 0;
    }else{
        //Otherwise, succeed, find nodes and build the minimap
        find_nodes(&(r->from),src_vs,p,0);
        find_nodes(&(r->to),dst_vs,p,0);
        
        build_vertex_set_minimap(g,src_vs);
        build_vertex_set_minimap(g,dst_vs);
        
        while (new_avai!=NULL){
            //If the next avaliable is not empty
            //Declare all the local variables used
            vertex_set_t* src_vs_avai;
            vertex_set_t* dst_vs_avai;
            vertex_set_t* vint_src;
            vertex_set_t* vint_dst;
            
            //If any one of them cannot be allocated, fail and return 0
            if( NULL== (src_vs_avai=new_vertex_set() ) || (NULL== ( dst_vs_avai=new_vertex_set() ) ) ){
                return 0;
            }else{
                //Otherwise, succeed, find nodes for the new one
                find_nodes(&(new_avai->from),src_vs_avai,p,0);		 
                find_nodes(&(new_avai->to),dst_vs_avai,p,0);

                if(src_vs_avai->count==0){
                    //If no source point, free the memory
                    free_vertex_set(src_vs_avai);
                    free_vertex_set(dst_vs_avai); 
                    avai=new_avai;
                    new_avai=new_avai->next;
                    continue;
                }else{
                    //Otherwise, judge the destination point
                    if (dst_vs_avai->count==0){
                        //If no destination point, free the memory
                        free_vertex_set(src_vs_avai);
                        free_vertex_set(dst_vs_avai);
                        avai=new_avai;
                        new_avai=new_avai->next;
                        continue;
                    }else{
                        //If any one of them cannot be allocated, fail and return 0
                        if( NULL== (vint_src=new_vertex_set() ) || (NULL== ( vint_dst=new_vertex_set() ) ) ){
                             return 0;
                        }else{
                            //Otherwise, allocate and find out the matched points
                            vint_src->count=0;
                            vint_dst->count=0;
                            //Initializations before searching
                            vint_src->id_array_size=src_vs->id_array_size;
                            vint_dst->id_array_size=dst_vs->id_array_size;

                            //If any one of them cannot be allocated, fail and return 0
                            if ((NULL==(vint_src->id=malloc(sizeof(int32_t)*vint_src->id_array_size)))||(NULL==(vint_dst->id=malloc(sizeof(int32_t)*vint_dst->id_array_size)))){
                                return 0;
                            }else{
                                //Otherwise, succeed, build minimap for the new one
                                build_vertex_set_minimap(g,src_vs_avai);
                                build_vertex_set_minimap(g,dst_vs_avai);

                                if((src_vs_avai->minimap & src_vs->minimap)==0){
                                    //If source's minimap or next's minimap is 0, fail and free memory
                                    free_vertex_set(src_vs_avai);
                                    free_vertex_set(dst_vs_avai);  
                                    free_vertex_set(vint_dst);
                                    free_vertex_set(vint_src); 
                                    //Use singly linked list to check next one
                                    avai=new_avai;
                                    new_avai=new_avai->next;   
                                    continue;
                                }else{
                                    if ((dst_vs_avai->minimap & dst_vs->minimap)==0){
                                        //If destination's minimap or next's minimap is 0, fail and free memory
                                        free_vertex_set(src_vs_avai);
                                        free_vertex_set(dst_vs_avai);  
                                        free_vertex_set(vint_dst);
                                        free_vertex_set(vint_src); 
                                         //Use singly linked list to check next one
                                        avai=new_avai;
                                        new_avai=new_avai->next;   
                                        continue;
                                    }else{
                                        if( merge_vertex_sets(src_vs_avai,src_vs,vint_src)==0){
                                            //Merge find that no point in common, fail and free the memory
                                            free_vertex_set(src_vs_avai);
                                            free_vertex_set(dst_vs_avai);  
                                            free_vertex_set(vint_dst);
                                            free_vertex_set(vint_src);  
                                             //Use singly linked list to check next one
                                            avai=new_avai;
                                            new_avai=new_avai->next;     
                                            continue;
                                        }else{
                                            if( merge_vertex_sets(dst_vs_avai,dst_vs,vint_dst)==0){
                                                //Merge find that no point in common, fail and free the memory
                                                free_vertex_set(src_vs_avai);
                                                free_vertex_set(dst_vs_avai);  
                                                free_vertex_set(vint_dst);
                                                free_vertex_set(vint_src);  
                                                 //Use singly linked list to check next one
                                                avai=new_avai;
                                                new_avai=new_avai->next;     
                                                continue;
                                            }else{
                                                //Otherwise, succeed and build minimap
                                                build_vertex_set_minimap(g,vint_dst);
                                                build_vertex_set_minimap(g,vint_src);

                                                h->n_elts=0;
                                                h->elt=malloc(sizeof(int32_t)*g->n_vertices);
                                                //Use dijkstra routine to find path
                                                if(dijkstra(g,h,vint_src,vint_dst,path)==0){
                                                    //If no path found, fail and free the memory
                                                    free_vertex_set(src_vs_avai);
                                                    free_vertex_set(dst_vs_avai);  
                                                    free_vertex_set(vint_dst);
                                                    free_vertex_set(vint_src); 
                                                    //Use singly linked list to check next one
                                                    avai=new_avai;
                                                    new_avai=new_avai->next;      
                                                    continue;            
                                                }else{
                                                    build_path_minimap(g,path);
                                                    //The partner and next fields of the second request should then be set to NULL, and the
                                                    //first request should be added to the shared list (using the first request’s next field).
                                                    avai->next=new_avai->next;
                                                    new_avai->next=NULL;

                                                    r->partner=new_avai;
                                                    new_avai->partner=NULL;
                                                    r->src_vs=vint_src;
                                                    new_avai->src_vs=vint_src;
                                                    r->dst_vs=vint_dst;
                                                    new_avai->dst_vs=vint_dst;
                                                    r->path=path;
                                                    new_avai->path=path;

                                                    r->next=shared;
                                                    shared=r;
                                                   //Free at the end and return 1 meaning success
                                                    free_vertex_set(src_vs_avai);
                                                    free_vertex_set(src_vs);
                                                    free_vertex_set(dst_vs_avai);
                                                    free_vertex_set(dst_vs);
                                                    return 1;
                                                }
                                            }
                                        }
                                    }
                                    
                                }
                            }
                        }
                    }
                    
                }

            }
        }
        //If next available is empty pointer, set value and return 1 meaning success
        r->src_vs=src_vs;
        r->dst_vs=dst_vs;
        r->path=NULL;
        r->partner=NULL;
        r->next=available;
        available=r;
        return 1; 
        
    }
}


void
print_results ()
{
    request_t* r;
    request_t* prt;

    printf ("Matched requests:\n");
    for (r = shared; NULL != r; r = r->next) {
        printf ("%5d", r->uid);
	for (prt = r->partner; NULL != prt; prt = prt->next) {
	    printf (" %5d", prt->uid);
	}
	printf (" src=%016lX dst=%016lX path=%016lX\n", r->src_vs->minimap,
		r->dst_vs->minimap, r->path->minimap);
    }

    printf ("\nUnmatched requests:\n");
    for (r = available; NULL != r; r = r->next) {
        printf ("%5d src=%016lX dst=%016lX\n", r->uid, r->src_vs->minimap,
		r->dst_vs->minimap);
    }
}


int32_t
show_results_for (graph_t* g, int32_t which)
{
    request_t* r;
    request_t* prt;

    // Can only illustrate one partner.
    for (r = shared; NULL != r; r = r->next) {
	if (which == r->uid) {
	    return show_find_results (g, r, r->partner);
	}
	for (prt = r->partner; NULL != prt; prt = prt->next) {
	    if (which == prt->uid) {
		return show_find_results (g, prt, r);
	    }
	}
    }

    for (r = available; NULL != r; r = r->next) {
        if (which == r->uid) {
	    return show_find_results (g, r, r);
	}
    }
    return 0;
}


static void
free_request (request_t* r)
{
    free_vertex_set (r->src_vs);
    free_vertex_set (r->dst_vs);
    if (NULL != r->path) {
	free_path (r->path);
    }
    free (r);
}

void
free_all_data ()
{
    request_t* r;
    request_t* prt;
    request_t* next;

    // All requests in a group share source and destination vertex sets
    // as well as a path, so we need free those elements only once.
    for (r = shared; NULL != r; r = next) {
	for (prt = r->partner; NULL != prt; prt = next) {
	    next = prt->next;
	    free (prt);
	}
	next = r->next;
	free_request (r);
    }

    for (r = available; NULL != r; r = next) {
	next = r->next;
	free_request (r);
    }
}


