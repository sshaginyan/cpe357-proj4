#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

/* Flags set by --help and --version */
static int help_flag, version_flag;

int main (int argc, char *argv[])
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
      printf("option -c\n");
      break;

    case 't':
      printf("option -t\n");
      break;
   
    case 'x':
      printf("option -x\n");
      break;

    case 'v':
      printf("option -v\n");
      break;
      
    case 'S':
      printf("option -S\n");
      break;
    
    case 'f':
      printf("option -f\n");
      break;
    
    case '?':
      /* getopt_long already printed an error message. */
      break;
     
    default:
      abort ();
    }
  }
  
  /* Instead of reporting --help and --version as
   * they are encountered, we report the final status
   * resulting from them. */
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

