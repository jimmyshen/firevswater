/**
 * Fire vs Water : Debug tracing / assertion macros
 *
 * Copyright (C) 2011 Chun Min Shen <kuato@erratical.com>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef FVW_DEBUG_H
#define FVW_DEBUG_H

#ifndef DEBUG_MODE

#define TRACE_FN()
#define TRACE(msg)
#define VTRACE(msg, ...)           
#define ASSERT(cond)

#else

#include <stdio.h>

#define TRACE_FN()                  fprintf(stderr, "Entered function %s()\n", __FUNCTION__);
#define TRACE(msg)                  fputs("... ", stderr); fputs(msg, stderr); fputc('\n', stderr);
#define VTRACE(msg, ...)            fputs("... ", stderr); fprintf(stderr, msg, __VA_ARGS__); fputc('\n', stderr);
#define ASSERT(cond) if ( !(cond) ) fprintf(stderr, "... assertion failed in %s at line %d\n", __FUNCTION__, __LINE__);

#endif  // DEBUG_MODE

#endif  // FVW_DEBUG_H

