/*
 * console.c
 *
 *  Created on: Oct 8, 2014
 *      Author: srdja
 */

#include "console.h"

size_t msg_size = 256;
char msg_buff[256];

void console_init(size_t cbuff_s)
{

}

void console_destroy()
{

}

void console_msg_post(char *msg, ...)
{
    va_list args;
    va_start(args, msg);

    vsnprintf(msg_buff, msg_size, msg, args);

    va_end(args);

    printf("%s", msg_buff);
}
