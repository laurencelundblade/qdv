/* ==========================================================================
 * more_tests.c -- linkage to extra tests in qdv repo
 *
 * Copyright (c) 2026, Laurence Lundblade. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * See BSD-3-Clause license in file named "LICENSE"
 *
 * Created on 7/2/26
 * ========================================================================= */

#ifndef MORE_TESTS_H
#define MORE_TESTS_H

#include "run_tests.h"

#include "more_float_tests.h"

#include "stddef.h"

/* A table of tests that are run by run_tests.c */
/* These are tests that are too fussy, long-running
 * or have license and tool issues to be part of the
 * main repo. */
test_entry more_tests[] = {
    TEST_ENTRY(HalfPrecisionAgainstRFCCodeTest),
    {NULL, NULL, false}
};


#endif /* ! MORE_TESTS_H */
