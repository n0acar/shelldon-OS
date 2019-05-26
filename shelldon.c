/*
 * shelldon interface program

KUSIS ID: 50086 PARTNER NAME: NEVZAT ACAR		
KUSIS ID: 54161 PARTNER NAME: DUYGU SEZEN ISLAKOGLU

 */

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <string.h>
#include <wait.h>
#include <dirent.h>
//#include <sys/stat.h>
//#include <fcntl.h>


#define MAX_LINE       80 /* 80 chars per line, per command, should be enough. */
#define HISTORY_LIMIT  10

int parseCommand(char inputBuffer[], char tempBuffer[], char *args[],int *background, int *size);
static void search_dir(char dir_name[], char stringToSearch[], int r); 

int main(void)
{

  char inputBuffer[MAX_LINE]; 	        /* buffer to hold the command entered */
  char tempBuffer[MAX_LINE]; 	
int background;             	        /* equals 1 if a command is followed by '&' */
  int size;
  char *args[MAX_LINE/2 + 1];	        /* command line (of 80) has max of 40 arguments */
  pid_t child;            		/* process id of the child process */
  int status;           		/* result from execv system call*/
  int shouldrun = 1;
  int i, upper;
  int isRedirectedAppend;
  int isRedirectedTruncate;
  int fileRedirected;
  FILE *pathFile;

  
  char history[HISTORY_LIMIT][MAX_LINE];		/* history array*/
  int history_count;		/* for loop size */

  char hour[3];
  char minute[3];
  history_count=0;

  char prev_oldest[64];
  strcpy(prev_oldest, "");
	
  while (shouldrun){            		/* Program terminates normally inside setup */
    background = 0;
    
    shouldrun = parseCommand(inputBuffer,tempBuffer,args,&background,&size);       /* get next command */


    if (strncmp(inputBuffer, "exit", 4) == 0){
      shouldrun = 0;     /* Exiting from shelldon*/
	char *args_kernel_rm[] = {"/usr/bin/sudo", "rmmod", "our_simple", NULL}; 
	execv(args_kernel_rm[0], args_kernel_rm);
}
    pid_t parentId = getpid();
    pid_t rv;
    if (shouldrun) {
      /*
	
	After reading user input, the steps are 
	(1) Fork a child process using fork()
	(2) the child process will invoke execv()
	(3) if command included &, parent will invoke wait()
       */

/*
	int redirectIndex = 0;
	for(int i=0; i<size;i++) {
		if (strcmp(args[i], ">")==0) {
			isRedirectedAppend=1;
			redirectIndex=i+1;
			args[i]=NULL;
			break;}
		else if (strcmp(args[i], ">>")==0) {
			isRedirectedTruncate=1;
			redirectIndex=i+1;
			args[i]=NULL;
			break;}
	} 
	if (isRedirectedAppend || isRedirectedTruncate) {
		int fileRedirected = open(args[redirectIndex], O_APPEND | O_WRONLY);
		args[redirectIndex] = NULL;
	}
	

*/

	if(strcmp(args[0], "birdakika") == 0){
		int pointFlag=0;
		int len=strlen(args[1]);
		int index=0;	
		for(int i=0;i<len;i++){
			if (pointFlag==0) {
				if(args[1][i]=='.') {
					hour[index]='\0';
					pointFlag=1;
					index=-1;
				}
				else {
					hour[index]=args[1][i];
					
				}
				
			}
			else {
				if(index==2) {
					minute[index]='\0';
				}
				else {
					minute[index]=args[1][i];
				}
			}
			index++;
			
				
		}
		//printf("%s\n", hour);
		//printf("%s\n", minute);
			
	}

	if(strcmp(args[0], "history") != 0 && strcmp(args[0], "!")!=0 && strcmp(args[0], "!!")!=0) {			
			
			for(int i=history_count;i>0;i--) {			
				strcpy(history[i],history[i-1]);
			
			//	printf("i: %s  i-1: %s", history[i], history[i-1]);
			}

			if (history_count<HISTORY_LIMIT) {
				history_count++;
			}
			else {
				history_count=HISTORY_LIMIT;
			}

			strcpy(history[0],tempBuffer);
	}
	

	

	
	rv= fork();
	
	if(rv==0){
		if(strcmp(args[0], "whatisnew") == 0) {	
			char categories[]="World\nU.S.\nPolitics\nN.Y.\nBusiness\nOpinion\nTech\nScience\nHealth\nSports\nArts\nBooks\nStyle\nFood\nTravel\nMagazine\nT Magazine\nReal Estate\nVideo";			
			if(size<2)printf("%s",categories);		
			else{char *args_custom[] = {"/usr/bin/w3m", "-v", "",NULL};
			char section[200];
			strcpy(section,"https://www.nytimes.com/section/");
			strcat(section, args[1]);
			args_custom[2] = section;
			execv(args_custom[0],args_custom);}

		}
		else if(strcmp(args[0], "birdakika") == 0){
			char *argMusic[] = {"/usr/bin/mpg321", "", NULL}; 
			argMusic[1]= args[2];
			char cronCommand[200];
			strcpy(cronCommand,"echo '");
			strcat(cronCommand, minute);
			strcat(cronCommand, " ");
			strcat(cronCommand, hour);
			strcat(cronCommand, " * * * mpg321 ");
			strcat(cronCommand, args[2]);
                  	strcat(cronCommand, "' | crontab -");
                  	system(cronCommand);
			execv(argMusic[0], argMusic);

		}

		else if(strcmp(args[0], "codesearch") == 0){
			if (size >=3){
			if(strcmp(args[1], "-r") == 0){
				char *stringToMatch = args[2];
				search_dir(".", stringToMatch, 1);
			}
			else if (strcmp(args[2], "-f") == 0) {
				char *stringToMatch = args[1];
				char *fileToSearch = args[3];
				FILE * fp = fopen(fileToSearch, "r");
				if (fp == NULL) printf("%s\n", "No file with this path.");
				char word[200];
				char line[200];
				strcpy(word, "");
				int index=0;
				int indexLine=0;
				char c;
				int count=0;
				int lineEnd=0;
				while((c = fgetc(fp)) != EOF) {

					if ((strcmp(word, stringToMatch) == 0) && !lineEnd) {
						 lineEnd=1;
					}
					if(c == ' ' || c == '\n' || c == '\t') {
						strcpy(word, "");
						index=0;
						if (c == '\n'){
							if (lineEnd) {
								printf("%d: %s, %s\n", count, fileToSearch, line);
								lineEnd=0;	
							}
						 	count++;

							strcpy(line, "");
							
							indexLine=0;
						}
						

					}
					
					else {
						word[index]=c;
						word[index+1]='\0';
						index++;
					}
					if(c != '\n'&& c != '\t') {
							line[indexLine]=c;
							line[indexLine+1]='\0';
							indexLine++;
						}
				}
			fclose(fp);
		


			}
			}

			else {
				char *stringToMatch = args[1];
				search_dir(".", stringToMatch, 0);
			}
		}
		
		else if(strcmp(args[0], "oldestchild") == 0){
			char *args_kernel[] = {"/usr/bin/sudo", "insmod", "our_simple.ko", "processID=-1", NULL}; 
    			//char *processID = malloc(strlen("processID=") + strlen(args[1]) + 1); 
   			char processID[64];
			strcpy(processID, "processID=");
			printf("%d",size);
			if(size>1){
   				strcat(processID, args[1]);
				args_kernel[3]= processID;
			}

			pid_t child = fork();
			if(size <=1) execv(args_kernel[0], args_kernel);
			else if(strcmp(args[1],prev_oldest)!=0 && strcmp(prev_oldest,"")!=0){
				if (child == 0) {
					char *args_kernel_rm[] = {"/usr/bin/sudo", "rmmod", "our_simple", NULL}; 
					printf("child process:%s\n",prev_oldest);				 	
					execv(args_kernel_rm[0], args_kernel_rm);
				}
			}
			else if(strcmp(args[1],prev_oldest)!=0){
				if (child > 0 ){
					
						wait(NULL);
						printf("old process before:%s\n",prev_oldest);	
						strcpy(prev_oldest, args[1]);
						printf("old process after:%s\n",prev_oldest);
						execv(args_kernel[0], args_kernel);
					}
			}
			else if (child!=0 &&  strcmp(args[1],prev_oldest)==0) printf("The module is already loaded.");
		}
		
		else if(strcmp(args[0], "history") == 0){
			
        		for(int i=0;i<history_count; i++) {
				printf("%d %s\n", history_count-i, history[i]);
			}
		}

		else if(strcmp(args[0], "!!") == 0){
					
			char *args_history[]= {"",NULL};	
			


/////////////////////////////  /ENVIRONMENT SEARCH/  //////////////////////////////////////////
			char dst[50];
			strcpy(dst, "which ");
			strcat(dst, history[0]);
			strcat(dst, " > readFile");
			args_history[0]=dst;
			system(args_history[0]);
   			char c;

    			pathFile = fopen("readFile", "r");
    			if (pathFile == NULL) {
        		printf("fopen failed\n");
        		return 1;
    			}


    		int i=0;
    		while((c = fgetc(pathFile)) != EOF) {
			if (c == '\n') {
		    		args_history[0][i]='\0'; // print '\n', as it is part of the line
		    		break;
			}
			args_history[0][i]=c;
			i++;
    		}
    			fclose(pathFile);
			remove("readFile");
////////////////////////////////////////////////////////////////////////////////////////////
			execv(args_history[0],args_history);

		}


		else if(strcmp(args[0], "!") == 0){
			int n= atoi(args[1]);
			printf("%d",n);
			char *args_history[]= {"",NULL};	
			char dst[50];
			strcpy(dst, "which ");
			strcat(dst, history[history_count-n]);
			strcat(dst, " > readFile");
			args_history[0]=dst;
			system(args_history[0]);
   			char c;

    			pathFile = fopen("readFile", "r");
    			if (pathFile == NULL) {
        		printf("fopen failed\n");
        		return 1;
    			}

    		int i=0;
    		while((c = fgetc(pathFile)) != EOF) {
			if (c == '\n') {
		    		args_history[0][i]='\0'; // print '\n', as it is part of the line
		    		break;
			}
			args_history[0][i]=c;
			i++;
    		}
    			fclose(pathFile);
			remove("readFile");
			execv(args_history[0],args_history);

		}

		else {
			char dst[50];
			strcpy(dst, "which ");
			strcat(dst, args[0]);
			strcat(dst, " > readFile");
			args[0]=dst;
			system(args[0]);
   			char c;
	
    			pathFile = fopen("readFile", "r");
    			if (pathFile == NULL) {
        		printf("fopen failed\n");
        		return 1;
    			}
 
    		int i=0;
    		while((c = fgetc(pathFile)) != EOF) {
        	if (c == '\n') {
            		args[0][i]='\0'; // print '\n', as it is part of the line
            		break;
        	}
		args[0][i]=c;
		i++;
    	}

    			fclose(pathFile);
			remove("readFile");
			execv(args[0], args);
		}
		/*
		if (isRedirectedAppend || isRedirectedTruncate) {
			isRedirectedAppend =0;
			isRedirectedTruncate =0;
			dup2(fileRedirected, STDOUT_FILENO);
			close(fileRedirected);
		}
		*/
		

		return 0;

		
 	}
	else {	
		if(strcmp(args[0], "oldestchild") == 0) 			strcpy(prev_oldest, args[1]);
		if(!background){
			wait(NULL);
		}
		
	}
	

    }
  }
  return 0;
}

