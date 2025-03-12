/* SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (C) 2025 Steve Jeong <steve@how2flow.net>
 */

#ifndef _ITS_H
#define _ITS_H

#include <stdint.h>


#define ALIGN(x, a) (((x) + ((a) - 1)) & ~((a) - 1))

#define GITS_BASE 0x10040000
#define GITS_TYPER (GITS_BASE + 0x0008)

#endif /* _ITS_H */
