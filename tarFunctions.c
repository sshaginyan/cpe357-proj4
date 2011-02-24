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

/***************************************************************************************/
/* DEFINITIONS:
 *
 *      BUF_SIZE     100
 *      BLOCK_SIZE   512
 *      NAME_SIZE    100
 *      MODE_SIZE      8
 *      UID_SIZE       8
 *      GID_SIZE       8
 *      FILE_SIZE     12
 *      TIME_SIZE     12
 *      CHKSM_SIZE     8
 *      LNKNAME_SIZE 100
 *      USTAR_SIZE     6
 *      VER_SIZE       2
 *      UNAME_SIZE    32
 *      GNAME_SIZE    32
 *      DEVNUM_SIZE    8
 *      PREFIX_SIZE  155
 *
 *
 * STRUCTURES USED:
 *
 *   struct header{                    <SIZE>
 *       char fileName[NAME_SIZE];      (100)
 *       char mode[MODE_SIZE];          (  8)
 *       char userID[UID_SIZE];         (  8)
 *       char groupID[GID_SIZE];        (  8)
 *       char fileSize[FILE_SIZE];      ( 12)
 *       char time[TIME_SIZE];          ( 12)
 *       char checksum[CHCKSM_SIZE];    (  8)
 *       char fileType;                 (  1)
 *       char linkName[LNKNAME_SIZE];   (100)
 *       char ustar[USTAR_SIZE];        (  6)
 *       char version[VER_SIZE];        (  2)
 *       char userName[UNAME_SIZE];     ( 32)
 *       char groupName[GNAME_SIZE];    ( 32)
 *       char devMajNum[DEVNUM_SIZE];   (  8)
 *       char devMinNum[DEVNUM_SIZE];   (  8)
 *       char prefix[PREFIX_SIZE];      (155)
 *   };  
 *                                                                                     */
/***************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "tar.h"
#include "tarFunctions.h"

/***************************************************************************************/
/* Description: Creates a new treeDir structure and returns a pointer to it.           */
/***************************************************************************************/
treeDir *newDir (header *aHeader)
{
    treeDir *aDir;

    aDir = malloc (sizeof (struct treeDir));
    if (aDir == NULL)
    {
        perror("malloc");
        return NULL;
    }
 
    aDir->fileInfo = aHeader;
    aDir->next = NULL;
    aDir->parent = NULL;
    aDir->child = NULL;

    if (aHeader->fileType == DIRTYPE)
    	aDir->isDir = 1;
    else
        aDir->isDir = 0;

    return aDir;
}

/***************************************************************************************/
/* UNDER CONSTRUCTION */
/* builds a tree */
/***************************************************************************************/
treeDir *makeTree (header *headers[])
{
    int i = 0;
    treeDir *root = NULL;
    treeDir *parent = NULL;
    treeDir *child = NULL;

    root = parent = newDir( headers[i++] );
    while( headers[i] != NULL)
    {
        child = newDir( headers[i] );
        if (parent->isDir)
            child->parent = parent;
        if (child->isDir)
            parent = child;

        i++;
    }

    return root;
}
/* UNDER CONSTRUCTION: note: TOKENIZE PATH*/
treeDir *search (treeDir *parent, char *path)
{
    char parentPath[strlen (path)];
    
    strcpy (parentPath, path);
    getParentPath (parentPath);

    if (parent != NULL)
    {
        while
    }
}
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> NO PURPOSE YET <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*
void traverce (treeDir *parent)
{
    if (parent != NULL)
    {
        while(parent != NULL)
        {
            if (Parent->isDir)
               traverce (parent->child);
            
            parent = parent->next;
        }
    }
    return;
}*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

/***************************************************************************************/
/* Brif: Creates new header structure and returns a pointer to it.                     */
/* Details: Given an array of 512 characters (one Block), it creates a header structure*/
/*    and initializes every field in the structure. Returns a pointer to structure.    */
/***************************************************************************************/
header *newHeader(char *buf)
{
    int i;
    header *tarHeader;
    header *pos;
    
    tarHeader = malloc (sizeof (struct header));
    if (tarHeader == NULL)
    {
        perror("malloc");
        return NULL;
    }

    pos = tarHeader;

    for (i = 0; i < BLOCK_SIZE; i++)
    {
        *pos = buf[i];
        pos++;
    }
    return tarHeader;
}

/***************************************************************************************/
/* Description: gets a header from a tar file and places pointer to next the next      */
/*     header in the file.                                                             */
/***************************************************************************************/
header *nextHeader (int fd)
{
    int n, fullFile;
    int count = 0;
    char buf[BLOCK_SIZE];
    char aHeader[BLOCK_SIZE];
    
    if ((n = read(fd, aHeader, BLOCK_SIZE)) <= 0)
        return NULL;

    while (!fullFile && (n = read(fd, buf, BLOCK_SIZE)) > 0)
    {
        if (isNullBlock(buf))
            count++;
        else
            count = 0;

        if (count == 2)
            fullFile = 1;
    }

    return newHeader (aHeader);
}

/***************************************************************************************/
/* Description: given an array, determines if it is full of NULLs.                     */
/***************************************************************************************/
int isNullBlock (char *block)
{
    int i;
    int count = 0;

    for (i = 0; i < BLOCK_SIZE; i++)
    {
        if (block[i] == '\0')
            count++;
    }
    
    if (count == BLOCK_SIZE)
        return 1;
    else
        return 0;
}

/***************************************************************************************/
/* Description: changes a number represented in ASCII characters to intigers.          */
/***************************************************************************************/
int charToInt (char *arr, int leng)
{
    int num = 0;
    int i;

    for (i = 0; i < leng; i++)
    {
        num = (num * 10) + (arr[i] - '0');
    }

    return num;
}
/*>>>>>>>>>>>>>>>>>>>>>>  NEEDS REVISION <<<<<<<<<<<<<<<<<<<<<<<<<<*/
/***************************************************************************************/
/* Description: Displays the names of the files in an array. (myTar option -t)         */
/***************************************************************************************/
void printFiles (header *files[], int leng)
{
    int i;

    for (i = 0; i < leng; i++)
        printf("%s\n", files[i]->fileName); /* fileName NULL terminated? */

    return;
}

/***************************************************************************************/
/* Description: prints the help file to the screen.                                    */
/***************************************************************************************/
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

/***************************************************************************************/
/* Description: prints the version file to the screen.                                 */
/***************************************************************************************/
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

