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
#include <string.h>
#include <math.h>
#include "tar.h"
#include "tarFunctions.h"

/***************************************************************************************/
/* Description: Given a pointer to a header, creates a new treeDir structure and       */
/*      returns a pointer to it.                                                       */
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
    aDir->level = getLevel((aDir->fileInfo)->fileName);

    if (aHeader->fileType == DIRTYPE)
        aDir->isDir = 1;
    else
        aDir->isDir = 0;

    return aDir;
}

/***************************************************************************************/
/* Description: Given an array of header pointers, it builds a directory tree and      */
/*       returns a pointer to the root of the tree.                                    */
/***************************************************************************************/
treeDir *makeTree (header *headers[])
{
    int i = 0;
    treeDir *root = NULL;
    treeDir *parent = NULL;
    treeDir *child = NULL;
    treeDir *current = NULL;

    root = parent = newDir (headers[i++]);

    for (; headers[i] != NULL; i++)         /* Links all treeDir structures together. */
    {
        child = newDir (headers[i]);
        parent->next = child;
        parent = child;
    }

    parent = current = root;

    while (current != NULL)
    {
        child = current->next;
        if (parent->isDir)
            parent->child = child;
        while (child->level > parent->level)
        {
            if (child->level == (parent->level + 1))
                child->parent = parent;

            child = child->next;
        }
        parent->next = child;

        parent = current = current->next;
    }

    return root;
}

/***************************************************************************************/
/* Description: Given a treeDir tree and the name of a path in the tree, finds the     */
/*       the directory of the file that the path points to and returns a pointer to it.*/
/***************************************************************************************/
treeDir *getParent (treeDir *parent, char *path)
{
    int i = strlen (path);
    char parentPath[BUF_SIZE];

    strcpy (parentPath, path);
    for (i -= 1; i > 0; i--)
    {
        if(parentPath[i] == '/')
        {
            parentPath[i] = '\0';
            break;
        }
    }

    return find (parent, parentPath);
}

/***************************************************************************************/
/* Description: Given a path, gest the level down the tree where the path leads to.    */
/***************************************************************************************/
int getLevel (char *path)
{
    int i = strlen(path);
    int count = 0;

    for (i -= 2; i >= 0; i--)
    {
        if(path[i] == '/')
            count++;
    }

    return count;
}

/***************************************************************************************/
/* Description: Given a treeDir tree and the name of a path in the tree, finds a file  */
/*      in the Directory tree and returns a pointer to it.                             */
/***************************************************************************************/
treeDir *find (treeDir *parent, char *path)
{
    treeDir *temp = NULL;
    if (parent != NULL)
    {
        while (parent != NULL)
        {
            if (strcmp (parent->fileInfo->fileName, path) == 0)
                return parent;

            if (parent->isDir)
                if ((temp = find (parent->child, path)) != NULL)
                    return temp;

parent = parent->next;
        }
    }
    return NULL;
}
/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>UNDER CONSTRUCTION<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
/***************************************************************************************/
/* Description: Given a file, creates a tar file.                                      */
/***************************************************************************************/
void createTar ()
{
    struct stat f_stat;
    int errchk;

    /*errchk = fstat (fd,); */

}

/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> NO PURPOSE YET <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*
 * Description: Traverses a directory tree. Could be use for printing or searching.    *
 void traverse (treeDir *parent)
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
/* Description: Given an array of 512 characters, it creates a header structure and    */
/*    initializes every field in the structure. Returns a pointer to structure.        */
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
        pos->fileName[i] = buf[i];
    }
    return tarHeader;
}

/***************************************************************************************/
/* Description: Gets a header from a tar file and returns a pointer to its header      */
/*     structure.                                                                      */
/***************************************************************************************/
header *nextHeader (int fd, int startOfFile)
{
    int n;
    char buf[BLOCK_SIZE];
    
    n = read(fd, buf, BLOCK_SIZE); 
    if (n == -1)
    {
        perror("In function: nextHeader\nread");
        exit(EXIT_FAILURE);
    }
    if (n == 0)
        return NULL;
    
/*    printf("\n im here\n\n");*/
    return newHeader(buf);
}

/***************************************************************************************/
/* Description: moves the position of a tar file descriptor to the next header.        */
/***************************************************************************************/
void skipData (int fd, int skip)
{
    char *buf;

    if (skip == 0)
        return;

    skip += BLOCK_SIZE - (skip % BLOCK_SIZE);

    printf("size of skip %d\n", skip);

    if(lseek(fd, skip, SEEK_CUR) == -1)
    {
        perror("In function: skipData\nread");
        exit(EXIT_FAILURE);
    }

    return;
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
/* Description: converts a number represented in ASCII characters to an intiger.       */
/***************************************************************************************/
int charToInt (char *arr, int leng)
{
    int num = 0;
    int i;
    
    for (i = 0; i < leng && arr[i] != '\0'; i++)
    {
        num = ((num * 10) + (arr[i] - '0'));
    }

    return num;
}

/***************************************************************************************/
/* Description: converts a number represented in ASCII characters to an intiger.       */
/***************************************************************************************/
int Oct2Dec(int oct)
{
    int n, r, s=0, i;
    n = oct;
    for(i = 0; n != 0; i++)
    {
        r = n % 10;
        s += r * pow(8, i);
        n = n / 10;
    }
    return s;
}

/***************************************************************************************/
/* Description: Reads headers in a tar and saves fileName and fileType in an array.    */
/***************************************************************************************/
int readTar(header *headerArray[], int fd)
{
    int count = 0;

    printf("will read tar, count %d\n", count);
    while((headerArray[count] = nextHeader(fd, count)) != NULL)
    {
        skipData(fd, Oct2Dec(charToInt(headerArray[count]->fileSize, FILE_SIZE)));
        count++;
    }

    printf("read tar count: %d \n", count);
    return count;
}

/*>>>>>>>>>>>>>>>>>>>>>>  NEEDS REVISION <<<<<<<<<<<<<<<<<<<<<<<<<<*/
/***************************************************************************************/
/* Description: Given an array of headers and the number of headers in the array,      */
/*      displays the names of the files in an array. (myTar option -t)                 */
/***************************************************************************************/
void printFiles (header *files[], int leng)
{
    int i;

    for (i = 0; i < leng; i++)
        printf("%s\n", files[i]->fileName);

    return;
}

/***************************************************************************************/
/* Description: Given an array of headers and the number of headers in the array,      */
/*      displays the names allong with details of the files in an array.               */
/***************************************************************************************/
void printVerbose (header *files[], int leng)
{
    int i;
    for (i = 0; i < leng; i++)
    {
/*        printf("%s ", stat permisions); */
        printf("%s/", files[i]->userName);
        printf("%s ", files[i]->groupName);
        printf("%8s ", files[i]->fileSize);
/*        printf("%s ", stat date); */
        printf("%s ", files[i]->time);
        printf("%s\n", files[i]->fileName);
    }

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
/***************************************************************************************/
/* Description: Free allocated memory.                                                 */
/***************************************************************************************/
void freeMem (void **arr)
{
    int i;
    for (i = 0; arr[i] != NULL; i++)
        free (arr[i]);
    return;
}
