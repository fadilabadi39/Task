#include <stdlib.h>  /* For exit() function */
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
void printInformation(char *name);
void readInformation(char *name,char *c);
char* parserForHypen(char *command);
void takeDesicion(char **command);
void editBadItem(char *name,char *command);
void editGoodItem(char *name,char *command);
void deliverdItem(char *name,char *command);
void writeOnFile(char *name,char *str);
void delay(int m);
void printInformation(char *name){
    char name_of_file[40];
    strcpy(name_of_file,name);
    strcat(name_of_file,".txt");
    FILE *fptr;
    char c[30];
    if ((fptr = fopen(name_of_file, "r")) == NULL)
    {
        printf("There is no item,sorry!");

        exit(1);         
    }

    // reads text until newline 
    fscanf(fptr,"%[^\n]", c);
    fclose(fptr);
    
}
void delay(int milliseconds)
{
    long pause;
    clock_t now,then;

    pause = milliseconds*(CLOCKS_PER_SEC/1000);
    now = then = clock();
    while( (now-then) < pause )
        now = clock();
}

void readInformation(char *name, char *ce){
   
     char name_of_file[40];
    strcpy(name_of_file,name);
    strcat(name_of_file,".txt");
    FILE *fptr;
    char c[30];
    if ((fptr = fopen(name_of_file, "r")) == NULL)
    {
        printf("Theres is no item,sorry!");

        exit(1);         
    }
    

    // reads text until newline 
    fscanf(fptr,"%[^\n]", c);
    fclose(fptr);
    strcpy(ce,c);
   
}

void writeOnFile(char *name,char *str){
    
    FILE *fp;
    char name_of_file[40];
    strcpy(name_of_file,name);
    strcat(name_of_file,".txt");
    int fd;
    struct flock lock;
    size_t length = strlen(str);
    fd = open (name_of_file, O_WRONLY);

    /* Initialize the flock structure. */
    memset (&lock, 0, sizeof(lock));
    lock.l_type = F_WRLCK;
    /* Place a write lock on the file. */
    fcntl (fd, F_SETLKW, &lock);

    printf ("Please Press Enter to confirm ");
    /* Wait for the user to hit Enter. */
    getchar ();
    write(fd,str,length);
    printf ("Done!\n");
    /* Release the lock. */
    lock.l_type = F_UNLCK;
    fcntl (fd, F_SETLKW, &lock);

 close (fd);
}



char* parserForHypen(char *command){
    
    char splitStrings[10][10]; 
    int i,j,cnt;
    char *strs[7];
    
    j=0; cnt=0;
     
    for(i=0;i<=(strlen(command));i++)
    {
     
        if(command[i]=='-'||command[i]=='\0')
        {
            splitStrings[cnt][j]='\0';
            cnt++;  //for next word
            j=0;    //for next word, init index to 0
        }
        else
        {
            splitStrings[cnt][j]=command[i];
            j++;
        }
    }
        
                }




void takeDesicion(char **command){
    
    if((strcmp(*(command + 0),"seller")==0) && (strcmp(*(command + 3),"add"))==0){
        editGoodItem(*(command + 1),*(command + 2));
        free(command);
    }
    
    else if((strcmp(*(command + 0),"seller")==0) && (strcmp(*(command + 3),"del"))==0){
        editBadItem(*(command + 1),*(command + 2));
        free(command);
    }
    else if((strcmp(*(command + 0),"customer")==0) && (strcmp(*(command + 3),"order"))==0){
        deliverdItem(*(command + 1),*(command + 2));
        free(command);
    }
    
    else {
        printf("error in parsing");
        }
    
    
}

char** globalParser(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = (char**)malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}


void editGoodItem(char *name,char *command){
    
    char **generalPart,**firstPart,** secondPart;
    char c[30];
    char stringToFile[40];
    char buffer[40];
    int number1,number2,number3,sum;
    readInformation(name,c);
    generalPart = globalParser(c,',');
    firstPart = globalParser(*(generalPart+ 1),':');
    secondPart= globalParser(*(generalPart + 2),':');
    number1 = atoi(*(firstPart + 1));
    number2 = atoi(command);
    sum = number1 + number2;
    strcpy(stringToFile,name);
    number3 = atoi(*(secondPart + 1));
    sprintf(buffer,"%s:A,good:%d,bad:%d",stringToFile,sum,number3);
    writeOnFile(name,buffer);
    
    
}
void deliverdItem(char *name,char *command){
    
    char **generalPart,**firstPart,** secondPart;
    char c[30];
    char stringToFile[40];
    char buffer[40];
    int number1,number2,number3,sum;
    readInformation(name,c);
    generalPart = globalParser(c,',');
    firstPart = globalParser(*(generalPart+ 1),':');
    secondPart= globalParser(*(generalPart + 2),':');
    number1 = atoi(*(firstPart + 1));
    number2 = atoi(command);
    sum = number1 - number2;
    if(sum < 0)
    {
        printf("%s","can`t service this amount,please wait seller to serve u \n");
        delay(60000);
        printf("%s","try now \n");
    }
    else
     {
        strcpy(stringToFile,name);
        number3 = atoi(*(secondPart + 1));
        sprintf(buffer,"%s:A,good:%d,bad:%d",stringToFile,sum,number3);
        writeOnFile(name,buffer);
    
    }
}

void editBadItem(char *name,char *command){
    
    char **generalPart,**firstPart,** secondPart;
    char c[30];
    char stringToFile[40];
    char buffer[40];
    int number1,number2,number3,sum;
    readInformation(name,c);
    generalPart = globalParser(c,',');
    firstPart = globalParser(*(generalPart+ 1),':');
    secondPart= globalParser(*(generalPart + 2),':');
    number1 = atoi(*(secondPart + 1));
    number2 = atoi(command);
    sum = number1 - number2;
    strcpy(stringToFile,name);
    number3 = atoi(*(firstPart + 1));
    sprintf(buffer,"%s:A,good:%d,bad:%d",stringToFile,number3,sum);
    writeOnFile(name,buffer);
    
    
}
    void help(){
        printf(" -add: The seller will add extra <count> items to the good item file. Using this option. \n " );
        printf(" Example : seller – chair - 4 – add \n");
        printf(" -del: The seller can remove <count> items from the bad_items in the item file. Using this option \n " );
        printf(" Example : seller – chair – 4 – del \n");
        printf(" count of item from customer. \n");
        printf(" Example : customer - table \n");
        printf(" customer can deliver amount of any item \n");
        printf(" Example : customer - table - 5");
        
        
    }
int main(int argc, char** argv)
   {
       char** tokens;
      // printf("%s",argv[1]);
 
       // readInformation("table",segment);
       if(strcmp(argv[1],"--help")==0)
       {
          help();
          }
 
       else
       {
           tokens = globalParser(argv[1],'-');
           takeDesicion(tokens);
 
          printf("\n");

       }
 
       return 0;
   }
 

