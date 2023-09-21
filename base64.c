#include "base64.h"
#include <string.h>

int base64_encode_str(const char *src, const int src_len, char *dst, int dst_size) {
    // 1.Check the array is enough to store and the length of src is within the limit
    if (src_len > SUHENG_BASE64_ENCODE_SRC_MAX_LENGTH)
        return -2;
    int dst_len = (src_len % 3 == 0) ? (src_len / 3) * 4 : (src_len / 3 + 1) * 4;
    if (dst_len > dst_size)
        return -1;//space is not enough
    memset(dst, 0, dst_size);
    // 2.encode 
    int dst_index = 0;
    int src_index = 0;
    char bit6 = 0x3F;   // 00111111  6bit
    char bit8 = 0xFF;  // 00000000
    while (src_index < src_len) {
        //take three Bytes from src and encode them into four Bytes BASE64 characters
        char c0 = src[src_index++];
        char c1 = (src_index < src_len) ? src[src_index++] : 0;
        char c2 = (src_index < src_len) ? src[src_index++] : 0;
        dst[dst_index++] = ENCODE_TABLE[(c0 >> 2) & bit6];
        dst[dst_index++] = ENCODE_TABLE[((c0 << 4) | ((c1 & bit8) >> 4)) & bit6];
        dst[dst_index++] = ENCODE_TABLE[((c1 << 2) | ((c2 & bit8) >> 6)) & bit6];
        dst[dst_index++] = ENCODE_TABLE[c2 & bit6];
    }
    // 3.padding
    switch (src_len % 3) {
        case 1:
            dst[--dst_index] = '=';
        case 2:
            dst[--dst_index] = '=';
    }
    return dst_len;
}


int base64_decode_str(const char *src, const int src_len, char *dst, int dst_size) {
    // 1.judge the string is valid
    if (src_len % 4 != 0)
        return -3;

    // 2. judge the src_len is within the limit
    if (src_len > SUHENG_BASE64_DECODE_SRC_MAX_LENGTH)
        return -2;
    // 3. calc dst len
    int dst_len = src_len / 4 * 3;
    if (src[src_len - 2] == '=') {
        dst_len -= 2;
    } else if (src[src_len - 1] == '=') {
        dst_len--;
    }
    if (dst_len > dst_size)
        return -1; //space is not enough

    memset(dst, 0, dst_size);

    // 4.decode
    char bit8 = 0xFF;
    int dst_index = 0;
    int src_index = 0;
    while (src_index < src_len) {
        char c0 = DECODE_TABLE[src[src_index++]];
        char c1 = DECODE_TABLE[src[src_index++]];
        char c2 = DECODE_TABLE[src[src_index++]];
        char c3 = DECODE_TABLE[src[src_index++]];
        dst[dst_index++] = (char) ((c0 << 2) | (c1 >> 4));
        dst[dst_index++] = (char) (((c1 << 4) | (c2 >> 2)) & bit8);
        dst[dst_index++] = (char) (((c2 << 6) | c3) & bit8);
    }
    return dst_len;
}