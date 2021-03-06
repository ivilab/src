/* $Id: edge.h 25764 2020-09-05 23:08:20Z kobus $ */
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
   |  Author:  Kyle Simek
 * =========================================================================== */

#ifndef IVI_CPP_EDGE_BASE_H
#define IVI_CPP_EDGE_BASE_H

#include <string>

#include <l/l_incl.h>
#include <edge/edge_base.h>
#include <i_cpp/i_image.h>

#ifdef IVI_HAVE_BST_SERIAL
#include <boost/serialization/list.hpp>
#include <boost/serialization/split_member.hpp>
//#include <boost/archive/text_oarchive.hpp>
//#include <boost/archive/text_iarchive.hpp>
#endif

#include <boost/shared_ptr.hpp>
#include <string>

namespace ivi
{

class Edge;
class Edge_set;

/** @class Edge_point this class wraps the c structure Edge_point.
 * No memory allocation is handled in here, this class simply
 * provides a c++ interface to the underlying c structure
 */
class Edge_point
{
public:
    Edge_point(ivi_c::Edge_point * pt)
    {
        _c_edge_pt = pt;
    }

    ~Edge_point() { }

    Edge_point(const Edge_point & pt)
    {
        (*this) = pt;
    }

    Edge_point & operator=(const Edge_point & pt)
    {
        _c_edge_pt = pt._c_edge_pt;
        return (*this);
    }

    /** @brief returns the column the point lies in */
    inline unsigned int get_col() const { return _c_edge_pt->col;}

    /** @brief returns the row the point lies in */
    inline unsigned int get_row() const { return _c_edge_pt->row;}

    /** @brief returns the rate of change in brightness along the columns at the point. */
    inline double get_dcol() const {return _c_edge_pt->dcol;}

    /** @brief returns the rate of change in brightness along the rows at the point. */
    inline double get_drow() const {return _c_edge_pt->drow;}

    /** @brief returns the magnitude of the gradient */
    inline double get_gradient_magnitude() const {return _c_edge_pt->mag;}
    /** @brief Rate of change in brightness along the columns at the point. */

    /** @brief  returns true if this is edge point is part of the silhouette of an object,
     *  false otherwise */
    inline bool get_silhouette() const {return (bool)(_c_edge_pt->silhouette); }

    /** @brief sets the column the point lies in */
    inline void set_col(unsigned int icol) { _c_edge_pt->col = icol;}

    /** @brief returns the row the point lies in */
    inline void set_row(unsigned int irow) { _c_edge_pt->row = irow;}

    /** @brief returns the rate of change in brightness along the columns at the point. */
    inline void set_dcol(double idcol) { _c_edge_pt->dcol = idcol;}

    /** @brief returns the rate of change in brightness along the rows at the point. */
    inline void set_drow(double idrow) { _c_edge_pt->drow = idrow;}

    /** @brief returns the magnitude of the gradient */
    inline void set_gradient_magnitude(double imag) { _c_edge_pt->mag = imag;}

    /** @brief sets the silhouette field of this edge point */
    inline void set_silhouette(bool isilhouette) { _c_edge_pt->silhouette = (unsigned int)isilhouette; }

    /** @brief Rate of change in brightness along the columns at the point. */

    /** @brief returns a const pointer to the c Edge_point structure */
    const ivi_c::Edge_point * _c_ptr()
    {
        return _c_edge_pt;
    }

    /** @brief draws this edge point onto an image */
    void draw(Image & img, float r, float g, float b) const
    {
        ivi_c::Pixel pixel;
        pixel.r = r;
        pixel.g = g;
        pixel.b = b;
        ivi_c::IVI_image * out = img.non_const_c_ptr();
        ETX(ivi_c::color_edge_point(&out, out, _c_edge_pt, &pixel) );
    }


private:

    /** @brief Sets the pointer to the underlying c edge point
     *
     * @param i_edge_pt the pointer to the c edge_point
     */
    void set_edge_pt(ivi_c::Edge_point * i_edge_pt)
    {
        _c_edge_pt = i_edge_pt;
    }

