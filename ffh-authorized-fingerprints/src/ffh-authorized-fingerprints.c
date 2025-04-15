#include <libubox/utils.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ecdsautil/sha256.h>
#include <unistd.h>
#include <errno.h>
#include "ffh-authorized-fingerprints.h"

void null_equal_signs(char *buffer) {
    /* Replace all equal-signs with a terminator
     *
     * Args
     *     buffer: The pointer of the buffer to modify
     */
    while (*buffer) {
        if (*buffer == '=') {
            *buffer = '\0';
        }
        buffer++;
    }
}

void encode_sha256(uint8_t *buf, size_t len, char* outbuf) {
    /* Generate the sha256sum of a known length buffer and emit it in ssh fingerprint form.
     *
     * SSH fingerprints are the base64 encoding of the sum, but without the padding equal-sign.
     *
     * Args
     *     buf: The start of the bytes to hash
     *     len: The amount of bytes to hash
     *     outbuf: The storage for the fixed length fingerprint
     */
    uint8_t hash[4096];
    ecdsa_sha256_context_t ctx;

    ecdsa_sha256_init(&ctx);
    ecdsa_sha256_update(&ctx, buf, len);
    ecdsa_sha256_final(&ctx, hash);

    int b64_buflen = B64_ENCODE_LEN(32);
    char str[b64_buflen];

    b64_encode(hash, 32, str, b64_buflen);
    null_equal_signs(str);
    snprintf(outbuf, 45, "%s",  str);
}

void print_fingerprint(const char *fingerprint, const char *comment, void *userdata) {
    /* A simple fingerprint printer to be used as callback.
     *
     * Args
     *     fingerprint: The fingerprint consisting of only base64 characters.
     *     comment: The comment from the key file (words 3ff of each line).
     *     userdata: In this callback unused userdata which can be used to modify datastructures via callback.
     */
    (void) userdata;
    printf("SHA256:%s %s", fingerprint, comment);
}

void emit_fingerprints(fingerprint_comment_callback cb, void *userdata) {
    /* Emit the fingerprints of all authorized keys into the given callback function.
     *
     * Args
     *     cb: The callback function which will be provided with the fp, the comment and userdata
     *     userdata: A passthrough into the callback, which allows e.g. data data modification.
     */
    FILE *fp = fopen("/etc/dropbear/authorized_keys", "r");
    if (!fp) {
        perror("Unable to open authorized_keys");
        exit(1);
    }

    char line[MAX_LINE_LENGTH];
    char buf[45];
    while (fgets(line, sizeof(line), fp)) {
        if (line[0] == '#' || strlen(line) < 32) continue;

        char *key_start = strchr(line, ' ');
        if (!key_start) continue;
        key_start++;

        char *key_end = strchr(key_start, ' ');
        if (key_end) *key_end = '\0';

        uint8_t decoded[4096];
        size_t decoded_len_calc = B64_DECODE_LEN(key_end-key_start);

        int decoded_bytes = b64_decode(key_start, decoded, decoded_len_calc);
        if (decoded_bytes == -1) {
            fprintf(stderr, "Failed to decode base64\n");
            fclose(fp);
            exit(1);
        }

        encode_sha256(decoded, decoded_bytes, buf);

        cb(buf, key_end+1, userdata);
    }

    fclose(fp);
}

int main() {
    /* Print all fingerprints of authorized keys on this device, as well as their comments.
     */
    emit_fingerprints(print_fingerprint, NULL);
    exit(0);
}

