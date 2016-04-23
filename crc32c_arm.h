#ifndef STORAGE_LEVELDB_UTIL_CRC32C_H_ARM_
#define STORAGE_LEVELDB_UTIL_CRC32C_H_ARM_

#include <stddef.h>
#include <stdint.h>
#include <arm_acle.h>

namespace crc32c_arm {

// Return the crc32c of concat(A, data[0,n-1]) where init_crc is the
// crc32c of some string A.  Extend() is often used to maintain the
// crc32c of a stream of data.
uint32_t Extend(uint32_t init_crc, const char* data, size_t n)
{
   size_t i;
   uint32_t state = init_crc ^ 0xffffffff;
   /** TODO: would it help speed to align to word boundary first? */
   for (; n>=8; n-=8) {
        state = __crc32cd (state, *((uint64_t*)data));
        data += 8;
   }
   if (n >= 4) {
        state = __crc32cw (state, *((uint32_t*)data));
        data += 4;
   }
   if (n >= 2) {
        state = __crc32ch (state, *((uint16_t*)data));
        data += 2;
   }
   if (n)
        state = __crc32cb (state, *((uint8_t*)data));
   return state ^ 0xffffffff;
}

// Return the crc32c of data[0,n-1]
inline uint32_t Value(const char* data, size_t n) {
  return Extend(0, data, n);
}

static const uint32_t kMaskDelta = 0xa282ead8ul;

// Return a masked representation of crc.
//
// Motivation: it is problematic to compute the CRC of a string that
// contains embedded CRCs.  Therefore we recommend that CRCs stored
// somewhere (e.g., in files) should be masked before being stored.
inline uint32_t Mask(uint32_t crc) {
  // Rotate right by 15 bits and add a constant.
  return ((crc >> 15) | (crc << 17)) + kMaskDelta;
}

// Return the crc whose masked representation is masked_crc.
inline uint32_t Unmask(uint32_t masked_crc) {
  uint32_t rot = masked_crc - kMaskDelta;
  return ((rot >> 17) | (rot << 15));
}

}  // namespace crc32c

#endif  // STORAGE_LEVELDB_UTIL_CRC32C_H_ARM_
