#include <stdint.h>
#include <stdio.h>

#include "mp5.h"
#include "mp9.h"

// Introductory Paragraph: My task in this machine problem is to implement a request matching
// and pathfinding subroutines for a tool that helps people to find walking partners.
// In particular, I write C subroutines that identify possible starting and ending points
// and that find the shortest path between any pair of starting and ending points.
// For this purpose, I make use of a ‘pyramid tree’ and write code to implement a heap for
// use by Dijkstra’s single-source shortest-paths algorithm.

int32_t
match_requests (graph_t* g, pyr_tree_t* p, heap_t* h,
		request_t* r1, request_t* r2,
		vertex_set_t* src_vs, vertex_set_t* dst_vs, path_t* path)
{
    //Clear the start-point and destination-point count value (set to 0 to begin)
    src_vs->count=0;
    dst_vs->count=0;
    //Call the find_nodes function to find out all points within the range for the start-point
    find_nodes (&(r1->from),src_vs,p,0);
    //Call the trim_nodes function to clear out all points without the range for the start-point
    trim_nodes (g, src_vs, &(r2->from));
    //Call the find_nodes function to find out all points within the range for the destination-point
    find_nodes (&(r1->to),dst_vs,p,0);
    //Call the trim_nodes function to clear out all points without the range for the destination-point
    trim_nodes (g, dst_vs, &(r2->to));





    if ( src_vs->count==0 || dst_vs->count==0 ){
        //If no start-point or destination-point, fail
        return 0;
    }
    if ( dijkstra (g,h,src_vs,dst_vs,path)==0){
        //If the path length is beyond the boundary, fail
        return 0;
    }
    //Succeed otherwise
    return 1;
}
