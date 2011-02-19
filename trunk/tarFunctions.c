/**
 * @file
 * Set of functions used in myTar.c
 * <pre>tarFunctions.c
 * CPE 357
 * ----------------
 * Set of functions used in myTar.c
 *
 * Last modified: Fri Feb 25, 2011</pre>
 * @author Luis Castillo, Ricardo Tijero
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "tarFunctions.h"

void printHelp ()
{
  int fd;
  char buf[BUF_SIZE];
  int i;
  int numChars;

  fd = open("HELP", O_RDONLY);
  if(fd == -1)
  {
    perror("Error opening HELP file");
    return;
  }

  while((numChars = read(fd, buf, BUF_SIZE)) > 0)
  {
    for( i = 0; i < numChars; i++)
      printf("%c", buf[i]);
  }

  close(fd);
  return;
}

void printVersion ()
{
  int fd;
  char buf[BUF_SIZE];
  int i;
  int numChars;

  fd = open("VERSION", O_RDONLY);
  if(fd == -1)
  {
    perror("Error opening VERSION file");
    return;
  }

  while((numChars = read(fd, buf, BUF_SIZE)) > 0)
  {
    for( i = 0; i < numChars; i++)
      printf("%c", buf[i]);
  }

  close(fd);
  return;
}

