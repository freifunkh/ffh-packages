#include "watchdog.h"
#include <stdbool.h>
#include <unistd.h>
#include "libubus.h"
#include <json-c/json.h>
#include <stdio.h>

struct ubus_context *ctx;

enum {
	RETURN_MAGICCLOSE,
	__RETURN_MAX,
};

static const struct blobmsg_policy watchdog_policy[__RETURN_MAX] = {
	[RETURN_MAGICCLOSE] = { .name = "magicclose", .type = BLOBMSG_TYPE_BOOL, },
};

void cb_bool_extractor(struct ubus_request *req, int type, struct blob_attr *msg) {
    (void)type;
    bool *enabled = (bool *)req->priv;

    struct blob_attr *tb[__RETURN_MAX];

    if (blobmsg_parse(watchdog_policy, __RETURN_MAX, tb, blob_data(msg), blob_len(msg))) {
    	*enabled = false;
    	return;
    }

    if (!tb[RETURN_MAGICCLOSE]) {
    	*enabled = false;
    	return;
    }

    *enabled = blobmsg_get_bool(tb[RETURN_MAGICCLOSE]);
}

bool has_watchdog(void) {
    /* Determine whether /dev/watchdog is provided by a driver
     */
    return (access("/dev/watchdog", F_OK) == 0);
}

void set_magicclose_flag(bool enable) {
    /* Whether procd should send the magic byte "v" to the watchdog prior closing.
     * The byte tells the hardware watchdog not to restart the device, even if the
     * file descriptor of /dev/watchdog is closed and allows procd to close it.
     */
    static struct blob_buf buf;
    blobmsg_buf_init(&buf);
    blobmsg_add_u8(&buf, "magicclose", enable);

    ctx = ubus_connect(NULL);
    uint32_t ubus_id_system = 0;
    if (ubus_lookup_id( ctx, "system", &ubus_id_system )) {
        printf("Could not find object system\n");
	goto err;
    }
    printf("%u\n", ubus_id_system);

    bool rc = false;
    ubus_invoke(ctx, ubus_id_system, "watchdog", buf.head, cb_bool_extractor, &rc, 1000);

    blob_buf_free(&buf);

err:
    if (ctx) {
        ubus_free(ctx);
    }
}

void set_run_procd_trigger_state(bool enable) {
    static struct blob_buf buf;
    blobmsg_buf_init(&buf);
    blobmsg_add_u8(&buf, "stop", !enable);

    ctx = ubus_connect(NULL);
    uint32_t ubus_id_system = 0;
    if (ubus_lookup_id( ctx, "system", &ubus_id_system )) {
        printf("Could not find object system\n");
	goto err;
    }
    printf("%u\n", ubus_id_system);

    bool rc = false;
    ubus_invoke(ctx, ubus_id_system, "watchdog", buf.head, cb_bool_extractor, &rc, 1000); // TODO some different callback

    blob_buf_free(&buf);

err:
    if (ctx) {
        ubus_free(ctx);
    }
}

bool magicclose_enabled(void) {
    /* Emit the current state of ubus system watchdog magicclose
     * Which disarms the hardware watchdog and closes the fd of /dev/watchdog upon termination
     * of the current trigger.
     */
    ctx = ubus_connect(NULL);
    uint32_t ubus_id_system = 0;
    if (ubus_lookup_id( ctx, "system", &ubus_id_system )) {
        printf("Could not find object system\n");
	goto err;
    }
    printf("%u\n", ubus_id_system);

    bool rc = false;
    ubus_invoke(ctx, ubus_id_system, "watchdog", NULL, cb_bool_extractor, &rc, 1000);

err:
    if (ctx) {
        ubus_free(ctx);
    }

    return rc;
}

int tickle_watchdog() {
    FILE *fp = fopen("/dev/watchdog", "w");
    if (fp == NULL) {
        return -1;
    }
    fputc('1', fp);
    fclose(fp);
    return 0;
}
