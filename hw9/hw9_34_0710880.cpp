#include <string>
#include <iostream>
#include <ctime>
#include <cmath>
#include <bits/stdc++.h> 
using namespace std;

 
// Function to find page faults using FIFO 
int pageFault_FIFO (int pages[], int n, int capacity) 
{
    unordered_set <int> set; //current page
    queue <int> indexes; //fifo stl
    int page_faults = 0;

    for(int i = 0; i < n; i++)
    {
        if(set.size () < capacity) { 
                
            //cout << i << endl;
            set.insert (pages[i]);
            ++page_faults;
            indexes.push (pages[i]);
            
        }
        else {
            if(set.find (pages[i]) == set.end ()) { //can not find the item in the list
                //cout << i << endl;
                int val = indexes.front ();  //next to pop
                indexes.pop ();
                set.erase (val);//delete the item
                set.insert (pages[i]);
                indexes.push (pages[i]);
                ++page_faults;
            }
        } 
    } 
    return page_faults;
}

int pageFault_LRU (int pages[], int n, int capacity) 
{
    unordered_set <int> set;
    //queue <int> indexes; //fifo stl
    
    //to store lru index of page
    unordered_map<int,int> indexes;
    
    int page_faults = 0;

    for(int i = 0; i < n; i++)
    {
        if(set.size () < capacity) { 
                
            //cout << i << endl;
            set.insert (pages[i]);
            ++page_faults;
            indexes[pages[i]] = i;
            
        }
        else {
            if(set.find (pages[i]) == set.end ()) { //can not find the item in the list
                //cout << i << endl;
                int lru = INT_MAX;
                int val;  //next to pop
                
                //find the lru pages in the set
                for (auto it=set.begin();it!=set.end();++it)
                {
                    //find the smallest index
                    if(indexes[*it] < lru)
                    {
                        lru = indexes[*it];
                        val = *it;
                    }
                }
            
                set.erase (val);//delete the item
                set.insert (pages[i]);
                ++page_faults;
            }
            indexes[pages[i]] = i;
        } 
    } 
    return page_faults;
}

// Driver code 
int main(int argc,char *argv[]) {
    int pages[10];
    int capacity = atoi(argv[1]);
    
    time_t t;
    srand((unsigned) time(&t));
    
    for(int i=0;i<10;++i)
    {
        pages[i] = rand()%6;
    }
    
    cout << "Referece string: ";
    for(int i = 0; i < 10; i++) {
        cout << pages[i];  cout << " ";
    }
    cout << endl;
    cout << "page faults under FIFO algorithm: ";
    cout << pageFault_FIFO (pages, 10, capacity) << endl;
    
    cout << "page faults under LRU algorithm: ";
    cout <<  pageFault_LRU (pages, 10, capacity) << endl;
    return 0;
}