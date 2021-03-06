/* $Id: edge_base.h 25612 2020-07-06 22:17:23Z kobus $ */
/**
 * This work is licensed under a Creative Commons 
 * Attribution-Noncommercial-Share Alike 3.0 United States License.
 * 
 *    http://creativecommons.org/licenses/by-nc-sa/3.0/us/
 * 
 * You are free:
 * 
 *    to Share - to copy, distribute, display, and perform the work
 *    to Remix - to make derivative works
 * 
 * Under the following conditions:
 * 
 *    Attribution. You must attribute the work in the manner specified by the
 *    author or licensor (but not in any way that suggests that they endorse you
 *    or your use of the work).
 * 
 *    Noncommercial. You may not use this work for commercial purposes.
 * 
 *    Share Alike. If you alter, transform, or build upon this work, you may
 *    distribute the resulting work only under the same or similar license to
 *    this one.
 * 
 * For any reuse or distribution, you must make clear to others the license
 * terms of this work. The best way to do this is with a link to this web page.
 * 
 * Any of the above conditions can be waived if you get permission from the
 * copyright holder.
 * 
 * Apart from the remix rights granted under this license, nothing in this
 * license impairs or restricts the author's moral rights.
 */

/* =========================================================================== *
   |
   |  Copyright (c) 1994-2010 by Kobus Barnard (author)
   |
   |  Personal and educational use of this code is granted, provided that this
   |  header is kept intact, and that the authorship is not misrepresented, that
   |  its use is acknowledged in publications, and relevant papers are cited.
   |
   |  For other use contact the author (kobus AT cs DOT arizona DOT edu).
   |
   |  Please note that the code in this file has not necessarily been adequately
   |  tested. Naturally, there is no guarantee of performance, support, or fitness
   |  for any particular task. Nonetheless, I am interested in hearing about
   |  problems that you encounter.
   |
   |  Author:  Joseph Schlecht, Kyle Simek, Luca Del Pero
 * =========================================================================== */


#ifndef IVI_EDGE_H 
#define IVI_EDGE_H 

/* Start for c2man */

#include <stdlib.h>
#include <inttypes.h>

#include "i/i_float.h"
#include "m/m_matrix.h"

/* End of includes for c2man */

