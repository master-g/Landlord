/*
 *  common.h
 *  Landlord
 *
 *  Created by Master.G on 14-1-20.
 *  Copyright (c) 2014 MED. All rights reserved.
 */

#ifndef LANDLORD_COMMON_H_
#define LANDLORD_COMMON_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "memtracker.h"

#define PRINT_GAME_LOG_

#ifdef PRINT_GAME_LOG
#define DBGLog printf
#else
#define DBGLog  silent_printf
#endif

void silent_printf(const char *fmt, ...);

#endif /* LANDLORD_COMMON_H_ */