/** 
 * The parseCommand function below will not return any value, but it will just: read
 * in the next command line; separate it into distinct arguments (using blanks as
 * delimiters), and set the args array entries to point to the beginning of what
 * will become null-terminated, C-style strings. 
 */

int parseCommand(char inputBuffer[],char tempBuffer[], char *args[],int *background,int size[1])
{
    int length,		/* # of characters in the command line */
      i,		/* loop index for accessing inputBuffer array */
      start,		/* index where beginning of next command parameter is */
      ct,	        /* index of where to place the next parameter into args[] */
      command_number;	/* index of requested command number */
    
    ct = 0;
	
    /* read what the user enters on the command line */
    do {
	  printf("shelldon>");
	  fflush(stdout);
	  length = read(STDIN_FILENO,inputBuffer,MAX_LINE); 
    }
	
    while (inputBuffer[0] == '\n'); /* swallow newline characters */
strcpy(tempBuffer, "");
strcpy(tempBuffer,inputBuffer);
tempBuffer[length-1]='\0';

    /**
     *  0 is the system predefined file descriptor for stdin (standard input),
     *  which is the user's screen in this case. inputBuffer by itself is the
     *  same as &inputBuffer[0], i.e. the starting address of where to store
     *  the command that is read, and length holds the number of characters
     *  read in. inputBuffer is not a null terminated C-string. 
     */    
    start = -1;
    if (length == 0)
      exit(0);            /* ^d was entered, end of user command stream */
    
    /** 
     * the <control><d> signal interrupted the read system call 
     * if the process is in the read() system call, read returns -1
     * However, if this occurs, errno is set to EINTR. We can check this  value
     * and disregard the -1 value 
     */

    if ( (length < 0) && (errno != EINTR) ) {
      perror("error reading the command");
      exit(-1);           /* terminate with error code of -1 */
    }
    
    /**
     * Parse the contents of inputBuffer
     */

    for (i=0;i<length;i++) { 
      /* examine every character in the inputBuffer */
      
      switch (inputBuffer[i]){
      case ' ':
      case '\t' :               /* argument separators */
	if(start != -1){
	  args[ct] = &inputBuffer[start];    /* set up pointer */
	  ct++;
	}
	inputBuffer[i] = '\0'; /* add a null char; make a C string */
	start = -1;
	break;
	
      case '\n':                 /* should be the final char examined */
	if (start != -1){
	  args[ct] = &inputBuffer[start];     
	  ct++;
	}
	inputBuffer[i] = '\0';
	args[ct] = NULL; /* no more arguments to this command */
	break;
	
      default :             /* some other character */
	if (start == -1)
	  start = i;
	if (inputBuffer[i] == '&') {
	  *background  = 1;
	  inputBuffer[i-1] = '\0';
	}
	
    
    
      } /* end of switch */
    }    /* end of for */
    
    /**
     * If we get &, don't enter it in the args array
     */
    
    if (*background)
      args[--ct] = NULL;
    *size=ct;

    args[ct] = NULL; /* just in case the input line was > 80 */

    return 1;
    
} /* end of parseCommand routine */


