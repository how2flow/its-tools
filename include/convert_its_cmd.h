/* SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (C) 2019 ARM Limited, All Rights Reserved.
 */

#ifndef _CONVERT_ITS_CMD_H
#define _CONVERT_ITS_CMD_H

extern uint8_t *(*convert_its_cmd[])(void);
uint8_t *convert_INT(void);
uint8_t *convert_INV(void);
uint8_t *convert_INVALL(void);
uint8_t *convert_MAPC(void);
uint8_t *convert_MAPD(void);
uint8_t *convert_MAPTI(void);
uint8_t *convert_SYNC(void);

#endif /* _CONVERT_ITS_CMD_H */
