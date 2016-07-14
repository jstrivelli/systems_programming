

#define TRACE_PATH_FILE ".file_paths.tmp"


#include "indexer.h"
/*
 *  Indexer.c is the final layout of the design. It will print out the information to a JSON standard
 *  We can call functions from the dictionary library that will allow us to input records from files
 *  that include its words, there filepath, and the frequency of that word in a particular file.
 */


int main(int argc, char *argv[])
{
  char * expected;
  if (argc != 3) {
    expected = "./index <inverted-index file name> <directory or file name>";
    fprintf(stderr, "Error: incorrect format, expecting: \n %s\n", expected);
    return 0;
  }

  SortedListPtr table = SLCreate(compareInfo, destroy_Info);

  char *input_arg = argv[2];
  char *output_file = argv[1];

  struct stat info;
  stat(input_arg, &info);

  /*We need to insert this file*/
  if (!(S_ISREG(info.st_mode)) && !(S_ISDIR(info.st_mode))) {
        fprintf(stderr, "Error: %s is not a file or directory\n", input_arg);
        return 0;
  }
  if (S_ISREG(info.st_mode)) {
    insertFile(table, input_arg);
  }
  /*This means that this is a directory*/
  if (S_ISDIR(info.st_mode)) {
    remove(TRACE_PATH_FILE);
    int flags = 0;
    nftw(input_arg, saveFilePath, 20, flags);

    // call insertFile for each file in TRACE_PATH_FILE
    FILE *tmp;
    tmp = fopen(TRACE_PATH_FILE, "r");

    char *line = NULL;
    size_t len = 0;
    ssize_t numchars;
    /*This will traverse and create tokens for each line of each file*/
    while ((numchars = getline(&line, &len, tmp)) != -1) {
        // remove \n at end of line
        line[numchars - 1] = '\0'; numchars--;
        /*This checks if we came across a hidden file in our traversing of files and directories*/
        if (isHiddenFile(line) == 0) {
          insertFile(table, line);
        }
    } 
  


    free(line);

    remove(TRACE_PATH_FILE);
    fclose(tmp);
  }

  // if output_file already exists, prompt user
  if (stat(output_file, &info) == 0) {
      printf("%s file already exists. Overwrite? (y or n)\n", output_file);
      char ans = getchar();
      while (!(ans == 'y' || ans == 'n')) {
        ans = getchar();
      }
      /*We dont want to index*/
      if (ans == 'n') {
            printf("We are not indexing %s\n", input_arg);
      } 
      /*We want to index*/
      else {
        printf("We are Indexing %s\n", input_arg);
        FILE *write_fp;
        write_fp = fopen(output_file, "w");
        print_Output(write_fp, table);
        fclose(write_fp);
      }
  } 
  else {
      FILE *write_fp;
      write_fp = fopen(output_file, "w");
      print_Output(write_fp, table);
      fclose(write_fp);
  }

  // We need to destroy all information because we are done using it
  SLDestroy(table);

  return 0;

 }

int compareInfo(void *p1, void *p2)
{
  Info *i1 = (Info *)p1;
  Info *i2 = (Info *)p2;

  return compareStrings(i1->token, i2->token);
}


int recordCmp(void *r1, void *r2) {
    Record *rec1 = (Record *) r1;
    Record *rec2 = (Record *) r2;
    int c = compareInts(&(rec1->occur), &(rec2->occur));
  if(c != 0) { 
    return compareStrings(rec1->filename, rec2->filename);
  }
  return c;  
}


void destroy_Info(void *p)
{
  Info *t = (Info *)p;
  free(t->token);
  SLDestroy(t->list);
  free(t);
}

void destroy_record(void *p)
{
  Record *r = (Record *)p;
  free(r->filename);
  free(r);
}