    // Pointer to the c Edge_point structure
    ivi_c::Edge_point * _c_edge_pt;

    friend class ivi::Edge;
    friend class ivi::Edge_set;

};

/** @class Edge this class wraps the c structure Edge.
 * No memory allocation is handled in here, this class simply
 * provides a c++ interface to the underlying c structure
 */
class Edge
{
public:
    Edge(ivi_c::Edge *edge)
    {
        if(edge == NULL)
        {
            throw ivi::Illegal_argument(
                    "Null edge pointer passed to Edge class constructor");
        }
        _c_edge = edge;

    }

    ~Edge() {}

    Edge(const Edge & edge)
    {
        (*this) = edge;
    }

    Edge & operator=(const Edge & edge)
    {
        _c_edge = edge._c_edge;
        return (*this);
    }

    /** @brief returns the number of edge points in this edge */
    inline unsigned int get_num_points() const
    {
        return _c_edge->num_points;
    }

    /** @brief returns the ith edge point of this edge */
    Edge_point get_edge_point(unsigned int i) const
    {
        if(i >= get_num_points())
        {
            throw ivi::Illegal_argument("Edge point index out of bounds");
        }
        return Edge_point(&_c_edge->points[i]);
    }

    /** @brief returns a const pointer to the c Edge structure */
    const ivi_c::Edge * _c_ptr() const
    {
        return _c_edge;
    }

    /** @brief draws the edge onto the input image,
     * using color (r,g,b). This funcstion expects
     * color values between 0 and 255
     */
    void draw(Image& img, float r, float g, float b) const
    {
        ivi_c::Pixel pixel;
        pixel.r = r;
        pixel.g = g;
        pixel.b = b;
        ivi_c::IVI_image * out = img.non_const_c_ptr();
        ETX(ivi_c::color_edge_points(&out, out, _c_edge->points, _c_edge->num_points, &pixel) );
    }

    /** @brief draws the edge onto the input image,
     * using a random color
     */
    void randomly_color(Image& img) const
    {
        ivi_c::IVI_image * out = img.non_const_c_ptr();
        ETX(ivi_c::randomly_color_edge_points(&out, out, _c_edge->points, _c_edge->num_points) );
    }


private:

    // Pointer to the c Edge structure
    ivi_c::Edge * _c_edge;
};

/** @class Edge_set this class wraps the c structure Edge_set.
 * The class will clean up the memory allocated for
 * the underlying c structure upon destruction
 */
class Edge_set
{
public:
    Edge_set() :
        m_edge_set(0)
    {}

    /** @brief Constructs an Edge_set by reading it from an input stream */
    Edge_set(ivi_c::Edge_set* in) :
        m_edge_set(in)
    {

    }

    /** @brief Constructs an Edge_set by reading it from an input file */
    Edge_set(const char* fname) :
        m_edge_set(0)
    {
        load(fname);
    }

    /** @brief Constructs an Edge_set by reading it from an input file */
    Edge_set(const std::string& fname) :
        m_edge_set(0)
    {
        load(fname.c_str());
    }

    Edge_set(const Edge_set& edge_in) :
        m_edge_set(0)
    {
        ivi_c::copy_edge_set(&m_edge_set, edge_in.m_edge_set);
    }

