/* SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (C) 2019 ARM Limited, All Rights Reserved.
 * Copyright (C) 2025 Steve Jeong <steve@how2flow.net>
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

/* About ITS */
#include "its.h"
#include "its_tools.h"
/* About CMD */
#include "convert_its_cmd.h"


uint8_t *(*convert_its_cmd[])(void) = {
	NULL, /* padding */
	convert_INT,
	convert_INV,
	convert_INVALL,
	convert_MAPC,
	convert_MAPD,
	convert_MAPTI,
	convert_SYNC
};

uint8_t *convert_INT(void)
{
	uint32_t device_id, event_id;
	static uint8_t command[32];
	uint8_t i;

	/* Fill command with 0s */
	for (i=0; i<32; i++)
		command[i] = 0;

	/* Input command params */
	printf("Generate msi evnet!\n");
	printf("INT <DeviceID> <EventID>\n");
	printf("<DeviceID> <EventID>: ");
	if (scanf("%d %d",&device_id, &event_id) != 2) {
		printf("Invalied arguments\n");
		exit(EXIT_FAILURE);
	}

	/* Construct command */
	command[0]   = 0x3;
	command[1]   = 0x0;

	command[4]   = (uint8_t)(0xFF & device_id);
	command[5]   = (uint8_t)(0xFF & (device_id >> 8));
	command[6]   = (uint8_t)(0xFF & (device_id >> 16));
	command[7]   = (uint8_t)(0xFF & (device_id >> 24));

	command[8]   = (uint8_t)(0xFF & event_id);
	command[9]   = (uint8_t)(0xFF & (event_id >> 8));
	command[10]  = (uint8_t)(0xFF & (event_id >> 16));
	command[11]  = (uint8_t)(0xFF & (event_id >> 24));

	return command;
}

uint8_t *convert_INV(void)
{
	uint32_t device_id, event_id;
	static uint8_t command[32];
	uint8_t i;

	/* Fill command with 0s */
	for (i=0; i<32; i++)
		command[i] = 0;

	/* Input command params */
	printf("Ensure ITS some caching consistency!\n");
	printf("INV <DeviceID> <EventID>\n");
	printf("<DeviceID> <EventID>: ");
	if (scanf("%d %d",&device_id, &event_id) != 2) {
		printf("Invalied arguments\n");
		exit(EXIT_FAILURE);
	}

	/* Construct command */
	command[0]   = 0xC;
	command[1]   = 0x0;

	command[4]   = (uint8_t)(0xFF & device_id);
	command[5]   = (uint8_t)(0xFF & (device_id >> 8));
	command[6]   = (uint8_t)(0xFF & (device_id >> 16));
	command[7]   = (uint8_t)(0xFF & (device_id >> 24));

	command[8]   = (uint8_t)(0xFF & event_id);
	command[9]   = (uint8_t)(0xFF & (event_id >> 8));
	command[10]  = (uint8_t)(0xFF & (event_id >> 16));
	command[11]  = (uint8_t)(0xFF & (event_id >> 24));

	return command;
}

uint8_t *convert_INVALL(void)
{
	uint32_t cid;
	static uint8_t command[32];
	uint8_t i;

	/* Fill command with 0s */
	for (i=0; i<32; i++)
		command[i] = 0;

	/* Input command params */
	printf("Ensure ITS all caching consistency!\n");
	printf("INVALL <ICID>\n");
	printf("<ICID>: ");
	if (scanf("%d",&cid) != 1) {
		printf("Invalied arguments\n");
		exit(EXIT_FAILURE);
	}

	/* Construct command */
	command[0]   = 0xD;
	command[1]   = 0x0;

	command[16]  = (uint8_t)(0xFF & cid);
	command[17]  = (uint8_t)(0xFF & (cid >> 8));

	return command;
}

uint8_t *convert_MAPC(void)
{
	uint64_t its_typer;
	uint32_t cid, target;
	static uint8_t command[32];
	uint8_t i;

	/* Fill command with 0s */
	for (i=0; i<32; i++)
		command[i] = 0;

	/* Input command params */
	printf("Maps the collection table entry defined by ICID\n");
	printf("MAPC <ICID> <TARGET RD>\n");
	printf("<ICID> <TARGET RD>: ");
	if (scanf("%d %d", &cid, &target) != 2) {
		printf("Invalied arguments\n");
		exit(EXIT_FAILURE);
	}

	its_typer = get_its_reg(GITS_TYPER);

	/* Check whether "target" is PA or Processor Number */
	if ((its_typer & (1 << 19)) != 0)
		target = target >> 16;

	/* Construct command */
	command[0]  = 0x9;
	command[1]  = 0x0;

	command[16] = (uint8_t)(0xFF & cid);
	command[17] = (uint8_t)(0xFF & (cid >> 8));

	command[18] = (uint8_t)(0xFF & target);
	command[19] = (uint8_t)(0xFF & (target >> 8));
	command[20] = (uint8_t)(0xFF & (target >> 16));
	command[21] = (uint8_t)(0xFF & (target >> 24));

	command[23] = 0x80;  // valid bit

	return command;
}

