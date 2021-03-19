#include "tsh.h"
#include <sys/wait.h>
#include<vector>

using namespace std;

void simple_shell::parse_command(char* cmd, char** cmdTokens) {
  
  // convert from cmd massive string to cmd tokens and store there using delimiter etc
  char* token = strtok(cmd," \n");
  int i=0;
  while(token != NULL) {
    cmdTokens[i]=token;
    i=i+1;
    token = strtok(NULL, " \n");
  }
  cmdTokens[i] = NULL;

  // TODO: tokenize the command string into arguments
}

void simple_shell::exec_command(char **argv)
{
  
//   int index = 0;
// // while(argv[index] != NULL){
//   printf(argv[index]);
//   index++;
// }

int pid=fork();
if (pid<0){
  cout<<"fork failed"<< endl;
} else if(pid==0){
  if(execvp(argv[0],argv) < 0){
    cout << "NO" << endl;
    exit(1);
  }
}else{
  waitpid(pid, 0, 0);
}



  // TODO: fork a child process to execute the command.
  // parent process should wait for the child process to complete and reap it
}

bool simple_shell::isQuit(char *cmd){
  return strcmp(cmd, "quit")==0;
  // TODO: check for the command "quit" that terminates the shell
}