    /**
     * Construct an edge set from a vector of independent edge points.
     * The result will be an edge set with a single edge, containing all the 
     * edge points.
     */
    Edge_set(
        const std::vector<ivi_c::Edge_point>& edge_pts,
        int num_rows_in,
        int num_cols_in
    )
    :   m_edge_set(0)
    {
        using ivi_c::Malloc_size;
        // This ifdef makes it work in production mode =)
#ifdef TRACK_MEMORY_ALLOCATION
        using ivi_c::debug_ivi_malloc;
#else
        using ivi_c::ivi_malloc;
#endif
        ivi_c::Edge_set* edge_set = static_cast<ivi_c::Edge_set*>(ivi_malloc(sizeof(ivi_c::Edge_set)));
        edge_set->num_edges = 1;
        edge_set->total_num_pts = edge_pts.size();
        edge_set->edges = static_cast<ivi_c::Edge*>(ivi_malloc(sizeof(ivi_c::Edge)));
        edge_set->edges[0].num_points = edge_pts.size();
        edge_set->edges[0].points = static_cast<ivi_c::Edge_point*>(ivi_malloc(sizeof(ivi_c::Edge_point) * edge_pts.size()));
        edge_set->num_rows = num_rows_in;
        edge_set->num_cols = num_cols_in;

        for(size_t i = 0; i < edge_pts.size(); i++)
        {
            edge_set->edges[0].points[i] = edge_pts[i];
        }

        m_edge_set = edge_set;
    }

        
    ~Edge_set()
    {
        ivi_c::free_edge_set(m_edge_set);
    }

    Edge_set& operator=(const Edge_set& other)
    {
        Edge_set tmp(other);
        swap(tmp);
        return *this;
    }

    /** Access edge points by edge index */
    ivi_c::Edge_point& operator()(size_t edge_num, size_t point_num)
    {
        assert(m_edge_set);
        assert(edge_num < m_edge_set->num_edges);
        assert(point_num < m_edge_set->edges[edge_num].num_points);
        return m_edge_set->edges[edge_num].points[point_num];
    }

    /** Access edge points by edge index */
    const ivi_c::Edge_point& operator()(size_t edge_num, size_t point_num) const
    {
        assert(m_edge_set);
        assert(edge_num < m_edge_set->num_edges);
        assert(point_num < m_edge_set->edges[edge_num].num_points);
        return m_edge_set->edges[edge_num].points[point_num];
    }

    /** @brief Swaps the content of this edgeset with the edgeset provided
     * in input */
    void swap(Edge_set& other)
    {
        using std::swap;
        swap(m_edge_set, other.m_edge_set);
    }

    /** Access edge points sequentially, with index 0 being the first point of the first edge */
    ivi_c::Edge_point& operator[](size_t i)
    {
        assert(m_edge_set);
        assert(i < m_edge_set->total_num_pts);
        return m_edge_set->edges[0].points[i];
    }

    /** Access edge points sequentially, with index 0 being the first point of the first edge */
    const ivi_c::Edge_point& operator[](size_t i) const
    {
        assert(m_edge_set);
        assert(i < m_edge_set->total_num_pts);
        return m_edge_set->edges[0].points[i];
    }

    /** number of edges in set */
    size_t num_edges() const
    {
        if(!m_edge_set)
            return 0;

        return m_edge_set->num_edges;
    }

    /** Alias of num_edges() to help comply with a random-access container concept */
    size_t size() const
    {
        return get_total_edge_points();
    }

    /** @brief returns the ith edge */
    Edge get_edge(unsigned int i) const
    {
        return Edge(&m_edge_set->edges[i]);
    }

    /** @brief clear the previous edge set and makes this class point to the input c edge set */
    void set_edge_set(ivi_c::Edge_set* iedge_set)
    {
        ivi_c::free_edge_set(m_edge_set);
        m_edge_set = iedge_set;
    }

    /** @brief returns the number of rows in the image the edges were detected from */
    size_t num_rows() const { return m_edge_set->num_rows; }

    /** @brief alias of num_rows() to comply with other IVI dimensioned objects. */
    inline size_t get_num_rows() const { return num_rows(); }

    /** @brief returns the number of columns in the image the edges were detected from */
    size_t num_cols() const { return m_edge_set->num_cols; }

    /** @brief alias of num_cols() to comply with other IVI dimensioned objects. */
    inline size_t get_num_cols() const { return num_cols(); }

    /** number of edge points in edge i */
    size_t edge_length(size_t i_edge) const
    {
        assert(m_edge_set);
        return m_edge_set->edges[i_edge].num_points;
    }

    /** total number of edge points over all edges in set */
    size_t get_total_edge_points() const
    {
        if(!m_edge_set)
            return 0;

        return m_edge_set->total_num_pts;
    }

