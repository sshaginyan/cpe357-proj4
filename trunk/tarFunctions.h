/**
 * CPE 357
 * -----------------
 * Header file for tarFunctions.c
 *
 * Last Modified: Fri Feb 25, 2011
 * @author Luis Castillo, Ricardo Tijero
 */
#ifndef TARFUNCTIONS_H
#define TARFUNCTIONS_H

#define BUF_SIZE 100
#define BLOCK_SIZE 512

/* Header components' sizes */
#define NAME_SIZE 100
#define MODE_SIZE 8
#define UID_SIZE 8
#define GID_SIZE 8
#define FILE_SIZE 12
#define TIME_SIZE 12
#define CHKSM_SIZE 8
#define LNKNAME_SIZE 100
#define USTAR_SIZE 6
#define VER_SIZE 2
#define UNAME_SIZE 32
#define GNAME_SIZE 32
#define DEVNUM_SIZE 8
#define PREFIX_SIZE 155

/* Tar header structure */
typedef struct header header;

struct header{
    char fileName[NAME_SIZE];
    char mode[MODE_SIZE];
    char userID[UID_SIZE];
    char groupID[GID_SIZE];
    char fileSize[FILE_SIZE];
    char time[TIME_SIZE];
    char checksum[CHKSM_SIZE];
    char fileType;
    char linkName[LNKNAME_SIZE];
    char ustar[USTAR_SIZE];
    char version[VER_SIZE];
    char userName[UNAME_SIZE];
    char groupName[GNAME_SIZE];
    char devMajNum[DEVNUM_SIZE];
    char devMinNum[DEVNUM_SIZE];
    char prefix[PREFIX_SIZE];
};

/* Tree Directory structure */
typedef struct treeDir treeDir;

struct treeDir{
    header *fileInfo;
    int level;        
    int isDir;
    struct treeDir *next;
    struct treeDir *parent;
    struct treeDir *child;
};

/* Function prototypes */
treeDir *newDir (header *aHeader);
treeDir *makeTree (header *headers[]);
treeDir *getParent (treeDir *parent, char *path);
int getLevel (char *path);
treeDir *find (treeDir *parent, char *path);
/*void traverse (treeDir *parent);      NO PURPOSE YET*/
header *newHeader (char *buf);
header * nextHeader (int fd, int startOfFile);
int isNullBlock (char *block);
int charToInt (char *arr, int leng);
int readTar (header *headerArray[], int fd);
void printFiles (header *files[], int leng);
void printVerbose (header *files[], int leng);
void printHelp (void);
void printVersion (void);
void freeMem (void **arr); /* DON'T KNOW IF ITS GOING TO WORK WITH AnyType*/

#endif                                      
