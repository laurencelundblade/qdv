/*==============================================================================
 decode_max.c -- Used for code size measurement of max size of decoder.

 Copyright (c) 2020, Laurence Lundblade. All rights reserved.

 SPDX-License-Identifier: BSD-3-Clause

 See BSD-3-Clause license in README.md

 =============================================================================*/


#include "qcbor/qcbor_decode.h"
#include "qcbor/qcbor_spiffy_decode.h"

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
 This calls enough decode functions so that everything in the decode
 library will be linked so the object code size of the full decoder
 can be measured.
 */
int main(int argc, const char * argv[])
{
    (void)argc; // Suppress unused warning
    (void)argv; // Suppress unused warning


    static const uint8_t min[] = {0x81, 0xf5};

    QCBORTagListOut TL;

    UsefulBuf_MAKE_STACK_UB(Pool, 100);

    // Decode it and see that is right
    QCBORDecodeContext DC;
    QCBORItem Item;

    // So first_symbol_x isn't dead-stripped
    Item.uDataType = first_symbol_1();
    Item.val.string.ptr = first_symbol_2;

    QCBORDecode_SetCallerConfiguredTagList(&DC, NULL);

    QCBORDecode_SetMemPool(&DC, Pool, true);

    QCBORDecode_Init(&DC, UsefulBuf_FROM_BYTE_ARRAY_LITERAL(min), QCBOR_DECODE_MODE_NORMAL);

    QCBORDecode_GetNextWithTags(&DC, &Item, &TL);
    if(Item.uDataType != QCBOR_TYPE_MAP) {
        return -2;
    }

    QCBORDecode_GetNext(&DC, &Item);
    if(Item.uDataType != QCBOR_TYPE_TRUE) {
        return -3;
    }

    // This won't actually run correctly, but does link correctly
    int64_t nInt;
    uint64_t uInt;
    double dDoub;
    QCBORDecode_GetInt64(&DC, &nInt);

    QCBORDecode_GetInt64ConvertAll(&DC, 0xff, &nInt);

    QCBORDecode_GetUInt64ConvertAll(&DC, 0xff, &uInt);

    QCBORDecode_GetDoubleConvertAll(&DC, 0xff, &dDoub);

    UsefulBufC UB;
    QCBORDecode_GetByteStringInMapN(&DC, 9, &UB);


    bool b;
    QCBORDecode_GetBool(&DC, &b);

    QCBORDecode_GetEpochDate(&DC, 0, &nInt);

    QCBORDecode_GetBignumInMapN(&DC, 0, 0, &UB, &b);

    QCBORDecode_GetDecimalFraction(&DC, 0, &nInt, &nInt);

    UsefulBuf B = {NULL, 0};
    QCBORDecode_GetBigFloatBig(&DC, 0, B, &UB, &b, &nInt);
    
    QCBORDecode_IsTagged(&DC, &Item, 5);

    QCBORDecode_EnterMapFromMapSZ(&DC, "map");
    QCBORDecode_ExitMap(&DC);

    QCBORDecode_EnterArrayFromMapSZ(&DC, "map");
    QCBORDecode_ExitArray(&DC);

    QCBORDecode_EnterBstrWrapped(&DC, 0, &UB);
    QCBORDecode_ExitBstrWrapped(&DC);

    QCBORDecode_GetItemInMapN(&DC, 0, 0, &Item);

    QCBORDecode_GetItemsInMap(&DC, &Item);
    
    if(QCBORDecode_Finish(&DC)) {
        return -4;
    }

    return 0;
}
