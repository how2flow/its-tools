/* SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (C) 2019 ARM Limited, All Rights Reserved.
 * Copyright (C) 2025 Steve Jeong <steve@how2flow.net>
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

/* About ITS */
#include "its.h"
#include "its_tools.h"


void translate_its_cmd(uint8_t *cmd)
{
	printf("ITS ConvertCommand:: Wrote command with: \n");
	printf("DW0: %02x %02x %02x %02x %02x %02x %02x %02x\n", cmd[7],  cmd[6],  cmd[5],  cmd[4],  cmd[3],  cmd[2],  cmd[1],  cmd[0]);
	printf("DW1: %02x %02x %02x %02x %02x %02x %02x %02x\n", cmd[15], cmd[14], cmd[13], cmd[12], cmd[11], cmd[10], cmd[9],  cmd[8]);
	printf("DW2: %02x %02x %02x %02x %02x %02x %02x %02x\n", cmd[23], cmd[22], cmd[21], cmd[20], cmd[19], cmd[18], cmd[17], cmd[16]);
	printf("DW0: %02x %02x %02x %02x %02x %02x %02x %02x\n", cmd[31], cmd[30], cmd[29], cmd[28], cmd[27], cmd[26], cmd[25], cmd[24]);
}
