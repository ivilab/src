/* $Id: test_likelihood2.cpp 25499 2020-06-14 13:26:04Z kobus $ */

#include "l/l_sys_debug.h"  /* For ASSERT */
#include "likelihood_cpp/model_edge.h"
#include "likelihood_cpp/edge_points_likelihood.h"
#include <iostream>
#include "m_cpp/m_int_matrix.h"
#include "edge/edge_base.h"

int main(int argc, char **argv)
{
    using namespace ivi;
    using namespace ivi_c;

    std::vector<Model_edge> model_edges;
    model_edges.push_back( Model_edge(10.0, 10.0, 11.0, 10.0, false) );
    model_edges.push_back( Model_edge(9.0, 11.0, 9.0, 12.0, false) );

    ivi::Int_matrix model_map(20,20,0);
    model_map(9,10) = 1;
    model_map(9,11) = 1;
    model_map(12, 10) = 2;
    model_map(11, 10) = 2;

    ivi_c::Edge_set*   edge_set = 0;
    ASSERT(edge_set = (ivi_c::Edge_set *)ivi_malloc(sizeof(ivi_c::Edge_set)));
    edge_set->edges = NULL;

    ASSERT(edge_set->edges = (ivi_c::Edge *)ivi_malloc(2 * sizeof(ivi_c::Edge)));
    edge_set->num_edges = 2; //2
    ASSERT(edge_set->edges[0].points = (ivi_c::Edge_point *) ivi_malloc(4 * sizeof(ivi_c::Edge_point)));
    edge_set->edges[1].points = edge_set->edges[0].points + 2;
    edge_set->total_num_pts = 4; //4

    edge_set->edges[0].num_points = 2;
    edge_set->edges[0].points[0].col = 10;
    edge_set->edges[0].points[0].row = 10;
    edge_set->edges[0].points[0].dcol = 0.0;
    edge_set->edges[0].points[0].drow = 1.0;
    edge_set->edges[0].points[0].mag = 1.0;
    edge_set->edges[0].points[1].col = 11;
    edge_set->edges[0].points[1].row = 10;
    edge_set->edges[0].points[1].dcol = 0.0;
    edge_set->edges[0].points[1].drow = 1.0;
    edge_set->edges[0].points[1].mag = 1.0;

    edge_set->edges[1].num_points = 2;
    edge_set->edges[1].points[0].col = 10;
    edge_set->edges[1].points[0].row = 12;
    edge_set->edges[1].points[0].dcol = 1.0;
    edge_set->edges[1].points[0].drow = 0.0;
    edge_set->edges[1].points[0].mag = 1.0;
    edge_set->edges[1].points[1].col = 10;
    edge_set->edges[1].points[1].row = 11;
    edge_set->edges[1].points[1].dcol = 1.0;
    edge_set->edges[1].points[1].drow = 0.0;
    edge_set->edges[1].points[1].mag = 1.0;

    ivi::Edge_set data_edges(edge_set);

#warning "[Code police] Something wrong here"
#if 0
    Independent_edge_points_likelihood likelihood2(
        data_edges,
        60, //Number of angle bins
        20, //Number of image rows
        20, //Number of image cols
        1.0, //stdev for difference in orientation
        1.0, //stdev for distance
        10.0, // max dist allowed
        1.0, //background probability
        1e-60, //noise probability
        1e-13, //silhouette miss probability
        1e-13 //inner edge miss probability
     );

    Independent_edge_points_likelihood likelihood(likelihood2);


    double ll = likelihood(model_map, model_edges);
    ASSERT( fabs(ll + 8.35167962272242015) < DBL_EPSILON);

    std::vector<Model_edge> model_edges2;
    model_edges2.push_back( Model_edge(10.0, 10.0, 11.0, 10.0, false) );
    model_edges2.push_back( Model_edge(10.0, 12.0, 10.0, 11.0, false) );

    ivi::Int_matrix model_map2(20,20,0);
    model_map2(10,10) = 1;
    model_map2(10,11) = 1;
    model_map2(12, 10) = 2;
    model_map2(11, 10) = 2;


    double ll2 = likelihood(model_map2, model_edges2);
    ASSERT( fabs(ll2 + 7.3518509602346418 ) < DBL_EPSILON );
#endif

    return 0;
}
