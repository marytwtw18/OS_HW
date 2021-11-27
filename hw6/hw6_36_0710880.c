#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

void * barber_function(void *idp);
void * customer_function(void *idp);
void service_customer();
void * make_customer_function();

// Mutex
pthread_mutex_t srvCust;

// Semaphores 
sem_t barber_ready; 
sem_t customer_ready;
sem_t modifySeats;

// Inputs 
int number_of_chair;
int total_custs;
int available_seats;
int no_served_custs = 0;
int served_custs = 0;

int item = 0;
int customerWait[30];    

void * barber_function(void *idp)
{    
    int counter = 0;
    
    while(1)
    {
        sem_wait(&customer_ready);
        sem_wait(&modifySeats);
            
        //barber is servicing a client
        available_seats++;
        printf("Barber services a client,available seats: %d\n",available_seats);

        sem_post(&modifySeats);
        sem_post(&barber_ready);        

        pthread_mutex_lock(&srvCust);
        usleep(200000);   //cut hair for 200ms
        pthread_mutex_unlock(&srvCust);
        
        served_custs++;
       
        counter++; 
        if(counter == (total_custs - no_served_custs))  break;
    }
    pthread_exit(NULL);    
}

void * customer_function(void *idp)
{  
    sem_wait(&modifySeats); 

    //if there is available seat
    if(available_seats > 0)
    {
        available_seats--;
        printf("Client takes a seat,available seats: %d\n", available_seats);
           
        sem_post(&customer_ready);
        sem_post(&modifySeats);         
        sem_wait(&barber_ready);      
    }
    else
    {
        sem_post(&modifySeats);
        no_served_custs++;
        //printf("A Customer left.\n");
    }
    pthread_exit(NULL);
}

void * make_customer_function(){
    int tmp;   
    int counter = 0;

    while(counter < total_custs)
    {
        pthread_t customer_thread;
        tmp = pthread_create(&customer_thread, NULL, (void *)customer_function, NULL);  
        if(tmp)  printf("Failed to create thread.");
        
        //Sleep forcustomerWait[count]ms before creating another customer
        customerWait[counter] = customerWait[counter]*1000;
        usleep(customerWait[counter]);
        //printf("interval time %d ms\n",customerWait[count]/1000);  
        ++counter;
    }
}

int main(){

    pthread_t barber_1;
    pthread_t customer_maker;
    int tmp;
    
    FILE * pFile;
    char buffer[100];
    int store_array[100];   

    pthread_mutex_init(&srvCust, NULL);
    sem_init(&customer_ready, 0, 0);
    sem_init(&barber_ready, 0, 0);
    sem_init(&modifySeats, 0, 1);
     
    pFile = fopen ("input.txt", "r");
    if (pFile == NULL)
    {
        perror("file doesn't exist\n");
    }
    while (fgets(buffer, 100, pFile))
    {
        //printf("%s", buffer);
        store_array[item] = atoi(buffer);
        ++item;
    }
    
    number_of_chair = store_array[0];
    total_custs = store_array[1];
    
    for(int i = 2;i<item;++i)
    {
        customerWait[i-2] = store_array[i];
        //printf("%d\n", customerWait[i-1]);
    }
    
    available_seats = number_of_chair; 
    
    // Create barber thread
    tmp = pthread_create(&barber_1, NULL, (void *)barber_function, NULL);  
    if(tmp)  printf("Failed to create thread."); 
    
    // Create customer_maker thread
    tmp = pthread_create(&customer_maker, NULL, (void *)make_customer_function, NULL); 
    if(tmp)  printf("Failed to create thread."); 
     
   // Wait for threads to finish 
    pthread_join(barber_1, NULL);
    pthread_join(customer_maker, NULL);
    
    printf("------------------------------------------------\n");
    printf("Serviced clients:%d\n",served_custs);
    printf("Unserviced clients:%d\n", no_served_custs);    	
}