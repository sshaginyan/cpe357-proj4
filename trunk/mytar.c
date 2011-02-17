/**
 * @file
 * <pre> CPE 357 Winter 2011
 * -------------------
 *
 * Program "describe the program here (without quotes)"
 *
 * Last Modified: Wed Feb 16 13:39:06 PST 2011</pre>
 * @author Luis Castillo
 * Copyright (C) 2011 Luis Castillo. All rights reserved.
 */

#include <sys/types.h>
#include <unistd.h>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

#include <getopt.h>

/** The main thing.
 * @param argc the number of tokens on the input line.
 * @param argv an array of tokens.
 * @return 0 on success, 1-255 on failure
 */
int main (int argc, char *argv[])
{
  static int c_flag = 0;
  static int t_flag = 0;
  static int x_flag = 0;
  static int v_flag = 0;
  static int S_flag = 0;
  static int f_flag = 0;
  static int help_flag = 0;
  static int version_flag = 0;
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
      {"sparse",  no_argument, 0, 'S'},
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
      printf ("option %s", long_options[option_index].name);
      if (optarg)
        printf (" with arg %s", optarg);
      printf ("\n");
      break;
     
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
  
  /* Report the final status of the flags */
  if (c_flag)
    printf("c flag is set\n");
  if (t_flag)
    printf("t flag is set\n");
  if (x_flag)
    printf("x flag is set\n");
  if (v_flag)
    printf("v flag is set\n");
  if (S_flag)
    printf("S flag is set\n");
  if (f_flag)
    printf("f flag is set\n");
  if (help_flag)
    printf("help flag is set\n");
  if (version_flag)
    printf("version flag is set\n");

  /* Print any remaining command line arguments (not options). */
  if (optind < argc)
  {
    printf ("non-option ARGV-elements: ");
    while (optind < argc)
      printf ("%s ", argv[optind++]);
    putchar ('\n');
  }
  
  return EXIT_SUCCESS;
}

/* vim: set et ai sts=2 sw=2: */
