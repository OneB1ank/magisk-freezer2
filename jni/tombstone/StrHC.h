#include "HeadFile.h"
#ifndef STR_HC_H
#define STR_HC_H
namespace strhc
{
    char *strcpy(char *dst, const char *src)
    {
        const size_t length = strlen(src);
        memcpy(dst, src, length + 1);
        return dst;
    }

    char *strcat(char *dst, const char *src) {
    const size_t dstlen = strlen(dst);
    const size_t srclen = strlen(src);
    memcpy(dst+dstlen, src, srclen+1);
    return dst;
}

} // namespace strhc

#endif // STR_HC_H