#ifndef GR_SPHERE
#define GR_SPHERE
#include <m_cpp/m_vector.h>
#include <m_cpp/m_matrix.h>
#include "m/m_incl.h"
#include <i_cpp/i_image.h>
#include <i/i_draw.h>

namespace ivi
{

class Sphere{
public:
    double GetRadius() const;
    const ivi::Vector & GetCenter() const;
    //Contructor for radius & center
    Sphere(ivi::Vector & center, double radius);
    Sphere();
    void drawSphere(void);
    static void init();
private:
    double radius;
    ivi::Vector center;
};

}
#endif
