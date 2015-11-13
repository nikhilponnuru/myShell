#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>


//#define builtin_commands_size 2
#define delimiters " \n\t\a"
#define tokensize 120
#define size_of_bufferString 2000


int builtin_cd(char **sentence_splitted);
int builtin_exit(char **sentence_splitted);




char *take_input()
{
           printf("inside take_input");
//    char string[2048];
      int size = size_of_bufferString;
    char *bufferString = malloc(size * sizeof(char));
  
    if(bufferString==NULL)
      {
              printf("error inside'" );
 	 fprintf(stderr, "In myshell: allocation error occured\n"); 
       	exit(EXIT_FAILURE);
      }
    
    int i,k=0;
   
//    scanf("%c",c);

      
     
   while(1)
   {
          i=getchar();

        if(i==EOF || i=='\n' )
            {
                  bufferString[k++]='\0';
                      return bufferString;
            }
 	else{
//          i=getchar();
          bufferString[k++]=i;
            }

       

      if(k >= size_of_bufferString)
       { 
           size += size_of_bufferString;
           
	bufferString =  realloc(bufferString , size );
             if(bufferString==NULL)
	       {
                     	fprintf(stderr ,"In myshell: allocation error occured \n");
                       exit(EXIT_FAILURE);
                }

        }


   }
}





char **parse_input(char *sentence)
{
          printf("entered parse input");
       int size = size_of_bufferString;
	int pos=0;
        char *tok;
     char **tokens =malloc(tokensize * sizeof(char));
     if(!tokens)
 	{
		fprintf(stderr,"In myshell allocation error occurred");
	}
	
     tok=strtok(sentence,delimiters);
      char **back_them_up;            
	
       while(tok!=NULL)
	{
	   
		tokens[pos]=tok;
		pos++;
		tok=strtok(NULL,delimiters);
		
                
               if(pos >= size)
	          {
			size+=size_of_bufferString;
			back_them_up=tokens;
		    tokens = realloc(tokens , size* sizeof(char));
		    if(!tokens)
			{
				fprintf(stderr,"error occured memory allocation");
                  exit(EXIT_FAILURE);
			}
		
		 }
           tokens[pos]=NULL;
               return tokens;
   }

}



int process_creation(char **tokens_recieved) 
{
           printf("entered process creation");
     int status;       
  pid_t pid,w_pid;
     pid=fork();
	if(pid==0)
	{
                    printf("succesfully entered");
		if(execvp(tokens_recieved[0],tokens_recieved)==-1){
			fprintf(stderr,"error in execvp");
		}
                printf("2 succesfully enetered");
           
    exit(EXIT_FAILURE);
         }
 
 

       else if(pid<0)
		fprintf(stderr,"error in pid");
	else if(pid>0)
          {
                  do{
                printf("inside parents");

		 waitpid(pid, &status, WUNTRACED);
		    }while(!WIFEXITED(status) && !WIFSIGNALED(status));
          }
             return 1;
}



 

int (*builtin_commands[])(char**)={&builtin_cd,&builtin_exit}; //note that all these are global declarations hence seen everywhere in the program


char  *strings[]={"cd","exit"};//here strings array is an array of pointers pointing to cd and exit--we need to take array of pointers because later on we will compare the user command with these 2 strings if they match we shall call the corresponding system call but the user input is **sentence_splitted then sentence_splitted[1] for e.g will be a pointer to first string hence to match type compatibility in strcmp we shall declare array of  pointers.

int  builtin_commands_size()
{
 return sizeof(strings)/(sizeof(char*));
}


int builtin_cd(char **sentence_splitted)
{

	//ok now I entered cd command then let me check what is typed after cd command i.e e.g:--cd Desktop so here Desktop is typed hence 1 parameter must be checked not 0 th paramater


//if(sentence_splitted[1]==NULL)
  //fpr

        if(chdir(sentence_splitted[1])!=0)//here chdir is a system call which is directly provided by posix C but mind that only few such system calls are provided directly 
	{
		fprintf(stderr,"error at cd statement");
       }

  return 1;
}


int builtin_exit(char **sentence_splitted)
{
   exit(0);
//	return 0;  even return 0 will also work because there is a while loop running in loop function which terminates once signal becomes 0 anyhow even exit means whole myshell program stops
}


int execute(char **sentence_splitted)
{

        printf("entered execute'");
	int i,status;
	if(sentence_splitted[0]==NULL)
		return 1;
	for(i=0;i<builtin_commands_size();i++)
	{
		if(strcmp(sentence_splitted[0] , strings[i])==0)
		{
			
                    status=(*builtin_commands[i])(sentence_splitted);
			return status;
		}
	}
         return process_creation(sentence_splitted);
}



void loop()
{
	char **sentence_splitted; //i.e words obtained from senetence
	int signal;
        char *sentence;
	
 
    do
      {
        printf(">");

	sentence = take_input();//take the input(it will be like a one single string )

	sentence_splitted = parse_input(sentence);//split or parse input basing on delimiters like whitespaces(like space or carriage return)
	signal = execute(sentence_splitted);
        free(sentence_splitted);
	free(sentence);
   

      }while(signal);

 /*here we are choosing do..while because for one iteration we dont want to check the signal or condition because initial condition (before starting loop is not known)*/

    // free(


}

int main(int agrc, char **argv )
{
  printf("entered main");



      loop();

  return EXIT_SUCCESS;
}