uint8_t *convert_MAPD(void)
{
	size_t sz;
	uint64_t device_id, size, table;
	static uint8_t command[32];
	uint8_t i;

	/* Fill command with 0s */
	for (i=0; i<32; i++)
		command[i] = 0;

	/* Input command params */
	printf("Maps the Device table entry defined by ICID\n");
	printf("MAPD <DeviceID> <ITT_addr> <Size>\n");
	printf("<DeviceID> <ITT> <Size>: ");
	if (scanf("%ld %lx %ld", &device_id, &table, &size) != 3) {
		printf("Invalied arguments\n");
		exit(EXIT_FAILURE);
	}

	/* table post-process */
	table = (table >> 8);
	table = ALIGN(table, 0x100);

	/* size post-process */
	if (size > 0)
		size--;
	else
		exit(EXIT_FAILURE);

	/* Construct command */
	command[0]  = 0x8;
	command[1]  = 0x0;

	command[4]  = (uint8_t)(0xFF & device_id);
	command[5]  = (uint8_t)(0xFF & (device_id >> 8));
	command[6]  = (uint8_t)(0xFF & (device_id >> 16));
	command[7]  = (uint8_t)(0xFF & (device_id >> 24));

	command[8]  = (uint8_t)(0x3F & size);

	command[17] = (uint8_t)(0xFF & table);
	command[18] = (uint8_t)(0xFF & (table >> 8));
	command[19] = (uint8_t)(0xFF & (table >> 16));
	command[20] = (uint8_t)(0xFF & (table >> 24));
	command[21] = (uint8_t)(0xFF & (table >> 32));

	command[23] = 0x80;  // valid bit

	return command;
}

uint8_t *convert_MAPTI(void)
{
	uint32_t device_id, event_id, intid, cid;
	static uint8_t command[32];
	uint8_t i;

	/* Fill command with 0s */
	for (i=0; i<32; i++)
		command[i] = 0;

	/* Input command params */
	printf("Maps the event defined by EventID & DeviceID to its ITE\n");
	printf("MAPTI <DeviceID> <EventID> <pINTID> <ICID>\n");
	printf("<DeviceID> <EventID> <pINTID> <ICID>: ");
	if (scanf("%d %d %d %d", &device_id, &event_id, &intid, &cid) != 4) {
		printf("Invalied arguments\n");
		exit(EXIT_FAILURE);
	}

	/* Construct command */
	command[0]   = 0xA;
	command[1]   = 0x0;

	command[4]   = (uint8_t)(0xFF & device_id);
	command[5]   = (uint8_t)(0xFF & (device_id >> 8));
	command[6]   = (uint8_t)(0xFF & (device_id >> 16));
	command[7]   = (uint8_t)(0xFF & (device_id >> 24));

	command[8]   = (uint8_t)(0xFF & event_id);
	command[9]   = (uint8_t)(0xFF & (event_id >> 8));
	command[10]  = (uint8_t)(0xFF & (event_id >> 16));
	command[11]  = (uint8_t)(0xFF & (event_id >> 24));

	command[12]  = (uint8_t)(0xFF & intid);
	command[13]  = (uint8_t)(0xFF & (intid >> 8));
	command[14]  = (uint8_t)(0xFF & (intid >> 16));
	command[15]  = (uint8_t)(0xFF & (intid >> 24));

	command[16]  = (uint8_t)(0xFF & cid);
	command[17]  = (uint8_t)(0xFF & (cid >> 8));

	return command;
}

uint8_t *convert_SYNC(void)
{
	uint64_t target;
	static uint8_t command[32];
	uint8_t i;

	/* Fill command with 0s */
	for (i=0; i<32; i++)
		command[i] = 0;

	/* Input command params */
	printf("Ensure all outstanding ITS ops associated with physical ints\n");
	printf("SYNC <RDbase>\n");
	printf("<RDbase>: ");
	if (scanf("%ld", &target) != 1) {
		printf("Invalied arguments\n");
		exit(EXIT_FAILURE);
	}

	target = target >> 16;

	/* Construct command */
	command[0]   = 0x5;
	command[1]   = 0x0;

	command[18] = (uint8_t)(0xFF & target);
	command[19] = (uint8_t)(0xFF & (target >> 8));
	command[20] = (uint8_t)(0xFF & (target >> 16));
	command[21] = (uint8_t)(0xFF & (target >> 24));

	return command;
}
