/*==============================================================================
 decode_min.c -- Used for code size measurement of minimum size of decoder.

 Copyright (c) 2020, Laurence Lundblade. All rights reserved.

 SPDX-License-Identifier: BSD-3-Clause

 See BSD-3-Clause license in README.md

 =============================================================================*/


#include "qcbor/qcbor_decode.h"


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
 This calls just the basic decode functions so that only a minimum of the
 decode library will be linked so the object code size of the smallest decoder
 can be measured.
 */
int main(int argc, const char * argv[])
{
    (void)argc; // Suppress unused warning
    (void)argv; // Suppress unused warning

    static const uint8_t min[] = {0x81, 0xf5};

    // Decode it and see that is right
    QCBORDecodeContext DC;
    QCBORItem Item;
    QCBORDecode_Init(&DC, UsefulBuf_FROM_BYTE_ARRAY_LITERAL(min), QCBOR_DECODE_MODE_NORMAL);

    // So first_symbol_x isn't dead-stripped
    Item.uDataType = first_symbol_1();
    Item.val.string.ptr = first_symbol_2;

    QCBORDecode_GetNext(&DC, &Item);
    if(Item.uDataType != QCBOR_TYPE_ARRAY) {
        return 1;
    }

    QCBORDecode_GetNext(&DC, &Item);
    if(Item.uDataType != QCBOR_TYPE_TRUE) {
        return 2;
    }

    if(QCBORDecode_Finish(&DC)) {
        return 3;
    }

    return 0;
}