static void search_dir(char dir_name[], char stringToSearch[], int r) {
	struct dirent *de;  // Pointer for directory entry 
  	DIR *dr = opendir(dir_name); // opendir() returns a pointer of DIR
    	if (dr == NULL) {  // opendir returns NULL if no dir
        	printf("Could not open current directory" ); 
    	} 
	char searchString[120];
	strcpy(searchString, stringToSearch);
  	while ((de = readdir(dr)) != NULL) {
		if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0)
                	continue;

		char path[255];
		
		
		if (r) {
			if (de->d_type & DT_DIR) {
				char newPath[255];
				strcpy(newPath, dir_name);
				strcat(newPath, "/");
	    			strcat(newPath, de->d_name);
                		/* Recursively call "list_dir" with the new path. */
                		search_dir(newPath, searchString, 1);
			}
		}
		
		strcpy(path, dir_name);
		strcat(path, "/");
		strcat(path, de->d_name);
		
		if (!(de->d_type & DT_DIR)) {
			FILE * fp = fopen(path, "r");
				if (fp == NULL) printf("%s\n", "No file with this path.");
				char word[200];
				char line[200];
				strcpy(word, "");
				int index=0;
				int indexLine=0;
				char c;
				int count=0;
				int lineEnd=0;
				while((c = fgetc(fp)) != EOF) {

					if ((strcmp(word, searchString) == 0) && !lineEnd) {
						 lineEnd=1;
					}
					if(c == ' ' || c == '\n' || c == '\t') {
						strcpy(word, "");
						index=0;
						if (c == '\n'){
							if (lineEnd) {
								printf("%d: %s -> %s\n", count, path, line);
								lineEnd=0;	
							}
						 	count++;

							strcpy(line, "");
							
							indexLine=0;
						}
						

					}
					
					else {
						word[index]=c;
						word[index+1]='\0';
						index++;
					}
					if(c != '\n'&& c != '\t') {
							line[indexLine]=c;
							line[indexLine+1]='\0';
							indexLine++;
						}
				}
			fclose(fp);
		
			}

		} 
		
  	closedir(dr);  
}
	
