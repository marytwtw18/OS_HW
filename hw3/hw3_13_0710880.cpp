#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<wait.h>
#include<errno.h>
#include<unistd.h>

using namespace std;

int main(int argc,char *argv[])
{
    int arg = atoi(argv[1]);
    
    int fib_0 = 0,fib_1 = 1,fib_n;
    pid_t parent_pid,chlid_pid;
    
    //error checking
    if(argc != 2)
    {
        cout<<"Warning:You have to provide one argument for the sequence only."<<endl;
    }
    else if(arg < 1)
    {
        cout<<"Warning:You have to provide a positive number for the sequence."<<endl;
    }
    else
    {
        parent_pid = fork();
        
        if(parent_pid < 0)  //error occurred
        {
            perror("Fork failed.\n");
            exit(-1);
        }
        else if(parent_pid == 0)  //child process
        {
            chlid_pid = getpid();
            cout << "Child process-Start,pid = " << chlid_pid<<endl;
            cout << fib_0 << " " << fib_1;
            for(int i = 1;i < (arg - 1); ++i)
            {
                fib_n = fib_0 + fib_1;
                fib_0 = fib_1;
                fib_1 = fib_n;
                cout << " "<< fib_n;
            }
            cout << "\nChild process-end,pid = " << chlid_pid<<endl;
        }
        else
        {
            chlid_pid = getpid();
            cout << "Parent process-start, pid = " << chlid_pid<<endl;
            wait(NULL);
            cout << "Parent process-end, pid = " << chlid_pid<<endl;
        }
    }
    return 0;
}
