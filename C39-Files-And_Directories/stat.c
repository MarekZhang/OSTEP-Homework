#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int main (int argc, char *argv[]) {
  if (argc != 2) {
    printf("usage: ./stat <path>\n");
    exit(EXIT_FAILURE);
  }
  
  struct stat file_stat;
  char *path = argv[1];

  if (stat(path, &file_stat) != 0) {
    perror("failed to access file status.\n");
    exit(EXIT_FAILURE);
  }

  printf("The file size is: %ld bytes\n", (long)file_stat.st_size);
  printf("The number of blocks allocated: %ld\n", (long)file_stat.st_blocks);
  printf("The number of hard links: %hu \n", file_stat.st_nlink);
  printf("The inode number is %llu \n", file_stat.st_ino);

}

