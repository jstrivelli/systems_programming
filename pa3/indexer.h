#ifndef INDEXER_H
#define INDEXER_H


#include <dirent.h>
#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "sorted-list.h"
#include "tokenizer.h"

struct Info {
  char *token;
  SortedListPtr list;
};

typedef struct Info Info;

struct Record {
  char *filename;
  int occur;
} ;

typedef struct Record Record;


int compareInfo(void *p1, void *p2);


int recordCmp(void *p1, void *p2);

void destroy_Info(void *p);

void destroy_record(void *p);


void print_Output(FILE* fp, SortedListPtr table);

void insertFile(SortedListPtr table, char *filename);

int saveFilePath(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf);

int isHiddenFile(char *filename);


#endif