    /** @brief Removes from the edge set all the edges whose length is shorter than min_length*/
    void remove_short_edges(uint32_t min_length)
    {
        ivi_c::remove_short_edges(m_edge_set, min_length);
    }

    /** @brief Finds the index where the Edge pointed by iedge is stored in the
     *         edge_set. Notice that this is based on pointers (the pointer
     *         address is compared, not the edge content)
     *
     *  @param iedge the edge to find in the edge_set
     */
    unsigned int find_index(const ivi::Edge & iedge) const
    {
        unsigned index;
        int rc = ivi_c::find_edge_index(m_edge_set, iedge._c_ptr(), &index);
        if (rc != ivi_c::NO_ERROR)
        {
            IVI_THROW_2(Illegal_argument, "Bad edge index");
        }
        return index;
    }

    /** @brief Removes the edge at index edge_id
     *
     *  @param iindex the index of the edge to remove
     */
    void remove_edge(unsigned int iindex)
    {
        if( ivi_c::remove_edge(m_edge_set, iindex) != ivi_c::NO_ERROR)
        {
            IVI_THROW_2(Illegal_argument, "Bad edge index");
        }
    }

    /**
     * @brief For each edge, it finds the edge point with the largest gradient
     * difference on either side, breaks the edge, and recurses on the two newly created edges.
     *
     * @param  edges    Edge set to break apart.
     * @param  i        The index in the edge set to recurse on.
     * @param  thresh   If the dot product of the averaged vectors is less than
     *                  this threshold, the edge is broken.
     * @param  num_avg  Number of gradient vectors to average into one when
     *                  computing the dot product.
     */
    void break_edges_at_corners(float thresh, uint32_t num_avg)
    {
        ivi_c::break_edges_at_corners(m_edge_set, thresh, num_avg);
    }


    /** @brief Randomly samples a subset of edge points from this set. */
    void sample_edge_set(float p)
    {
        using std::swap;

        ivi_c::Edge_set* c_result = NULL;
        ivi_c::sample_edge_set(&c_result, m_edge_set, p);
        swap(c_result, m_edge_set);
        ivi_c::free_edge_set(c_result);
    }

    /** @brief draws the edge with index edge_index onto the input image,
     * using color (r,g,b). This function expects
     * color values between 0 and 255
     */
    void draw_edge(Image & img, float r, float g, float b, unsigned int edge_index) const
     {
        if(edge_index >= num_edges())
        {
            throw ivi::Illegal_argument("Edge set, color edge, edge index out of bounds");
        }
        Edge(&m_edge_set->edges[edge_index]).draw(img, r, g, b);
     }

    /** @brief draws the edge with index edge_index onto the input image,
     * using a random color */
    void randomly_color_edge(Image & img, unsigned int edge_index) const
    {
        if(edge_index >= num_edges())
        {
            throw ivi::Illegal_argument("Edge set, randomly color edge, edge index out of bounds");
        }
        Edge(&m_edge_set->edges[edge_index]).randomly_color(img);
    }

    /** @brief draws all the edges onto the input image,
     * using color (r,g,b). This function expects
     * color values between 0 and 255
     */
    void draw(Image& img, float r, float g, float b) const
    {
        // since we can't get a non-const pointer to the underlying IVI_image, we have to store the result in a new IVI_image, and swap it into the input image.
        
        ivi_c::Pixel pixel;
        pixel.r = r;
        pixel.g = g;
        pixel.b = b;
        ivi_c::IVI_image * out = img.non_const_c_ptr();
        ETX(ivi_c::color_edge_set(&out, out, m_edge_set, & pixel));

    }

    /** @brief draws all the edges onto the input image,
     * using color (r,g,b). Each edge is drawn using a different
     * random color
     */
    void randomly_color(Image& img)
    {
        ivi_c::IVI_image* out = NULL;

        ETX(ivi_c::randomly_color_edge_set(&out, img.c_ptr(), m_edge_set));
        Image tmp(out); // this will handle freeing the c-style image
        img.swap(tmp);
    }

