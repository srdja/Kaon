/*
 * console.h
 *
 *  Created on: Oct 8, 2014
 *      Author: srdja
 */

#ifndef CONSOLE_H_
#define CONSOLE_H_

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void console_init(size_t cbuff_size);
void console_destroy();

void console_msg_post(char *msg, ...);

#endif /* CONSOLE_H_ */
