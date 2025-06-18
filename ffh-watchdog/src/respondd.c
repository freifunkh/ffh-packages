#include <json-c/json.h>
#include <libgluonutil.h>
#include <respondd.h>
#include "watchdog.h"

static struct json_object * respondd_provider_nodeinfo(void) {
	struct json_object *ret = json_object_new_object();
	struct json_object *hardware = json_object_new_object();

	if (has_watchdog()) {
		struct json_object *watchdog = json_object_new_object();
		json_object_object_add(hardware, "watchdog", watchdog);
	}
	json_object_object_add(ret, "hardware", hardware);

	return ret;
}

const struct respondd_provider_info respondd_providers[] = {
	{"nodeinfo", respondd_provider_nodeinfo},
	{ 0 }
};
