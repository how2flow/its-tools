/* SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (C) 2019 ARM Limited, All Rights Reserved.
 * Copyright (C) 2025 Steve Jeong <steve@how2flow.net>
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* About ITS */
#include "its.h"
#include "its_tools.h"
/* About CMD */
#include "convert_its_cmd.h"


int main(void)
{
	uint8_t *cmd, cmd_type, mode;

	cmd = NULL;
	mode = select_its_tools_mode();
	cmd_type = select_its_tools_cmd_type();

	switch (mode) {
	default:
		printf("Invalid Mode\n");
		break;
	}

	return 0;
}
