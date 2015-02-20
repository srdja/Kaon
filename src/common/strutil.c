/*
 * strutil.c
 *
 *  Created on: Nov 4, 2014
 *      Author: srdja
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "strutil.h"

char *
strucat(int args, ...)
{
    char *argv[args];
    int   lens[args];

    va_list  params;
    size_t len = 0;

    va_start(params, args);

    int i;
    for (i = 0; i < args; i++) {
        argv[i] = va_arg(params, char*);
        lens[i] = strlen(argv[i]);
        len += lens[i];
    }
    va_end(params);

    char *string = malloc(len + 1);

    memcpy(string, argv[0], lens[0] + 1);
    for (i = 1; i < args; i++)
        strncat(string, argv[i], lens[i]);

    return string;
}

char *
strucpy(const char *str)
{
    size_t len = strlen(str);
    char *copy = malloc(len + 1);
    strncpy(copy, str, len + 1);

    return copy;
}
