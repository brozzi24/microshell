#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char *cmd[] = {
  "<",
  "|",
};

char *readLine(void){
  char *line = NULL;
  ssize_t bufsize = 0;
  getline(&line, &bufsize,stdin);
  line[strlen(line) - 1] = 0;
  return line;
}

char **parseLine(char *line){
  int position = 0;
  char **tokens = malloc(64 * sizeof(char*));
  char *token;
  token = strtok(line, " ");
  while(token != NULL){
    tokens[position] = token;
    position++;
    token = strtok(NULL," ");
  }

  tokens[position] = NULL;
  return tokens;
}
int createPipe(char **arg1, char **arg2){
  printf("here2\n" );

  int pipefd[2];
  int pid;

  pipe(pipefd);
  pid=fork();


  if (pid == 0)
    {
      // child gets here and handles "grep Villanova"

      // replace standard input with input part of pipe

      dup2(pipefd[0], 0);

      // close unused hald of pipe

      close(pipefd[1]);

      // execute grep

      execvp(arg2[0], arg2);
    }
  else
    {
      // parent gets here and handles "cat scores"

      // replace standard output with output part of pipe

      dup2(pipefd[1], 1);

      // close unused unput half of pipe

      close(pipefd[0]);

      // execute cat

      execvp(arg1[0], arg2);
    }

    return 1;
}

void execArgPipe(char** parsed, char** cmdPipe){
  // 0 read end, 1 is write end
  int pipefd[2];
  pid_t p1, p2;

  if(pipe(pipefd) < 0){
    printf("error piping\n" );
    return;
  }

  p1 = fork();
  if(p1 < 0){
    perror("forking pipe");
  }

  if(p1 = 0){
    // child 1
    close(pipefd[0]);
    dup2(pipefd[1], STDOUT_FILENO);
    close(pipefd[1]);

    if(execvp(parsed[0], parsed) < 0){
      perror("exec child pipe");
    }
  }
  else {
    p2 = fork();

    if(p2 < 0) {
      perror("could not fork child 2");
    }

    //child 2 execvp
    if(p2 == 0 ){
      close(pipefd[1]);
      dup2(pipefd[0], STDIN_FILENO);
      close(pipefd[0]);
      if(execvp(cmdPipe[0],cmdPipe) < 0) {
        perror("child 2 exec");
      }
    }
    else{
      // parent wait
      wait(NULL);
      wait(NULL);

    }
  }

}

int createProcess(int count, char **args){
  pid_t pid, wpid;
  int status;
  int found = 0;
  int position = 0;
  char *arg1[256],*arg2[256];


  // check for built ins that cant use execvp
  // check for cd command
  if(strcmp(args[0],"cd")== 0){
    chdir(args[1]);
    return 1;
  }
  // check for exit command
  if(strcmp(args[0], "exit") == 0){
    printf("%s\n", "GOODBYE!" );
    exit(0);
  }



////////////////////////////////////
printf("%ld\n", sizeof(args) );
for (int i = 0; i < 5; i++) {
  /* code */
  if(strcmp(args[i],"|") == 0){
    printf("here\n");
    position = i;
  }
}
printf("%d\n",position );
int cnt = 0;
for (int i = 0; i < 5; i++) {
  /* code */
  if(i < position){
    arg1[i] = args[i];
  }
  if(i > position){
    arg2[cnt] = args[i];
    cnt++;
  }
}

if(position > 0){
  execArgPipe(arg1,arg2);
}


/////////////////////////////////////
  if(count == 0 && position == 0){
      pid = fork();
      if(pid == 0){
        //child
        if(execvp(args[0],args) == -1){
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

  return 1;
}

int checkArg(char **args){
  int count = 0;
  int found = 1;
  char *arg1[256],*arg2[256];
  if(args[0] == NULL){
    return 1;
  }

  return createProcess(count,args);

}

int builtIn(char **args){
  char built_in;

}



int main(){

  char *line;
  char **args;
  int status;
  char cwd[1024];
  while(1){
    printf("%s", "ROZZISHELL:" );
    getcwd(cwd,sizeof(cwd));
    printf("%s", cwd );
    printf("> ");
    line = readLine();
    args = parseLine(line);
    status = checkArg(args);
  }

  return 0;
}
