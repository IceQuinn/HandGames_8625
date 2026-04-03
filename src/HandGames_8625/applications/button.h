/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2026-03-06     mutou       the first version
 */
#ifndef APPLICATIONS_BUTTON_H_
#define APPLICATIONS_BUTTON_H_

#include <stdint.h>

void button_therad(void);

uint32_t Btn_Event_Wait(int32_t timeout);

#endif /* APPLICATIONS_BUTTON_H_ */
