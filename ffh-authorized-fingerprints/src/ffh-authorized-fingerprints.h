#include <libubox/utils.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ecdsautil/sha256.h>
#include <unistd.h>
#include <errno.h>

#define MAX_LINE_LENGTH 4096

typedef void (*fingerprint_comment_callback)(const char *fingerprint, const char *rest, void* userdata);

void null_equal_signs(char *buffer);

void encode_sha256(uint8_t *buf, size_t len, char* outbuf);

void print_fingerprint(const char *fingerprint, const char *comment, void *userdata);

void emit_fingerprints(fingerprint_comment_callback cb, void *userdata);
