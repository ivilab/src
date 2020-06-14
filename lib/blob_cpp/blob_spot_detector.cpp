/**
 * @file
 * @brief Def of function operator of class Spot_detector
 * @author Ernesto Brau
 */
/*
 * $Id: blob_spot_detector.cpp 25499 2020-06-14 13:26:04Z kobus $
 */

#include <l/l_int_matrix.h>
#include <blob_cpp/blob_spot_detector.h>
#include <m/m_vector.h>
#include <seg/seg_spots.h> /* decl. of find_bright_spots_in_image */

namespace
{

// RAII object automatically releases Vector_vector
struct VecVec
{
    ivi_c::Vector_vector* m_vvp;
    VecVec(ivi_c::Vector_vector* p) : m_vvp(p) {}
    ~VecVec() { ivi_c::free_vector_vector(m_vvp); }
};

// RAII object automatically releases Int_matrix_vector
struct IntMatVec
{
    ivi_c::Int_matrix_vector* m_imvp;
    IntMatVec(ivi_c::Int_matrix_vector* p) : m_imvp(p) {}
    ~IntMatVec() { ivi_c::free_int_matrix_vector(m_imvp); }
};

}

namespace ivi
{

const Spot_detector::Centroid_set& Spot_detector::operator()
(
    const Image& img
)   const
{
    // temporary containers for spot 
    ivi_c::Vector_vector* centroids = NULL;
    ivi_c::Int_matrix_vector* spots = NULL;

    // The key functionality is in this C function in lib/seg
    ETX(ivi_c::find_bright_spots_in_image(
                img.c_ptr(),
                m_background.get_c_matrix(),
                m_thresholds.get_c_matrix(),
                m_min_brightness,
                m_min_size,
                m_max_size,
                m_similarity,
                &spots,
                &centroids
            ));

    // RAII insurance that the above allocations will be freed.
    VecVec auto_centroids(centroids);
    IntMatVec auto_spots(spots);

	// copy centroids and spots into different containers
    spot_centroids.resize(centroids -> length, Vector(2, 0.0));
    spot_coordinates.resize(spots -> length);

    for (size_t i = 0; i < spot_centroids.size(); ++i)
    {
        spot_centroids[i].set(centroids->elements[i]->elements[0],
                              centroids->elements[i]->elements[1]);
        spot_coordinates[i].resize(spots->elements[i]->num_rows);

        for (size_t j = 0; j < spot_coordinates[i].size(); ++j)
        {
            spot_coordinates[i][j].first = spots->elements[i]->elements[j][0];
            spot_coordinates[i][j].second = spots->elements[i]->elements[j][1];
        }
    }

    return spot_centroids;
}

} //namespace ivi