int isHiddenFile(char *filename) {
  char *token;
  TokenizerT *tokenizer = TKCreate("/", filename);

  while ((token = TKGetNextToken(tokenizer))) {
    if (token[0] == '.' &&
        strcmp(token, ".") != 0 &&
        strcmp(token, "..") != 0) {
      free(token);
      TKDestroy(tokenizer);
      return 1;
    }
   free(token);
  }
  free(token);
  TKDestroy(tokenizer);
  return 0;
}


int saveFilePath(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf)
{
  FILE *tmp;
  tmp = fopen(TRACE_PATH_FILE, "a");

  // fpath cannot contain ".file_paths.tmp"
  if (tflag == FTW_F && (strstr(fpath, TRACE_PATH_FILE) == NULL)) {
    fprintf(tmp, "%s\n", fpath);
  }

  fclose(tmp);

  return 0;
}


void insertFile(SortedListPtr table, char *filename) {
    FILE *fp;
    Record *temp;
    Record *r;
    fp = fopen(filename, "r");

    if (fp == NULL) {
      fprintf(stderr, "Error: This file does not exist: %s\n", filename);
      fclose(fp);
      return;
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t numchars;

    while ((numchars = getline(&line, &len, fp)) != -1) {
      TokenizerT *tokenizer = TKCreate("", line);
      char *token;
      while ((token = TKGetNextToken(tokenizer))) {
          /*Makes the token lowercase*/
          int i = 0;
          for(i = 0; token[i]; i++) {
            token[i] = tolower(token[i]);
          }

          Info *t = (Info *)malloc(sizeof(Info));
          t->token = malloc(strlen(token) + 1);
          strcpy(t->token, token);

          NodePtr Info_node = SLFind(table, t);

          // alloc new space for new filenames, i dont understand this
          char *file = malloc(strlen(filename) + 1);
          strcpy(file, filename);

          if (!Info_node) {
             
              t->list = SLCreate(recordCmp, destroy_record);

              r = (Record *) malloc(sizeof(Record));
              r->filename = file;
              r->occur = 1;

              SLInsert(t->list, r);
              SLInsert(table, t);
          } 

         else {
            free(t);
              t = (Info *) Info_node->data;

              SortedListPtr records = t->list;
              NodePtr record_node = records->front;
              while (record_node) {
                temp = (Record *)record_node->data;
                if (compareStrings(file, temp->filename) == 0) {
                  break;
                }
                record_node = record_node->next;
              }

              if (!record_node) {
                r = (Record *)malloc(sizeof(Record));
                r->filename = file;
                r->occur = 1;
                SLInsert(records, r);
              } 
              else {

                temp = (Record *) record_node->data;

                r = (Record *) malloc(sizeof(Record));
                r->filename = file;
                r->occur = temp->occur;

                SLRemove(records, temp);
                r->occur++;
                SLInsert(records, r);

              }
          }

        free(token);

      }
      free(token);
      TKDestroy(tokenizer);
    }

    free(line);
    fclose(fp);
}




void print_Output(FILE* fp, SortedListPtr table) {
      SortedListIteratorPtr iter = SLCreateIterator(table);
      Info *t;
      fprintf(fp, "{\"list\" : \n");
      while((t = (Info *)SLNextItem(iter))) {


            fprintf(fp, "\t{\"%s\" : [\n", t->token);
            fflush(fp);

            SortedListIteratorPtr iter2 = SLCreateIterator(t->list);

            // manually print 1st record to avoid whitespace issues
            Record *r = (Record *)SLNextItem(iter2);
            fprintf(fp, "\t\t{%s: %d},\n", r->filename, r->occur);

            while((r = (Record *) SLNextItem(iter2))) {
                  fprintf(fp, "\t\t{%s: %d}\n", r->filename, r->occur);
                  fflush(fp);
            }

            fflush(fp);

            SLDestroyIterator(iter2);
            fprintf(fp, "\t]}\n");


      }
      fprintf(fp, "\n]}\n");
      SLDestroyIterator(iter);
}
