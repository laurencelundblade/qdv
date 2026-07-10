/* ==========================================================================
 * more_float_tests.c -- tests for float and conversion to/from half-precision
 *
 * Copyright (c) 2018-2025, Laurence Lundblade. All rights reserved.
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * See BSD-3-Clause license in file named "LICENSE"
 *
 * Created on from float_tests.c 7/2/26
 * ========================================================================= */


#include "qcbor/qcbor_main_decode.h"
#ifndef USEFULBUF_DISABLE_ALL_FLOAT
#include <math.h> /* For INFINITY and NAN and isnan() */
#endif /* ! USEFULBUF_DISABLE_ALL_FLOAT */

#include "half_to_double_from_rfc7049.h"

/* Public function. See more_float_tests.h */
/* Separated out because license of half_to_double_from_rfc7049 */
int32_t
HalfPrecisionAgainstRFCCodeTest(void)
{
   QCBORItem          Item;
   QCBORDecodeContext DC;
   unsigned char      pbHalfBytes[2];
   uint8_t            uHalfPrecInitialByte;
   double             d;
   UsefulBuf_MAKE_STACK_UB(EncodedBytes, 3);
   UsefulOutBuf      UOB;
   uint32_t          uHalfP;


   for(uHalfP = 0; uHalfP < 0xffff; uHalfP += 60) {
      pbHalfBytes[1] = (uint8_t)(uHalfP & 0xff);
      pbHalfBytes[0] = (uint8_t)(uHalfP >> 8); /* uHalfP is always less than 0xffff */
      d = decode_half(pbHalfBytes);

      /* Construct the CBOR for the half-precision float by hand */
      UsefulOutBuf_Init(&UOB, EncodedBytes);

      uHalfPrecInitialByte = (uint8_t)(HALF_PREC_FLOAT + (CBOR_MAJOR_TYPE_SIMPLE << 5)); /* 0xf9 */
      UsefulOutBuf_AppendByte(&UOB, uHalfPrecInitialByte); /* initial byte */
      UsefulOutBuf_AppendUint16(&UOB, (uint16_t)uHalfP);   /* argument */

      /* Now parse the hand-constructed CBOR. This will invoke the
       * conversion to a float
       */
      QCBORDecode_Init(&DC, UsefulOutBuf_OutUBuf(&UOB), QCBOR_DECODE_MODE_ALLOW_NAN_PAYLOADS);
      QCBORDecode_GetNext(&DC, &Item);
      if(Item.uDataType != QCBOR_TYPE_DOUBLE) {
         return -1;
      }

      if(isnan(d)) {
         /* The RFC code uses the native instructions which may or may not
          * handle sNaN, qNaN and NaN payloads correctly. This test just
          * makes sure it is a NaN and doesn't worry about the type of NaN
          */
         if(!isnan(Item.val.dfnum)) {
            return -3;
         }
      } else {
         if(Item.val.dfnum != d) {
            return -2;
         }
      }
   }
   return 0;
}
