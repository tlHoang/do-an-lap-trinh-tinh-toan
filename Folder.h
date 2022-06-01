#include <dirent.h>
#include <errno.h>

void check_folder() {
	DIR* dir = opendir("receipt");
	if (dir) {
	    closedir(dir);
	} else if (ENOENT == errno) {
	    printf("Missing \"receipt\" folder. Please create one.");
	    exit(1);
	} else {
	    printf("Error: opendir() failed...");
	    exit(1);
	}

	dir = opendir("report");
	if (dir) {
	    closedir(dir);
	} else if (ENOENT == errno) {
	    printf("Missing \"report\" folder. Please create one.");
	    exit(1);
	} else {
	    printf("Error: opendir() failed...");
	    exit(1);
	}
}
