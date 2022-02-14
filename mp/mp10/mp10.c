#include <stdint.h>
#include <stdio.h>

#include "mp10.h"

// Introductory Paragraph: My task this week is to extend last week’s program to make
// use of dynamic allocation in several ways and to attempt to pair up requests from
// a list of many requests. For this purpose, I will implement and use structures
// designed to help improve performance. In particular, I will write C subroutines
// that manage dynamic allocation of vertex sets and paths, subroutines that enable,
// calculate, and use “mini maps” based on a high level of a pyramid tree, and a
// subroutine that attempts to find a partner for a request among a linked list of
// unpaired requests. 

int32_t 
mark_vertex_minimap (graph_t* g, pyr_tree_t* p)
{
    int32_t index;
    //Index for identifying the pyramid tree node
    int32_t pt_index;
    //If the graph has 64 vertices or fewer, simply use each vertex’ array index for the vertex’ mm_bit field.
    if(g->n_vertices <= 64){
        for (index=0;index < g->n_vertices;index++){
            g->vertex[index].mm_bit=index;
        }
        return 1;
    }else{
        //Otherwise, for each graph vertex, your function must identify the pyramid tree node corresponding to each vertex
        for(index=p->n_nodes-1;index >= (p->n_nodes-2)/4;index--){
            pt_index=index;
            //Find the ancestor node
            //for (pt_index;pt_index<=84;){
            while(pt_index>84){
                pt_index=(pt_index-1)/4;
            }
            g->vertex[p->node[index].id].mm_bit=pt_index-21;
            //Set the vertex’ mm_bit to the ancestor’s node index minus 21.
        }
        return 1;
    }
    return 0;
}


void 
build_vertex_set_minimap (graph_t* g, vertex_set_t* vs)
{
    //Declare all variables used
    int32_t mmbit_info;
    uint64_t bit_const=1ULL;
    uint64_t shifting;
    
    for(int32_t index=0;index<vs->count;index++){
        //Calculate the vertex set (or path’s) minimap field
        mmbit_info=g->vertex[vs->id[index]].mm_bit;
        //Use 1ULL to obtain a 64-bit constant before shifting
        shifting=bit_const<<mmbit_info;
        vs->minimap =vs->minimap|shifting;
    }
}


void 
build_path_minimap (graph_t* g, path_t* p)
{
    //Declare all variables used
    int32_t mmbit_info;
    uint64_t bit_const=1ULL;
    uint64_t shifting;
    
    for(int32_t index=0;index<p->n_vertices;index++){
        //Calculate the vertex set (or path’s) minimap field
        mmbit_info=g->vertex[p->id[index]].mm_bit;
        //Use 1ULL to obtain a 64-bit constant before shifting
        shifting=bit_const<<mmbit_info;
        p->minimap =p->minimap|shifting;
    }
}


int32_t
merge_vertex_sets (const vertex_set_t* v1, const vertex_set_t* v2,
		   vertex_set_t* vint)
{
    //Declaration for all the variables used
    int32_t v1_index=0;
    int32_t v2_index=0;
    vint->count=0;
    
    //Finds the intersection of two vertex sets v1 and v2
    while ( (v1_index < v1->count) && (v2_index < v2->count) ){
            if(v1->id[v1_index]==v2->id[v2_index]){
                //If equal, add to vint
                vint->id[vint->count]=v1->id[v1_index];
                vint->count++;
                v1_index++;
                v2_index++;
            }else{
                //If not equal, judge which is larger
                if (v1->id[v1_index] < v2->id[v2_index] ){
                    v1_index++;
                }else{
                    v2_index++;
                }
            }
    }
    if (vint->count > 0){
        return 1;
        //If the result is not empty
    }else{
        return 0;
        //Otherwise, fail
    }
        
}

