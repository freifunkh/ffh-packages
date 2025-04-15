#include <json-c/json.h>
#include <libgluonutil.h>
#include <respondd.h>
#include <string.h>
#include "ffh-authorized-fingerprints.h"

void add_fingerprint(const char *fingerprint, const char *comment, void *target_object) {
    (void) comment;
    json_object_array_add(target_object, json_object_new_string(fingerprint));
}

static struct json_object * get_authorized(void) {
	struct json_object *ret = json_object_new_array();
	emit_fingerprints(add_fingerprint, ret);

	return ret;	
}

static struct json_object * respondd_provider_nodeinfo(void) {
	struct json_object *ret = json_object_new_object();

	struct json_object *owner = json_object_new_object();
	json_object_object_add(owner, "authorized", get_authorized());
	json_object_object_add(ret, "owner", owner);

	return ret;
}

const struct respondd_provider_info respondd_providers[] = {
	{"nodeinfo", respondd_provider_nodeinfo},
	{ 0 }
};
