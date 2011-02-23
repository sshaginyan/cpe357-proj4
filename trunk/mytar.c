/**
 * @file
 * <pre> CPE 357 Winter 2011
 * -------------------
 *
 * Program "describe the program here (without quotes)"
 *
 * Last Modified: Wed Feb 16 13:39:06 PST 2011</pre>
 * @author Luis Castillo, Ricardo Tijero
 * Copyright (C) 2011 Luis Castillo, Ricardo Tijero. All rights reserved.
 */

#include <sys/types.h>
#include <unistd.h>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

#include <getopt.h> 
#include "tarFunctions.h"

static int c_flag = 0;
static int t_flag = 0;
static int x_flag = 0;
static int v_flag = 0;
static int S_flag = 0;
static int f_flag = 0;
static int help_flag = 0;
static int version_flag = 0;

void parser(char *);
void parser2(int, char **);

/** The main thing.
 * @param argc the number of tokens on the input line.
 * @param argv an array of tokens.
 * @return 0 on success, 1-255 on failure
 */
int main (int argc, char *argv[])
{
  if (argc < 2)
  {
    printf("Usage: mytar [ctxvS]f tarfile [ path [ ... ] ]\n");
    exit(EXIT_FAILURE);
  }

  if (argv[1][0] == '-')
    parser2(argc, argv);
  else
    parser(argv[1]);

  /* Report the final status of the flags */
/*  if (t_flag && !v_flag)
    printf("call function to list without verbose\n");
  if (t_flag && v_flag)
    printf("call function to list with verbose\n");
*/
  if (f_flag == 0 && help_flag == 0 && version_flag == 0)
  {
    printf("-f option is required\n");
    exit(EXIT_FAILURE);
  }
  else if (help_flag == 1)
    printHelp();
  else if (version_flag == 1)
    printVersion();

  if ((c_flag == 0) && (t_flag == 0) && (x_flag == 0) &&
      (help_flag == 0) && (version_flag == 0))
  {
    printf("At least one operation -c -t or -x is required\n");
    exit(EXIT_FAILURE);
  }

  return EXIT_SUCCESS;
}

/** Function parses command line options
 * that do not begin with 'n'
 * @param arg pointer to command line argument
 */
void parser(char *arg)
{
  char c;

  while(*arg)
  {
    c = *arg;

    switch(c)
    {
      case 'c':
        c_flag = 1;
        break;

      case 't':
        t_flag = 1;
        break;

      case 'x':
        x_flag = 1;
        break;

      case 'v':
        v_flag = 1;
        break;
      case 'S':
        S_flag = 1;
        break;
      case 'f':
        f_flag = 1;
        break;

      case '-':
        break;

      case '?':
        /* getopt_long already printed an error message. */
        break;

      default:
        abort ();
    }
    arg++;
  }
}

void parser2(int argc, char *argv[])
{
  int c;

  while (1)
  {
    static struct option long_options[] =
    {
      /* These options set a flag. */
      {"help",    no_argument, &help_flag,    1},
      {"version", no_argument, &version_flag, 1},
      /* These options don't set a flag.
       * We distinguish them by their indices. */
      {"create",  no_argument, 0, 'c'},
      {"list",    no_argument, 0, 't'},
      {"extract", no_argument, 0, 'x'},
      {"verbose", no_argument, 0, 'v'},
      {"strict",  no_argument, 0, 'S'},
      {"file",    no_argument, 0, 'f'},
      {0, 0, 0, 0}
    };

    /* getopt_long stores the option index here. */
    int option_index = 0;

    c = getopt_long (argc, argv, "ctxvSf",
        long_options, &option_index);

    /* Detect the end of the options. */
    if (c == -1)
      break;

    switch (c)
    {
      case 0:
        /* If this option set a flag, do nothing else now. */
        if (long_options[option_index].flag != 0)
          break;
        /*      printf ("option %s", long_options[option_index].name);
                if (optarg)
                printf (" with arg %s", optarg);
                printf ("\n");
                break;*/

      case 'c':
        c_flag = 1;
        break;

      case 't':
        t_flag = 1;
        break;

      case 'x':
        x_flag = 1;
        break;

      case 'v':
        v_flag = 1;
        break;

      case 'S':
        S_flag = 1;
        break;

      case 'f':
        f_flag = 1;
        break;

      case '?':
        /* getopt_long already printed an error message. */
        break;

      default:
        abort ();
    }
  }
}

/* vim: set et ai sts=2 sw=2: */