    /** @brief saves this edge set on a file */
    void save(const char* fname) const
    {
        assert(m_edge_set);
        ETX(ivi_c::write_edge_set(m_edge_set, fname));
    }

    /** @brief loads this edge set from a file */
    void load(const char* fname)
    {
        ETX(ivi_c::read_edge_set(&m_edge_set, fname));
    }


    /** @brief writes this edge set to a file */
    void write(const char* fname) const
    {
        save(fname);
    }


    /** @brief reads this edge set from a file */
    void read(const char* fname)
    {
        load(fname);
    }

    /** @brief For debug purposes, checks that all the c++ wrappers are
     * consistent with the underlying c structures
     */
     bool is_edge_set_consistenct();


    /** @brief Returns a const pointer to the underlying c structure */
    const ivi_c::Edge_set* c_ptr() const { return m_edge_set; }

private:
    //C++ Wrappers for the underlying c edge_set structure
    ivi_c::Edge_set* m_edge_set;

private:
#ifdef IVI_HAVE_BST_SERIAL
    friend class boost::serialization::access;


    /** boost::Serialize compatible serialization */
    template <class Archive>
    void save(Archive& ar, const unsigned int /* version */) const
    {
        char* buffer = 0;
        size_t length;

        ivi_c::serialize_edge_set(m_edge_set, &buffer, &length);
        std::string str_tmp(buffer);
        ar & str_tmp;

        ivi_c::ivi_free(buffer);
    }

    /** boost::Serialize compatible serialization */
    template <class Archive>
    void load(Archive& ar, const unsigned int /* version */)
    {
        std::string buffer;
        ar & buffer;
        ivi_c::unserialize_edge_set(&m_edge_set, buffer.c_str());
    }
    BOOST_SERIALIZATION_SPLIT_MEMBER()
#endif /* IVI_HAVE_BST_SERIAL */

};

inline void load(Edge_set& edges, const std::string& fname)
{
    edges.load(fname.c_str());
}

inline void save(const Edge_set& edges, const std::string& fname)
{
    edges.save(fname.c_str());
}

typedef boost::shared_ptr<Edge_set> Edge_set_ptr;

/**
 * @brief Implements the Canny edge detection algorithm
 *
 * See http://en.wikipedia.org/wiki/Canny_edge_detector.
 *
 * We get rid of noise by first convolving the image with a Gaussian
 * filter with standard deviation sigma. A gradient map is then computed
 * using finite differences. The first threhsold is used to determine
 * edge pixels and non edge pixels (non maxima suppression).
 * We build edges by starting from edge pixels and following the
 * gradient direction until we find a pixel whose gradient is lower than
 * the second threshold (hysteresis).
 */
class Canny_edge_detector
{
public:

    /**
     * @param sigma     Gaussian blurring sigma.  Determines scale of edges to
     *                  detect.
     *
     * @param begin_thresh  Starting edge threshold hysteresis in the Canny
     *                  edge detection algorithm. Lower value gives more edges.
     *
     * @param end_thresh    Ending edge threshold for hysteresis in the Canny
     *                  edge detection algorithm.  Lower value gives longer
     *                  edges.  If it exceeds begin_thresh it isn't really
     *                  hysteresis.
     *
     * @param padding   Amount of padding to add to images before detecting
     *                  edges.  Images are padded by repeating the values
     *                  occurring at image boundaries.  Positive padding
     *                  can prevent spurious edge detections on image boundary.
     *                  If using, consider a value of around 2*sigma.
     *
     * @param use_fourier   Specifies whether to use Fast Fourier transform for
     *                  convolution or not.
     *
     * Note that each argument has a default value.
     */
    Canny_edge_detector(
            float sigma = 1,
            float begin_thresh = 0,
            float end_thresh = 0,
            size_t padding = 0,
            bool   use_fourier = true) :
        m_sigma(sigma),
        m_begin_thresh(begin_thresh),
        m_end_thresh(end_thresh),
        m_padding(padding),
        m_fourier(use_fourier)
    {}

