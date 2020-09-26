/*==============================================================================
 encode_max.c -- Used for code size measurement of max size of encoder.

 Copyright (c) 2020, Laurence Lundblade. All rights reserved.

 SPDX-License-Identifier: BSD-3-Clause

 See BSD-3-Clause license in README.md

 =============================================================================*/


#include "qcbor/qcbor_encode.h"


/* These two symbols hopefully appear at the start and
 allow use of nm(1) to list symbols in order so the
 sizes of the functions can be computed relative to the
 previous symbol.
 */
int first_symbol_1() {
    return 7;
}

static const uint8_t first_symbol_2[] = {0x55, 0x66};


/*
 This calls enough encode functions so that everything in the encode
 library will be linked so the object code size of the full encoder
 can be measured.
 */
int main(int argc, const char * argv[])
{
    (void)argc; // Suppress unused warning
    (void)argv; // Suppress unused warning

    uint8_t pBuf[300];
    UsefulBufC x;
    // Very simple CBOR, a map with one boolean that is true in it
    QCBOREncodeContext EC;

    QCBOREncode_Init(&EC, UsefulBuf_FROM_BYTE_ARRAY(pBuf));

    QCBOREncode_OpenArray(&EC);
    QCBOREncode_OpenMap(&EC);
    QCBOREncode_AddInt64ToMapN(&EC, 66, -999999999);
    QCBOREncode_AddUInt64ToMapN(&EC, 66, -999999999);
    QCBOREncode_BstrWrap(&EC);
    QCBOREncode_AddTag(&EC, 99);
    QCBOREncode_AddFloatNoPreferred(&EC, 99.999f);
    QCBOREncode_AddDoubleToMapN(&EC, 66, 99.9999999);
    QCBOREncode_CloseBstrWrap2(&EC,  true, &x);
    QCBOREncode_AddSZStringToMapN(&EC, 888, "hi there");
    QCBOREncode_OpenArrayIndefiniteLength(&EC);
    QCBOREncode_AddBigFloatToMapN(&EC, 10, 10, 10);
    QCBOREncode_CloseArrayIndefiniteLength(&EC);
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
