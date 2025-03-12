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


uint32_t get_its_reg(uint64_t addr)
{
	int fd;
	void *map_base;
	uint32_t ret;

	/* Memory mapping */
	fd = open("/dev/mem", O_RDONLY | O_SYNC);
	if (fd < 0) {
		printf("Open /dev/mem fail\n");
		exit(EXIT_FAILURE);
	}

	map_base = mmap(NULL, BLOCK_SIZE, PROT_READ, MAP_SHARED, fd, addr);
	if (map_base == MAP_FAILED) {
		printf("mmap fail\n");
		exit(EXIT_FAILURE);
	}

	ret = (*(volatile uint32_t *)map_base);

	/* free */
	munmap(map_base, sizeof(uint32_t));
	close(fd);

	return ret;
}

void set_its_reg(uint64_t addr, uint64_t val)
{
	int fd;
	void *map_base;
	uint64_t *target;

	/* Memory mapping */
	fd = open("/dev/mem", O_RDWR | O_SYNC);
	if (fd < 0) {
		printf("Open /dev/mem fail\n");
		exit(EXIT_FAILURE);
	}

	map_base = mmap(NULL, BLOCK_SIZE, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, addr);
	if (map_base == MAP_FAILED) {
		printf("mmap fail\n");
		exit(EXIT_FAILURE);
	}

	target = (uint64_t *)map_base;
	*target = val;

	/* free */
	munmap(map_base, sizeof(uint64_t));
	close(fd);
}

uint8_t select_its_tools_cmd_type(void)
{
	static uint8_t cmd_type;

	printf("Select ITS command\n");
	printf("[%d] INT\n",INT);
	printf("[%d] INV\n",INV);
	printf("[%d] INVALL\n",INVALL);
	printf("[%d] MAPC\n",MAPC);
	printf("[%d] MAPD\n",MAPD);
	printf("[%d] MAPTI\n",MAPTI);
	printf("[%d] SYNC\n",SYNC);
	printf("Select ITS command: ");
	if (scanf("%hhd", &cmd_type) != 1) {
		printf("Invalid arguments\n");
		exit(EXIT_FAILURE);
	}

	return cmd_type;
}

uint8_t select_its_tools_mode(void)
{
	static uint8_t mode;
	int ret;

	printf("Select Mode\n");
	if (scanf("%hhd", &mode) != 1) {
		printf("Invalid arguments\n");
		exit(EXIT_FAILURE);
	}

	return mode;
}
