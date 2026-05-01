/* ==========================================================================
 * qcbor_config.h -- common ifdef config for debugging
 *
 * Copyright (c) 2026, Laurence Lundblade.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Created by Laurence Lundblade on 4/30/26.
 * ========================================================================== */


#ifndef qcbor_config_h
#define qcbor_config_h

//#define QCBOR_DISABLE_FLOAT_HW_USE
//#define QCBOR_DISABLE_PREFERRED_FLOAT
//#define QCBOR_CONFIG_DISABLE_EXP_AND_MANTISSA
//#define QCBOR_DISABLE_ENCODE_USAGE_GUARDS
//#define QCBOR_DISABLE_INDEFINITE_LENGTH_STRINGS
//#define QCBOR_DISABLE_INDEFINITE_LENGTH_ARRAYS
//#define QCBOR_DISABLE_UNCOMMON_TAGS
//#define USEFULBUF_DISABLE_ALL_FLOAT
//#define QCBOR_DISABLE_TAGS
//#define QCBOR_DISABLE_NON_INTEGER_LABELS
//#define QCBOR_DISABLE_ALL

#ifdef QCBOR_DISABLE_ALL

#define QCBOR_DISABLE_FLOAT_HW_USE
#define QCBOR_DISABLE_PREFERRED_FLOAT
#define QCBOR_CONFIG_DISABLE_EXP_AND_MANTISSA
#define QCBOR_DISABLE_ENCODE_USAGE_GUARDS
#define QCBOR_DISABLE_INDEFINITE_LENGTH_STRINGS
#define QCBOR_DISABLE_INDEFINITE_LENGTH_ARRAYS
#define QCBOR_DISABLE_UNCOMMON_TAGS
#define USEFULBUF_DISABLE_ALL_FLOAT
#define QCBOR_DISABLE_TAGS
#define QCBOR_DISABLE_NON_INTEGER_LABELS

#endif


#endif /*qcbor_config_h */
