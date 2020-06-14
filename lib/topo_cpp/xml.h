/**
 * @file
 * @brief XML processing prototypes
 * @author Scott Morris
 * @author Alan Morris
 *
 * Originally from TopoFusion.  This code enables GPX input and output.
 */
/*
 * $Id: xml.h 25499 2020-06-14 13:26:04Z kobus $
 */

#ifndef XML_H_MORRISES_INCLUDED_UOFARIZONAVISION
#define XML_H_MORRISES_INCLUDED_UOFARIZONAVISION

#include <string>

#include <topo_cpp/layer.h>

namespace ivi
{
namespace TopoFusion
{

int readTrack_GPX( const std::string&, layer* );
int writeTrack_GPX( const std::string&, const layer& );

}
} // end namespace ivi

#endif
