#include <stdint.h>
#include <stdlib.h>

#include "mp10.h"

// Introductory Paragraph: My task this week is to extend last week’s program to make
// use of dynamic allocation in several ways and to attempt to pair up requests from
// a list of many requests. For this purpose, I will implement and use structures
// designed to help improve performance. In particular, I will write C subroutines
// that manage dynamic allocation of vertex sets and paths, subroutines that enable,
// calculate, and use “mini maps” based on a high level of a pyramid tree, and a
// subroutine that attempts to find a partner for a request among a linked list of
// unpaired requests. 

vertex_set_t*
new_vertex_set ()
{
    vertex_set_t* vs_ptr;
    vs_ptr=malloc(sizeof(vertex_set_t));
    // Memory allocation to get pointer
    if (vs_ptr!=NULL){
        //Initialize and return the structure
        vs_ptr->count=0;
        vs_ptr->id_array_size=20;
        vs_ptr->minimap=0;
        
        vs_ptr->id=malloc(vs_ptr->id_array_size*sizeof(int32_t));
        if(vs_ptr->id!=NULL){
            return vs_ptr;
        }else{
            return NULL;
            //If NULL, memory is full, fail
        }
    }else{
        return NULL;
        //If NULL, memory is full, fail
    }
}


void
free_vertex_set (vertex_set_t* vs)
{
    if (vs!=NULL){
        //If not empty, free it
        free(vs->id);
        free(vs);
    }else{
        return;
        //If NULL, memory is full, fail
    }
}


path_t*
new_path ()
{
    path_t *path_ptr;
    path_ptr=malloc(sizeof(path_t));
    // Memory allocation to get pointer
    if (path_ptr!=NULL){
        //Initialize and return the structure
        path_ptr->n_vertices=0;
        path_ptr->minimap=0;
        return path_ptr;
    }else{
        return NULL;
        //If NULL, memory is full, fail
    }
}


void
free_path (path_t* path)
{
    if (path!=NULL){
        free(path->id);
        free(path);
        //If not empty, free it
    }else{
        return;
        //If NULL, memory is full, fail
    }
}

