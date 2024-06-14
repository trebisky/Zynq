/* utils.c
 *
 * A handful of things that used to be supplied by libc.a
 *
 * Tom Trebisky 6-13-2024
 * from kyulib.c
 */

#include <stddef.h>
#include <string.h>

/* Used by printf -- could be a macro there.
 * The original printf relied on ctype.h
 */
int
isdigit ( int ch )
{
	if ( ch < '0' )
	    return 0;
	if ( ch > '9' )
	    return 0;
	return 1;
}

void *
memcpy ( void *s1, const void *s2, size_t count )
{
        char *p, *end;
	char *ps2;

        p = (char *) s1;
	ps2 = (char *) s2;
        end = &p[count];

        while ( p<end )
            *p++ = *ps2++;

        return s1;
}

void *
memset ( void *buf, int val, size_t count )
{
        char *p, *end;

        p=(char *)buf;
        end = &p[count];

        while ( p<end )
            *p++ = val;

        return buf;
}

int
memcmp ( const void *s1, const void *s2, size_t count )
{
        char *p, *q, *end;

        p=(char *)s1;
        q=(char *)s2;
        end = &p[count];

        while ( p < end ) {
            if ( *p++ < *q++ )
                return -1;
            if ( *p++ > *q++ )
                return 1;
        }

        return 0;
}

size_t
strlen ( const char *s )
{
        int len = 0;

        while ( *s++ )
            len++;

        return len;
}

/* From old linux sources, string.c */
char *
strncat(char *dest, const char *src, size_t count)
{
        char *tmp = dest;

        if (count) {
                while (*dest)
                        dest++;
                while ((*dest++ = *src++) != 0) {
                        if (--count == 0) {
                                *dest = '\0';
                                break;
                        }
                }
        }
        return tmp;
}

size_t
strnlen(const char *s, size_t count)
{
        const char *sc;

        for (sc = s; count-- && *sc != '\0'; ++sc)
                /* nothing */;
        return sc - s;
}

char *
strchr(const char *s, int c)
{
        for (; *s != (char)c; ++s)
                if (*s == '\0')
                        return NULL;
        return (char *)s;
}

/*
 * Unlike memcpy(), memmove() copes with overlapping areas.
 */
void *
memmove(void *dest, const void *src, size_t count)
{
        char *tmp;
        const char *s;

        if (dest <= src) {
                tmp = dest;
                s = src;
                while (count--)
                        *tmp++ = *s++;
        } else {
                tmp = dest;
                tmp += count;
                s = src;
                s += count;
                while (count--)
                        *--tmp = *--s;
        }
        return dest;
}


/* THE END */
