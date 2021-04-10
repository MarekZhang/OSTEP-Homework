#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

#define MAXSTRLEN 256

void file_info(char *filename);

void file_info_verbose(char *filename);

void strmod(mode_t mode, char *buf);

char *path = ".";

int main(int argc, char *argv[]) {
  char c;
  int verbose = 0;
  DIR *dir;
  struct dirent * dp;


  while ((c = getopt(argc, argv, "l")) != -1) {
    if (c == 'l')
      verbose = 1;
  }

  if (verbose && argc == 3) {
    path = argv[2];
  } else if (!verbose && argc == 2) {
    path = argv[1];
  }

  dir = opendir(path);
  if (dir == NULL) {
    perror("invalid directory path or the director cannot be accessed.\n");
    exit(EXIT_FAILURE);
  }

  while ((dp = readdir(dir)) != NULL) {
    if (verbose) {
      file_info_verbose(dp->d_name);
    } else {
      file_info(dp->d_name);
    }
  }

  return 0;
}

void file_info (char *filename) {
  printf("%s\n", filename);
}

void file_info_verbose (char *filename) {
  struct stat file_stat;
  char str[MAXSTRLEN];
  strcpy(str, path);
  strcat(str, "/");
  strcat(str, filename);
  if (stat(str, &file_stat) == -1) {
    printf("unable to access file %s\n", filename);
    exit(EXIT_FAILURE);
  }

  char buf[10];
  strmode(file_stat.st_mode, buf);

  printf("%-20s mode: %s   inode: %8llu   size: %8lldB  links: %3d\n", filename, buf, file_stat.st_ino, file_stat.st_size, file_stat.st_nlink);
}

void strmod(mode_t mode, char *buf) {
  const char chars[] = "rwxrwxrwx";

  for (int i = 0; i < 9; i++) {
    buf[i] = (mode & (1 << (8 - i))) ? chars[i] : '-';
  }

  buf[9] = '\0';
}
