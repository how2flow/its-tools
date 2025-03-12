/* SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (C) 2025 Steve Jeong <steve@how2flow.net>
 */

#ifndef _ITS_TOOLS_H
#define _ITS_TOOLS_H

/* its tools mode id */
#define TRANSLATE 1

/* its command id */
#define INT 1
#define INV 2
#define INVALL 3
#define MAPC 4
#define MAPD 5
#define MAPTI 6
#define SYNC 7

#define BLOCK_SIZE (4 * 1024)

/* its-tools access reg */
uint32_t get_its_reg(uint64_t addr);
void set_its_reg(uint64_t addr, uint64_t val);

/* its-tools process init */
uint8_t select_its_tools_cmd_type(void);
uint8_t select_its_tools_mode(void);

/* its-tools using its cmd */
void translate_its_cmd(uint8_t *cmd);

#endif /* _ITS_TOOLS_H */