    Canny_edge_detector& operator=(const Canny_edge_detector& other)
    {
        Canny_edge_detector tmp(other);
        swap(tmp);
        return *this;
    }

    /** Swaps the content of this edge detector with
     * the edge detector provided in input */
    void swap(Canny_edge_detector& other)
    {
        using std::swap;
        swap(m_sigma, other.m_sigma);
        swap(m_begin_thresh, other.m_begin_thresh);
        swap(m_end_thresh, other.m_end_thresh);
        swap(m_padding, other.m_padding);
        swap(m_fourier, other.m_fourier);
    }
            

    /** 
     * Detect edges in img 
     *
     * The design decision to return a pointer is motivated
     * by the fact that many objects may share this data.
     *
     * @param img The image to run the edge detector on
     * @param noiseless_data Set to true if the image is noise free,
     *        meaning that each pixel whose intensity is bigger than
     *        zero is an edge point
     * */
    Edge_set_ptr operator()(
        const Image& img,
        bool noiseless_data = false
    )   const
    {
        ivi_c::Edge_set* edge_set = NULL;
        ETX(ivi_c::detect_image_edge_set(
                &edge_set,
                img.c_ptr(),
                m_sigma,
                m_begin_thresh,
                m_end_thresh,
                m_padding, 
                m_fourier,
                noiseless_data));

        // wrap c struct in c++ object
        return Edge_set_ptr(new Edge_set(edge_set));
    }

    /**
    * @brief Detect edges in img
    *
    * This returns a pointer. It is up to the user to clean
    * up the memory to which it points.
    *
    * @param img. The image to run the edge detection on
    * @param noiseless_data Set to true if the image is noise free,
    *        meaning that each pixel whose intensity is bigger than
    *        zero is an edge point
    * */
    ivi::Edge_set* detect_edges(const Image& img, bool noiseless_data = false)
    {

        ivi_c::Edge_set* edge_set = NULL;
        ETX(ivi_c::detect_image_edge_set(
                &edge_set,
                img.c_ptr(),
                m_sigma,
                m_begin_thresh,
                m_end_thresh,
                m_padding,
                m_fourier,
                noiseless_data));

        // wrap c struct in c++ object
        return new Edge_set(edge_set);
    }

private:

    /// @brief The standard deviation of the Gaussian using to blur the image
    float m_sigma;

    /// @brief The beginning threshold for the Canny algorithm
    float m_begin_thresh;

    /// @brief The end threshold for the Canny algorithm
    float m_end_thresh;

    /// @brief The amount of padding to add to the image before edge detection
    size_t m_padding;

    /// @brief specifies whether to use fast Fourier transform to convolve
    bool  m_fourier;
};



// ///////////////////
// Global Functions //
// ///////////////////


/**
 * Create a black-and-white image from a set of edges.  Background will be
 * white, edges will be black.
 *
 * @param invert background is black, edges are white
 * @param remove_borders    fill an n-pixel frame around image with background
 *                          color (useful for removing artifact edges)
 * @author Kyle Simek
 */
Image edges_to_image(
    const Edge_set& edges,
    bool invert = false,
    size_t remove_borders = 0
);

/** 
 * Convert a binary image to an edge set.  
 *
 * @note    No attempt is made to group edge points into edges;  
 *          if you need such funtionality, I suggest creating a
 *          group_edge_points method or global function that receives the
 *          output of this function.
 *
 * @param i The image to convert.  Any pixels not (0,0,0) are considered edge
 *          points.
 * @param oriented  Whether or not to determine edge orientation (enabling is
 *                  slower).
 *
 * @return An edge set representing the image edges.  
 *
 * @note calling edge_image_to_edges(img, b).color(white_image, 0.0,0.0,0.0)
 *      should result in white image == img.  This has not been tested, though.
 *
 * @author Kyle Simek
 *
 */
Edge_set_ptr edge_image_to_edge_points(const Image& i, bool oriented = false);


} // namespace ivi
#endif
