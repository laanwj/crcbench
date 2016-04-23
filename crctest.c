/* gcc crctest.c -o crctest -O2 -march=armv8-a+crc */
#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <arm_acle.h>
#include <assert.h>
/* crc32c using special instructions */

// Reverses (reflects) bits in a 32-bit word.
unsigned reverse(unsigned x) {
   x = ((x & 0x55555555) <<  1) | ((x >>  1) & 0x55555555);
   x = ((x & 0x33333333) <<  2) | ((x >>  2) & 0x33333333);
   x = ((x & 0x0F0F0F0F) <<  4) | ((x >>  4) & 0x0F0F0F0F);
   x = (x << 24) | ((x & 0xFF00) << 8) |
       ((x >> 8) & 0xFF00) | (x >> 24);
   return x;
}

/* This is the basic CRC algorithm with no optimizations. It follows the
logic circuit as closely as possible. */
// ----------------------------- crc32a --------------------------------
const uint32_t POLY_CRC32 = 0x04C11DB7;
const uint32_t POLY_CRC32C = 0x1EDC6F41;

unsigned int crc32a(const unsigned char *message, size_t size) {
   size_t i;
   int j;
   unsigned int byte, crc;

   i = 0;
   crc = 0xFFFFFFFF;
   while (i<size) {
      byte = message[i];            // Get next byte.
      byte = reverse(byte);         // 32-bit reversal.
      for (j = 0; j <= 7; j++) {    // Do eight times.
         if ((int)(crc ^ byte) < 0)
              crc = (crc << 1) ^ POLY_CRC32;
         else crc = crc << 1;
         byte = byte << 1;          // Ready next msg bit.
      }
      i = i + 1;
   }
   return reverse(~crc);
}

unsigned int crc32a_arm(const unsigned char *message, size_t size) {
   size_t i;
   uint32_t state = 0xffffffff;
   for (size_t i=0; i<size; ++i) {
        state = __crc32b (state, message[i]);
   }
   return state ^ 0xffffffff;
}

// ----------------------------- crc32c --------------------------------
unsigned int crc32c(const unsigned char *message, size_t size) {
   size_t i;
   int j;
   unsigned int byte, crc;

   i = 0;
   crc = 0xFFFFFFFF;
   while (i<size) {
      byte = message[i];            // Get next byte.
      byte = reverse(byte);         // 32-bit reversal.
      for (j = 0; j <= 7; j++) {    // Do eight times.
         if ((int)(crc ^ byte) < 0)
              crc = (crc << 1) ^ POLY_CRC32C;
         else crc = crc << 1;
         byte = byte << 1;          // Ready next msg bit.
      }
      i = i + 1;
   }
   return reverse(~crc);
}

unsigned int crc32c_arm(const unsigned char *message, size_t size) {
   size_t i;
   uint32_t state = 0xffffffff;
   for (size_t i=0; i<size; ++i) {
        state = __crc32cb (state, message[i]);
   }
   return state ^ 0xffffffff;
}

/* 0xcbf43926 crc32 */
/* 0xe3069283 crc32c */
const char *testvec = "123456789";

int main()
{
    uint32_t crc;
    crc = crc32a(testvec, strlen(testvec));
    printf("CRC32[%s] sw → %08x\n", testvec, crc);
    assert(crc == 0xcbf43926);
    crc = crc32a_arm(testvec, strlen(testvec));
    printf("CRC32[%s] hw → %08x\n", testvec, crc);
    assert(crc == 0xcbf43926);
    crc = crc32c(testvec, strlen(testvec));
    printf("CRC32C[%s] sw → %08x\n", testvec, crc);
    assert(crc == 0xe3069283);
    crc = crc32c_arm(testvec, strlen(testvec));
    printf("CRC32C[%s] hw → %08x\n", testvec, crc);
    assert(crc == 0xe3069283);
    return 0;
}

