/*
 * strutil.h
 *
 *  Created on: Nov 4, 2014
 *      Author: srdja
 */

#ifndef STRUTIL_H_
#define STRUTIL_H_

#include <stdarg.h>
#include <stdint.h>

char *uinttostring (uint32_t i);
char *strucat      (int args, ...);
char *strucpy      (const char *srt);

#endif /* STRUTIL_H_ */
