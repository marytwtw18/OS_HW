#define _POSIX_SOURCE

#include<iostream>
#include<fstream>
#include<cstring>

#include<stdio.h>
#include<stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include<unistd.h>
#include<sys/ptrace.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/reg.h>

using namespace std;

int main(int argc,char **argv)
{
	pid_t child_pid;
	int p_status;
	long syscall;
  
  int input_FD, output_FD;
  ssize_t bytes_Read, bytes_Write;  
  int bufferSize = 32768;           
  char buffer[bufferSize];
  char output_file[300],input_file[300];

	child_pid = fork();

	if(child_pid == 0) // child process
	{
		ptrace(PTRACE_TRACEME, child_pid, NULL, NULL);
		kill(getpid(), SIGSTOP);

		//------- Implement your file copy code--------//
   do{
       cout << "Please input the source file name:"<<endl;
       cin >> input_file;
   
       //Open the input file, if file doesn't exist, abort 
       input_FD = open (input_file, O_RDONLY);  //read only 
       if (input_FD == -1) {
          perror ("Fail to open file");
          }
      }while(input_FD == -1);    //if error,reread until success
   do{
     cout << "Please input the destination file name:"<<endl;
     cin >> output_file;
   
     //Create output file only if it doesn't exist
      output_FD = open(output_file, O_WRONLY | O_CREAT, 0644);
      
      if(output_FD == -1){
          perror ("Fail to open file");
        }
      }while(output_FD == -1);
      
    //Read from input file and write to output file until read fails
    while((bytes_Read = read (input_FD, &buffer, bufferSize)) > 0){
        bytes_Write = write (output_FD, &buffer, (ssize_t) bytes_Read);
        
        if(bytes_Write != bytes_Read){
            perror("Error while copying!\n");
        }else {
        
          //Write completion message to screen
          cout<<output_file<<" has generated"<<endl;
		    }
    }   
    //------- ------------------------------------------//
		exit(0);
	}
	else if(child_pid > 0) // parent process
		while(waitpid(child_pid, &p_status, 0))
			if(WIFEXITED(p_status))
				break;
			else
			{
				syscall = ptrace(PTRACE_PEEKUSER, child_pid, sizeof(long)*ORIG_RAX, NULL);
				printf("system call: %ld\n", syscall);
				ptrace(PTRACE_SYSCALL, child_pid, NULL, NULL);
			}
	else
		printf("fork error\n");

  close (input_FD);  //3
  close (output_FD);  //3
  
	return 0;
}
