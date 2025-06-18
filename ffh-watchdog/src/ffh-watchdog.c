#include "watchdog.h"
#include <signal.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>

bool does_stratum_flag_exist() {
    struct stat foo;
    return !(stat("/var/gluon/state/has_ntp_sync", &foo) < 0);
}

bool has_stratum_been_lost_for_less_than_n_seconds(int seconds) {
    time_t current_time;
    double seconds_since_creation;
    struct stat file_info;
    if (stat("/var/gluon/state/has_lost_ntp_sync", &file_info) < 0) {
        printf("Warning: ntp_sync_lost flag is missing, ntpd hooks might not be running.");
	return true;
    }
    current_time = time(NULL);
    seconds_since_creation = difftime(current_time, file_info.st_ctime);
    printf("file has been created %f seconds ago. Threshold is %d seconds.\n", seconds_since_creation, seconds);
    return seconds_since_creation < seconds;
}

bool is_uptime_less_than_n_seconds(uint seconds) {
    struct sysinfo info;
    int error = sysinfo(&info);
    if(error != 0)
    {
        printf("code error = %d\n", error);
    }
    return info.uptime<seconds;
}



void signalHandler(int sig) {
    (void) sig;
    printf("disabling magicclose\n");
    set_magicclose_flag(false);

    bool enabled = magicclose_enabled();
    printf("currently enabled: %d\n", enabled);


    printf("re-enabling procd watchdog trigger\n");
    set_run_procd_trigger_state(true);
    printf("done");

    exit(0);
}

int main() {
    // register handler
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    if (!has_watchdog()) {
        printf("Warning: /dev/watchdog is not present. Aborting.");
	exit(1);
    }

    bool enabled = magicclose_enabled();
    printf("currently enabled: %d\n", enabled);

    printf("enabling magicclose\n");
    set_magicclose_flag(true);

    enabled = magicclose_enabled();
    printf("currently enabled: %d\n", enabled);

    printf("re-enabling procd watchdog trigger\n");
    set_run_procd_trigger_state(false);
    printf("done");

    while (true) {
        tickle_watchdog();
        sleep(5);
        printf("doing all the stuff\n");

	int grace_period_seconds = 1800; // 30min
	if (is_uptime_less_than_n_seconds(grace_period_seconds)) {
            printf("Uptime is less than %d seconds.\n", grace_period_seconds );
	    continue;
	}
	if (does_stratum_flag_exist()) {
            printf("Stratum exists\n");
            continue;
	}
	// 45min
	if (has_stratum_been_lost_for_less_than_n_seconds(2700)) {
            printf("Stratum not lacking long enough.\n");
            continue;
	}
	printf("All conditions failed. Cease watchdog tickling.\n");
	break;
    }

    return 0;
}
