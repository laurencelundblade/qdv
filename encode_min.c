/*==============================================================================
 encode_min.c -- Used for code size measurement of minimum size of encoder.

 Copyright (c) 2020, Laurence Lundblade. All rights reserved.

 SPDX-License-Identifier: BSD-3-Clause

 See BSD-3-Clause license in README.md

 =============================================================================*/


#include "qcbor/qcbor_encode.h"


/* These two symbols hopefully appear at the start and
   allow use of nm(1) to list symbols in order so the
   sizes of the functions can be computed relative to the
   previous symbol
 */
int first_symbol_1() {
    return 7;
}

static const uint8_t first_symbol_2[] = {0x55, 0x66};


/*
 This calls just the basic encode functions so that only a minimum of the
 encode library will be linked so the object code size of the smallest encoder
 can be measured.
 */
int main(int argc, const char * argv[])
{
    (void)argc; // Suppress unused warning
    (void)argv; // Suppress unused warning

    uint8_t pBuf[300];
    QCBOREncodeContext EC;

    QCBOREncode_Init(&EC, UsefulBuf_FROM_BYTE_ARRAY(pBuf));

    QCBOREncode_OpenArray(&EC);
    QCBOREncode_OpenMap(&EC);
    QCBOREncode_AddInt64ToMapN(&EC, 66, -999999999);
    QCBOREncode_AddSZStringToMapN(&EC, 888, "hi there");
    QCBOREncode_AddBoolToMapN(&EC, 66, true);
    QCBOREncode_CloseMap(&EC);
    QCBOREncode_CloseArray(&EC);

    UsefulBufC Encoded;

    // So first_symbol_x is not dead-stripped */
    Encoded.len = first_symbol_1();
    Encoded.ptr = first_symbol_2;

    if(QCBOREncode_Finish(&EC, &Encoded)) {
        return -1;
    }

    return 0;
}
