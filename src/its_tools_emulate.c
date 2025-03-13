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

#define CMD_SIZE (32)


void emulate_its_cmd(uint8_t *cmd)
{
	uint64_t *dw, new_cwriter, q_base, q_writer, q_readr, q_offset;
	uint32_t q_size;
	uint8_t *entry;
	int i;

	dw = malloc(4 * sizeof(uint64_t));
	if (!dw) {
		printf("Memory alloc fail\n");
		exit(EXIT_FAILURE);
	}

	q_base = (get_its_reg(GITS_CBASER) & (uint64_t)(0x0000FFFFFFFFF000));
	q_size = ((get_its_reg(GITS_CBASER) & 0xFF) + 1) * 0x1000;
	q_writer = get_its_reg(GITS_CWRITER);
	q_readr = get_its_reg(GITS_CREADR);

	if (q_readr == 0)
		q_readr = q_size - CMD_SIZE;
	else
		q_readr = q_readr - CMD_SIZE;

	/* Check that the queue is not full */
	while (q_writer == q_readr)
	{}

	q_offset = (q_base + q_writer);
	for (i = 0; i < 4; i++) {
		memcpy(&dw[i], &cmd[i * 8], 8);
		set_its_reg(q_offset + (i * 8), dw[i]);
	}

	new_cwriter = q_writer + CMD_SIZE;
	if (new_cwriter == q_size)
		new_cwriter = 0;

	set_its_reg(GITS_CWRITER, new_cwriter);

	while (GITS_CREADR != GITS_CWRITER)
	{}

	printf("Complete Add ITS cmd!\n");
}
