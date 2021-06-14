#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>

void wc(int* letters, int* words, int* lines, const int argc, char *argv[]);
void count(const int isStdIn,const char* fileName, int* letters, int* words, int* lines);

int main(int argc,char *argv[]){

  int *letters = (int*)malloc(sizeof(int));
  if(letters == NULL){
    perror("The first Allocation failed!");
    exit(EXIT_FAILURE);
  }
  int *words = (int*)malloc(sizeof(int));
  if(words == NULL){
    perror("The second Allocation failed!");
    exit(EXIT_FAILURE);
  }
  int *lines = (int*)malloc(sizeof(int));
  if(lines == NULL){
    perror("The third Allocation failed!");
    exit(EXIT_FAILURE);
  }
  wc(letters,words,lines,argc,argv);

  free(letters);
  free(words);
  free(lines);

  return 0;

}

void wc(int* letters, int* words, int* lines, const int argc, char *argv[]){

  int totalLetters = 0, totalWords = 0, totalLines = 0;

  struct stat s;
//meaning that the input is from the standart input
  if(argc == 1){
    count(1,"STD_IN",letters,words,lines);
    printf("\t%d\t%d\t%d\t%s\n",*lines,*words,*letters,"STD_IN");
    totalLetters+=*letters;
    totalWords+=*words;
    totalLines+=*lines;
  }
  else{

    for (int i = 1; i < argc; i++) {
      //(stat(argv[i],&s) == 0) determine information about a file based on its file path.
      //(S_ISDIR(s.st_mode)) returning true if its directory
      if((stat(argv[i],&s) == 0) && (S_ISDIR(s.st_mode))){
        printf("wc: %s: Is a directory\n",argv[i]);
        printf("\t0\t0\t0\t%s\n",argv[i]);
      }

      else {
        count(0,argv[i],letters,words,lines);
        printf("\t%d\t%d\t%d\t%s\n",*lines,*words,*letters,argv[i]);
        totalLetters+=*letters;
        totalWords+=*words;
        totalLines+=*lines;
      }
    }
  }
  printf("\t%d\t%d\t%d\ttotal\n",totalLines,totalWords,totalLetters);

}


void count(const int isStdIn ,const char* fileName, int* letters, int* words, int* lines){

  *letters = *words = *lines = 0;

  enum states { WHITESPACE, WORD };
  int lastState = WHITESPACE;
  int fdin = 0; //standard input unless defined other

  if(!isStdIn){
    fdin = open(fileName,O_RDONLY);
    if(fdin == -1){
      perror(fileName);
      exit(EXIT_FAILURE);
    }

  }

  char buff[1];
  int temp = 1;

  while(temp != 0){
    (temp = read(fdin,buff,sizeof(buff)));
    if(temp == -1){
       perror("Could not Read");
       exit(EXIT_FAILURE);
     }

     if(temp == 0 ){
       //if the character befor was not a white space so we need to count
       //the current line to
       if(lastState != WHITESPACE)(*words)++;
       break;
     }
     (*letters)++;
    if(isspace(buff[0])){
      //the last character was not a white space
        if(lastState == WORD) (*words)++;
        if(buff[0] == '\n') (*lines)++;
        lastState = WHITESPACE;
    }
    else lastState = WORD;


  }

  if(close(fdin) == -1){
    perror("Could not close the file");
    exit(EXIT_FAILURE);
  }



}
