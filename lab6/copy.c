#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NAME 0
#define OPTIONS 1
#define INFILE 2
#define OUTFILE 3

#define BUFFERSIZE 1024

int help(char *name);
int error_msg(char *message, char *name, int error_code);
int copy_read_write (int fd_from, int fd_to);
int copy_mmap(int fd_from, int fd_to);
void check_arguments(int arguments, int how_many, char *name);

int main(int argc, char *argv[]) {
  //  Using getopt()
  int opt = getopt(argc, argv, "mh");

  if (opt == 'h') {
    return help(argv[NAME]);
  }

  // Checking 2 filenames
  if (opt != -1) {
    check_arguments(argc,4,argv[NAME]);
  } else {
    check_arguments(argc,3,argv[NAME]);
  }

  // Both file descriptors
  int fd_from, fd_to;
  fd_from = open(argv[optind], O_RDONLY);
  fd_to = open(argv[optind + 1], O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

  // if file doesn't exists
  if (fd_from == -1) {
    return error_msg("Error opening the file", argv[NAME], 6);
  }

  if (opt != -1) {
    switch (opt) {
      case 'm':
        // use of copy_mmap
        if (copy_mmap(fd_from,fd_to) == 0) {
            printf("File %s copied as %s successfully\n", argv[optind], argv[optind + 1]);
        } else {
          return error_msg("Error copying the files", argv[NAME], 5);
        }
          break;
      case '?':
        return error_msg("Bad option", argv[NAME], 1);
    }
  } else {
    if (copy_read_write(fd_from,fd_to) == 0) {
      printf("File %s copied as %s successfully\n", argv[optind], argv[optind + 1]);
    } else {
      return error_msg("Error copying the files", argv[NAME], 5);
    }
  }

  // Closing files
  close(fd_from);
  close(fd_to);

  return 0;
}

int help(char *name) {
  printf("usage:\n"
  "   Copy files using read() or write() (no using -m)\n"
  "   Copy files using mmap() and memcpy() (-m to use them)\n\n\t"

  " %s  [-m] <file_name> <new_file_name>\n\t"
  " %s   <file_name> <new_file_name>\n\t"
  " %s  [-h|-help] \n\t"
  "\n"
  "\n"
  " %s Correct syntax examples:\n"
  "   %s -h\n"
  "   %s -m filename_input.txt filename_output.txt\n"
  "   %s filename_input.txt filename_output.txt\n"
  , name, name, name, name, name, name, name, name,name);
  return 0;
}

int error_msg(char *message, char *name, int error_code) {
  if (name != NULL) {
    printf("%s, see help: %s -h\n", message, name);
  } else {
    printf("%s\n", message);
  }
  return error_code;
}

int copy_read_write (int fd_from, int fd_to) {
  char buffer[BUFFERSIZE];
  int numbytes;
  int error_wrinting;

  while ((numbytes = read(fd_from, &buffer, sizeof(char))) > 0){
    error_wrinting = write(fd_to, &buffer, numbytes);
    if (numbytes == -1 || error_wrinting == -1) {
      return error_msg("Error copying file",NULL,7);
    }
  }

  return 0;
}

int copy_mmap(int fd_from, int fd_to) {
  void *src, *dest;
  struct stat s;
  int filesize;

  int status = fstat (fd_from, &s);
  filesize = s.st_size;

  if ((src = mmap(NULL, filesize, PROT_READ, MAP_SHARED, fd_from, 0)) == (void *) -1) {
    return error_msg("Error mapping in file",NULL,4);
  }
  ftruncate(fd_to, filesize);

  if ((dest = mmap(NULL, filesize, PROT_READ | PROT_WRITE, MAP_SHARED, fd_to, 0)) == (void *) -1) {
    return error_msg("Error mapping destination file",NULL,4);
  }

  memcpy(dest, src, filesize);
  munmap(src, filesize);
  munmap(dest, filesize);

  return 0;
}

void check_arguments(int arguments, int how_many, char *name) {
  if (arguments > how_many) {
    // Returns error code 3 which means too many arguments
    exit (error_msg("Too many arguments", name, 3));
  } else if (arguments < how_many) {

    //  Returns error code 4 which means not enough arguments
    exit (error_msg("Not enough arguments", name, 4));
  }
}
