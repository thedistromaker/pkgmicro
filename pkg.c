#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define TEMP_SCRIPT "/tmp/pkgscript.sh"

void die(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

void runcmd(const char *cmd) {
    int ret = system(cmd);
    if (ret != 0) {
        fprintf(stderr, "E: Command failed: %s\n", cmd);
        exit(EXIT_FAILURE);
    }
}

void install_package(const char *pkgname) {
    char script_path[512];
    snprintf(script_path, sizeof(script_path), "%s/%s.pscr", SCRIPT_DIR, pkgname);

    if (access(script_path, F_OK) != 0) {
        fprintf(stderr, "E: Package script not found: %s\n", script_path);
        exit(EXIT_FAILURE);
    }
    FILE *temp = fopen(TEMP_SCRIPT, "w");
    if (!temp) die("fopen");

    fprintf(temp,
        "#!/bin/sh\n"
        "set -e\n"
        ". %s\n"
        "echo \"Fetching: $SRC_URI\"\n"
        "wget \"$SRC_URI\"\n"
        "build\n"
        "install\n",
        script_path
    );
    fclose(temp);
    chmod(TEMP_SCRIPT, 0755);

    runcmd(TEMP_SCRIPT);
    unlink(TEMP_SCRIPT);
}

int main(int argc, char *argv[]) {
    if (argc != 3 || strcmp(argv[1], "install") != 0) {
        fprintf(stderr, "E: Usage: %s install <pkgname>\n", argv[0]);
        return EXIT_FAILURE;
    }

    install_package(argv[2]);
    return EXIT_SUCCESS;
}