#ifdef __cplusplus
extern "C" {
#ifdef COMPILING_CPLUSPLUS_SOURCE
namespace ivi_c {
#endif
#endif

/** @brief Edge point in an image or matrix (single-sampled image). */
typedef struct
{
    /** @brief Column the point lies in. */
    uint32_t col;

    /** @brief Row the point lies in. */
    uint32_t row;

    /** @brief Rate of change in brightness along the columns at the point. */
    double dcol;

    /** @brief Rate of change in brightness along the rows at the point. */
    double drow;

    /** @brief Magnitude of the gradient. */
    double mag;

    /** @brief set to 1 if this is edge point is part of the silhouette of an object,
     *  0 otherwise */
    unsigned int silhouette;

}
Edge_point;

/** @brief Edge composed of edge points */
typedef struct
{
    /** @brief Number of edge points in the list */
    uint32_t num_points;

    /** @brief Array of edge points in the edge */
    Edge_point* points;
} Edge;

/** @brief Set of edges. */
typedef struct
{
    /** @brief Number of edges. */
    uint32_t num_edges;

    /** @brief Total number of points in all edges. */
    uint32_t total_num_pts;

    /** @brief Set of edges and edge points. */
    Edge* edges;

    uint32_t num_rows;
    uint32_t num_cols;
}
Edge_set;


/** @brief Detects a set of edges in each sample of an image. */
int detect_image_edge_set
(
    Edge_set**   edges_out,
    const IVI_image* img,
    float          sigma, 
    float          begin_thresh, 
    float          end_thresh,
    uint32_t       padding,
    unsigned char   use_fourier,
    unsigned char   noiseless_data
);


/** 
 * @brief Detects a set of edges in a matrix (single-sample image).
 */
int detect_matrix_edge_set
(
    Edge_set**    edges_out,
    const Matrix* m,
    float           sigma, 
    float           begin_thresh, 
    float           end_thresh,
    uint32_t        padding,
    unsigned char   use_fourier,
    unsigned char   noiseless_data
);

/** @brief Frees a set of edges. */
void free_edge_set(Edge_set* edges);

/**
 * @brief Removes single precision edges with a number of points less than a
 * specified minimum. 
 */
void remove_short_edges(Edge_set* edges, uint32_t min_len);


/** 
 * @brief Breaks apart single precision edges where the gradient direction
 * changes significantly.
 */
void break_edges_at_corners
(
    Edge_set*   edges, 
    float       thresh, 
    uint32_t    num_avg
);



/** @brief Randomly samples single precision edge points from a set of them. */
void sample_edge_set
(
    Edge_set**      edges_out, 
    const Edge_set* edges_in, 
    float             p
);



/** @brief Colors single precision set of edges in an image. */
int color_edge_set
(
    IVI_image**         img_out,
    const IVI_image*    img_in,
    const Edge_set* edges, 
    const Pixel*    pxl
);



/** 
 * @brief Colors single precision set of edges in an image with randomly
 * selected colors. 
 */
int randomly_color_edge_set
(
    IVI_image**         img_out,
    const IVI_image*    img_in,
    const Edge_set* edges
);



/** @brief Colors edge points in an image. */
int color_edge_points
(
    IVI_image**           img_out,
    const IVI_image*      img_in,
    const Edge_point* pts, 
    uint32_t            num_pts,
    const Pixel*      pxl
);

/** @brief Colors an edge point in an image. */
int color_edge_point
(
    IVI_image**           img_out,
    const IVI_image*      img_in,
    const Edge_point* pt,
    const Pixel*      pxl
);

/**
 * Colors edge points in an image with a randomly selected 
 * color.
 */
int randomly_color_edge_points
(
    IVI_image**           img_out,
    const IVI_image*      img_in,
    const Edge_point* pts, 
    uint32_t            num_pts
);






/** @brief Reads a set of edges in ASCII format from a file. */
int read_edge_set
(
    Edge_set** edges_out,
    const char*  fname
);

int unserialize_edge_set
(
    Edge_set** edges_out,
    const char* buffer
);



/** @brief Writes a set of single precision edges in ASCII format to a file. */
int write_edge_set
(
    const Edge_set* edges,
    const char*       fname
);


int serialize_edge_set
(
    const Edge_set* edges,
    char** buffer,
    size_t* length
);

/** @brief Copies the edge set edges_in into edges_out. */
int copy_edge_set(Edge_set** edges_out, const Edge_set* edges_in);

/** @brief Removes the edge at index edge_id */
int remove_edge(Edge_set* edges, unsigned int edge_id);

/** @brief Finds the index where the Edge pointed by iedge is stored.
 *
 * This is finds it in the input edge_set.  Notice that this is based on
 * pointers (the pointer address is compared, not the edge content).
 */
int find_edge_index(
    const Edge_set* edges,
    const Edge * iedge,
    unsigned int *index
);



void create_edge_from_end_points
(
    double x_0,
    double y_0,
    double x_1,
    double y_1,
    Edge_set * edge_set,
    int index
);

void find_edge_length_from_end_points
(
    double x_0,
    double y_0,
    double x_1,
    double y_1,
    unsigned int * length
);

Edge_set * create_edge_set
(
    unsigned int num_points,
    unsigned int num_edges,
    const Vector * lengths,
    unsigned int num_rows,
    unsigned int num_cols
);

#ifdef __cplusplus
#ifdef COMPILING_CPLUSPLUS_SOURCE
}
#endif
}
/* End for c2man */

#endif

#endif
