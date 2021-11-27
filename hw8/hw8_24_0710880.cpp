#include<iostream>
using namespace std;

int main(int argc,char *argv[])
{
    
    unsigned long page;
    unsigned long offset;
    unsigned long address;
    
    address = atoi(argv[1]);
    page = address >> 12; // the left most
    offset = address & 0xfff; //12 bit offset
    
    cout << "The address "<<address<<" contains: "<<endl;
    cout << "page number = " << page<<endl;
    cout << "offset = " << offset <<endl;
    
    return 0;
}