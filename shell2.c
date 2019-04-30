#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>



int createPipe(char **cmd, int *start, int numPipe){

  char *cat_args[] = {"cat", "fruit", NULL,"grep", "apple", NULL};

  // for (int i = 0; i < numPipe; i++) {
  //   /* code */
  //   printf("start: %d\n", start[i]);
  // }
  int count = 1;
  for (int i = 0; i < numPipe; i++) {
    /* code */
    count = count * 2;
  }

  printf("%d\n", count );
  int pipefd[count];
  int pid;
  int status;

  pipe(pipefd);
  if(numPipe > 1){
    pipe(pipefd + numPipe);
  }
  for (int i = 0; i < numPipe+1; i++) {
    /* code */
    //printf("%d\n", i );
    pid = fork();

    if(pid==0){
      if(i == 0){
        dup2(pipefd[i+1],i+1);

        for (int i = 0; i < numPipe; i++) {
          /* code */
        }

        for (int i = 0; i < count; i++) {
          /* code */
          close(pipefd[i]);
        }


        if(execvp(cmd[start[i]],cmd+start[i]) == -1){
          printf("error 1\n");
        }
      }
      if(i == numPipe ){
        dup2(pipefd[i],i-i);

        for (int i = 0; i < count; i++) {
          /* code */
          close(pipefd[i]);
        }

        if(execvp(cmd[start[i]],cmd+start[i]) == -1){
          printf("error 2\n");

        }
      }
      else{
        dup2(pipefd[i-1],i-1);

        dup2(pipefd[i+2],i);

        for (int i = 0; i < count; i++) {
          /* code */
          close(pipefd[i]);
        }

        if(execvp(cmd[start[i]],cmd+start[i]) == -1){
          printf("error\n");
        }
      }
      // replace standard input with
    }
  }

  for (int i = 0; i < count; i++) {
    /* code */
    close(pipefd[i]);
  }


  for (size_t i = 0; i < 3; i++) {
    /* code */
    wait(&status);
  }
  // pid=fork();
  //
  // if (pid == 0)
  //
  //   {
  //     //printf("here\n" );
  //     // child gets here and handles "grep Villanova"
  //
  //     // replace standard input with input part of pipe
  //
  //     dup2(pipefd[0], 0);
  //
  //     // close unused hald of pipe
  //
  //     close(pipefd[1]);
  //
  //     // execute grep
  //
  //     if(execvp(cmd2[0], cmd2)!=0){
  //       printf("error\n");
  //     };
  //   }
  // else
  //   {
  //     // parent gets here and handles "cat scores"
  //
  //     // replace standard output with output part of pipe
  //
  //     dup2(pipefd[1], 1);
  //
  //     // close unused unput half of pipe
  //
  //     close(pipefd[0]);
  //
  //     // execute cat
  //
  //     execvp(cmd[0], cmd);
  //   }

    return 1;

}

int createProcess( char **args, int *start1, int chain){

  pid_t pid, wpid;
  int status;
  start1[0]=0;

  if(strcmp(args[0], "exit") == 0){
    printf("%s\n", "GOODBYE!" );
    exit(0);
  }


/////////////////////////////////////
for (int i = 0; i < chain + 1; i++) {
  /* code */

      if(strcmp(args[start1[i]],"cd")==0){
        chdir(args[start1[i]+1]);
      }
      else{
      pid = fork();
      if(pid == 0){
        //child

      if(execvp(args[start1[i]],args+start1[i]) == -1){
        perror("exec");
      }

        exit(EXIT_FAILURE);
      }
      else if(pid < 0) {
        perror("forking");
      }
          // parent
      else{
          if(wait(0) == -1){
            perror("wait");
          }
      }
      }
}


  return 1;
}



int main(){


  char cwd[1024];
  int position =0;
  int size = 0;
  while(1){
    position = 0;
    size =0;
    printf("%s", "ROZZISHELL:" );
    getcwd(cwd,sizeof(cwd));
    printf("%s", cwd );
    printf("> ");

    // read in line
    char *line = NULL;
    ssize_t bufsize = 0;
    getline(&line, &bufsize,stdin);
    line[strlen(line) - 1] = 0;


    printf("%s\n",line );

    // parse line

    char **cmd = malloc(64 * sizeof(char*));
    int *start = malloc(64 * sizeof(char*));
    int *start1 = malloc(64 * sizeof(char*));
    char **cmd1 = malloc(64 * sizeof(char*));
    char *token;
    int pipe = 0;
    int chain = 0;
    token = strtok(line, " ");
    while(token != NULL){
      size++;
      cmd[position] = token;
      position++;
      token = strtok(NULL," ");
    }
    // set last postion to null
    cmd[position] = NULL;

    start[0] = 0;
    start1[0] = 0;

    //printf("%d\n", size );
    int j=1;
    for (int i = 0; i < size; i++) {
      /* code */
      if(strcmp(cmd[i],"|")==0){
        //printf("%d\n", i );
        cmd[i] = NULL;
        start[j] = i+1;
        pipe++;
        j++;
      }

      if(strcmp(cmd[i],";")== 0){
        cmd[i] = NULL;
        start1[j] = i+1;
        chain++;
        j++;
      }

    }
    if( chain >= 1){
      int new = createProcess(cmd,start1,chain);
    }
    else if(pipe >= 1){
      printf("%d\n", pipe );
      int new = createPipe(cmd,start,pipe);

    }
    else{
      int new = createProcess(cmd,start1,chain);
    }

  }
  return 0;
}
