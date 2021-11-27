#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<wait.h>
#include<errno.h>
#include<unistd.h>
#include<sys/shm.h>
#include <sys/types.h>
#include <sys/ipc.h>

using namespace std;

#define MAX_SEQUENCE 10

typedef struct{
    long fib_sequence[MAX_SEQUENCE];
    int sequence_size;
} shared_data;

int main(int argc,char *argv[])
{
    int arg = atoi(argv[1]);   
    int result = 1;
    int num_two = 1;
    int num_plus_one = 1;
    int num = 1;
    pid_t parent_pid,chlid_pid;
    
    int shm_id; //build share memory id
    shared_data *shared_memory;
  
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
        //share memory get
        if((shm_id = shmget (0x00, sizeof (int) , 0666 | IPC_CREAT)) == -1)
        {
            perror("shmget failed");
            exit(1); 
        }
        
        shared_memory =  (shared_data *) shmat (shm_id, NULL, 0);
        if(shared_memory ==(void*) -1)
        {
            perror("shmat failed");
            exit(1); 
        }       
       
        parent_pid = fork();
        
        if(parent_pid < 0)  //error occurred
        {
            perror("Fork failed.\n");
            shmctl(shm_id, IPC_RMID, NULL) ;
            exit(-1);
        }
        else if(parent_pid == 0)  //child process
        {
            chlid_pid = getpid();
            //cout << "Child process-Start,pid = " << chlid_pid<<endl;        
            //print fib num 
            shared_memory-> sequence_size = arg;
            //cout << fib_0 << " " << fib_1;
            shared_memory->fib_sequence[0] = num;
            //shared_memory->fib_sequence[1] = fib_1;
            for(int i=2;i <= arg;++i)
            {
                num_two = 1;
                num = 1;
                num_plus_one = 1;
                for(int j = 1;j <=2*i ; ++j)
                {
                    num_two *=j;
                    //cout << " "<< fib_n;    
                }
                for(int j = 1;j <= i ; ++j)
                {
                    num *= j;
                    //cout << " "<< fib_n;    
                }
                for(int j = 1;j <= i+1 ; ++j)
                {
                    num_plus_one *= j;
                }
                //cout << num_two<<endl;
                result = ((num_two/num)/num_plus_one);
                shared_memory->fib_sequence[i-1] = result;
            }
            //cout << "Child process-end,pid = " << chlid_pid<<endl;
        }
        else
        {
            chlid_pid = getpid();
            //cout << "Parent process-start, pid = " << chlid_pid<<endl;
            //cout << "create a shared-memory segment,segment_id = "<<shm_id<<endl;
           
            wait(NULL);
            for(int i = 0;i < shared_memory-> sequence_size;++i)
            {
                cout << shared_memory->fib_sequence[i] <<" ";
            }
            cout << endl;
            cout << "Discussion:" << endl;
            cout << "  1.time different: shared memory time higher than process copy "<<endl;
            cout << "  2.memory: shared memory  memory  use less than process copy" << endl;
            
            //cout << "Parent process-end, pid = " << chlid_pid<<endl;
        }
    }
    shmdt(shared_memory);
    return 0;
}

