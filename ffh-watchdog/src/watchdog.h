#include <unistd.h>
#include <stdbool.h>
#include "libubus.h"
#include <stdio.h>

bool has_watchdog();

int tickle_watchdog();

bool magicclose_enabled(void);

void set_magicclose_flag(bool);

void set_run_procd_trigger_state(bool enable);
