#include "fs.h"
#include<iostream>

int main(int argc, char *argv[]){

  if (argc == 1 ){
    fprintf(stderr,"usage: %s <FileName> \n",argv[0]);
    exit(0);
  }
  myFileSystem f ((char *)"disk0"); // create disk object
  char line[100]; //store line from textfile
  char* cmd[3]; //store each part of command
  int cmd_cnt; //count for command
  char* input; //temp variable to get each part from command


  char buff[1024];
  //fill dummy buffer with 1's
  for (int i = 0; i < 1024; i++){
    buff[i] = '2' ;
    if(i==0){
      buff[i]='1';
    }
    if(i==1023){
      buff[i]='9';
    }


  }
  

  ifstream testfile (argv[1]);
  if (testfile.is_open()){

    //get each line in textfile
    while(testfile.getline(line,100)){
      //split line into parts
      cmd_cnt = 0;
      input = strtok(line," ");
      while(input != NULL){
        cmd[cmd_cnt] = input;
        cmd_cnt++;
        input = strtok(NULL," ");
      }

      //process each part from cmd
      switch (*cmd[0]){
        case 'C':
        {
          f.create_file((char*)cmd[1],atoi(cmd[2]));
          break;
        }
        case 'D':
        {
          f.delete_file((char*)cmd[1]);
          break;
        }
        case 'L':
        {
          f.ls();
          break;
        }
        case 'R':
        {
          // for (int i=0;i<1024;i++){
          //   buff[i]='0'; 
          // }
          int a= f.read((char*)cmd[1],atoi(cmd[2]),buff);
          printf("read out buffer... %d  \n", a);
          string s= "";
          for (int i=0;i<1024;i++){
            s=s+ buff[i]; 
          }
          cout<<s<<"\n";
          break;
        }
        case 'W':
        {
          // f.read((char*)cmd[1],atoi(cmd[2]),buff);
          // for (int i=0;i<1024;i++){
          //   buff[i]='0'; 
          // }
          int b= f.write((char*)cmd[1],atoi(cmd[2]),buff);
          printf("write buffer...  %d \n",b);
          string h= ""; 
          for (int i=0;i<1024;i++){
            h=h+ buff[i]; 
          }
          cout<<h<<"\n";


          break;
        }
        default:
        {
          printf("%s\n",cmd[0]);
          break;
        }
      }//end switch
    }
  }
  f.close_disk();
  return 0;
}
