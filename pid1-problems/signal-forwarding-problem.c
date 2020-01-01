#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

void cleanup() {
    printf("clean up...\n");
}

void signal_handler(int signal) {
    switch(signal) {
        case SIGTERM:
            printf("Got SIGTERM\n");
            cleanup();
            exit(0);
	}
}

/*
 * do cleanup when receive SIGTERM signal, then exit.
 */
int main(int argc, char **argv) {
    int argv0size = strlen(argv[0]);

    // name main process
    memset(argv[0], 0, argv0size);
    snprintf(argv[0], argv0size, "%s", "main");

    if (signal(SIGTERM, signal_handler) == SIG_ERR)
        printf("can't trap SIGTERM\n");

    printf("main process will exit after 300 seconds.\n");
    for(int i = 0; i < 300; ++i)
        sleep(1);
    printf("main process exit.\n");
    return 0;
}

