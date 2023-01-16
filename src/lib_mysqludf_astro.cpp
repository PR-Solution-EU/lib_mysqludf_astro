/*
    lib_mysqludf_astro - a library with astro functions
    Copyright (C) 2023  Norbert Richter <nr@prsolution.eu>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include <string.h>
#include <stdlib.h>
#include <cstdio>
#include <time.h>
#include <ctype.h>
#include <mysql.h>
#include <json-parser/json.h>
#include "lib_mysqludf_astro.h"

#ifdef DEBUG
#include <syslog.h>
#endif  // DEBUG


volatile int last_rc = 0;
char last_func[128] = {0};

/* Helper */
char *strcrpl(char *str, char find, char replace)
{
    for (char *p = str; *p; p++) {
        if (find == *p) {
            *p = replace;
        }
    }
    return str;
}

void parmerror(const char *context, UDF_ARGS *args)
{
    char *type;

#ifdef DEBUG
    setlogmask (LOG_UPTO (LOG_NOTICE));
    openlog (LIBNAME, LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
    syslog (LOG_NOTICE, "parmerror()");
#endif
    fprintf(stderr, "%s parameter error:\n", context);
    for (unsigned i=0; i<args->arg_count; i++) {
        switch (args->arg_type[i]) {
            case STRING_RESULT:
                type = (char*)"STRING";
                break;
            case INT_RESULT:
                type = (char*)"INT";
                break;
            case REAL_RESULT:
                type = (char*)"RWAL";
                break;
            case DECIMAL_RESULT:
                type = (char*)"DECIMAL";
                break;
            case ROW_RESULT:
                type = (char*)"ROW";
                break;
            default:
                type = (char*)"?unknown?";
                break;
        }
        fprintf(stderr, "  arg%2d: $%p, type %s (%d), len %ld, maybe_null %c, content '%s'\n",
                i,
                args->args[i],
                type,
                args->arg_type[i],
                args->lengths[i],
                args->maybe_null[i],
                (args->arg_type[i]==STRING_RESULT && args->args[i]!=NULL) ? (char *)args->args[i] : ""
                );
    }
#ifdef DEBUG
    closelog ();
#endif
}



/* Library functions */

/**
 * astro_info
 *
 * Returns udf library info as JSON string
 * astro_info()
 *
 */
bool astro_info_init(UDF_INIT *initid, UDF_ARGS *args, char *message)
{
    if (args->arg_count != 0) {
        parmerror("astro_info()", args);
        strcpy(message, "No arguments allowed (udf: astro_info)");
        return 1;
    }
    initid->ptr = (char *)malloc(MAX_RET_STRLEN+1);
    if (initid->ptr == NULL) {
        strcpy(message, "memory allocation error");
        return 1;
    }
    initid->max_length = MAX_RET_STRLEN;

    return 0;
}

void astro_info_deinit(UDF_INIT *initid)
{
    if (initid->ptr != NULL) {
        free(initid->ptr);
    }
}

char* astro_info(UDF_INIT *initid, UDF_ARGS *args, char* result, unsigned long* length, char *is_null, char *error)
{
    char *res = (char *)initid->ptr;

#ifdef DEBUG
    setlogmask (LOG_UPTO (LOG_NOTICE));
    openlog (LIBNAME, LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
    syslog (LOG_NOTICE, "astro_info()");
#endif

    *is_null = 0;
    *error = 0;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-truncation"
    snprintf(res, MAX_RET_STRLEN, "{\"Name\": \"%s\", \"Version\": \"%s\", \"Build\": \"%s\"}", LIBNAME, LIBVERSION, LIBBUILD);
#pragma GCC diagnostic pop

    *length = strlen(res);
#ifdef DEBUG
    syslog (LOG_NOTICE, "astro_info(): %s", res);
    closelog ();
#endif
    return res;
}
