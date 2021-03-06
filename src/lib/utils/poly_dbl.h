/*
* (C) 2017 Jack Lloyd
*
* Botan is released under the Simplified BSD License (see license.txt)
*/

#ifndef BOTAN_POLY_DBL_H__
#define BOTAN_POLY_DBL_H__

#include <botan/types.h>

namespace Botan {

/**
* Polynomial doubling in GF(2^n)
*/
void BOTAN_DLL poly_double_n(uint8_t out[], const uint8_t in[], size_t n);

inline void poly_double_n(uint8_t buf[], size_t n)
   {
   return poly_double_n(buf, buf, n);
   }

/*
* Little endian convention - used for XTS
*/
void poly_double_n_le(uint8_t out[], const uint8_t in[], size_t n);

}

#endif
