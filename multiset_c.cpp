#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include "multiset.h"

/*
HASHING WITH CHAINING OPERATIONS
*/

using namespace std;

 MultiSet_Chaining::MultiSet_Chaining()
{
    maxsize = 100;
    numitems = 0; // initially, the hashtable is empty
    reprarray = new AList* [100];  // allocate space for the array of pointers
    // the for loop initialises all table entries to be undefined
    for (int i = 0; i < maxsize; i++)
    {  
  
     reprarray[i] = 0;     
        
    }
    
}
 void MultiSet_Chaining::display(void) // only for test purposes
{
    cout << "The size of the hashtable is: " << maxsize << "\n";
    cout << "The number of elements in the hashtable is: " << numitems << "\n";
    for (int i = 0; i < maxsize; ++i)
    {
        if (reprarray[i] == 0)
            cout << "The entry " << i + 1 << " is undefined.\n";
        else{
        	if((*reprarray[i]).numitems==0){cout << "The entry " << i + 1 << " is undefined.\n";
			}
		for (int u = 1; u <= (*reprarray[i]).numitems; u++)
		{cout << "The entry " << i + 1 << " position    " << u << "   is  "<<(*reprarray[i]).getitem(u-1)->person<<".\n";
            
			}
			}
			
			
    }
    return;
}



 void MultiSet_Chaining::insertion(patient* p) {

    int index;
    index = p->medical_status % maxsize; // First determine the position index in the hash table, where the new value is stored, if free.
    
     // store item in the hashtable
    //(*reprarray[index - 1]).append(item);
   
    if(reprarray[index]==0) {  
	AList* pt=new AList;
	reprarray[index]=pt;}
   (*reprarray[index]).append(p);

   
}

 void MultiSet_Chaining::deletion(patient* p) {
	
    
    int index;
    int i = 1;
    index = p->medical_status% maxsize; // First determine the position index in the hash table, where the new value is stored, if free.
    if(reprarray[index]==0){
	return ;}
    while ((*reprarray[index]).getitem(i-1) != p) {
        if (i > (*reprarray[index]).numitems) {
            return;
        }
        i++;
    }
    (*reprarray[index]).remove(i);
    return;
}

   AList* MultiSet_Chaining::retrevial(int k) {
        // use the predefined hashfunction to get "key" values
        int index;
        index =k % maxsize; // First determine the position index in the hash table, where the new value is stored, if free.
        AList* pt= new AList;
        pt=reprarray[index];
        return pt;
        
    }


void MultiSet_Chaining::report_multiset(int k){
    cout<<"These patients in the HashSet has secondary key "<<k<<":"<<endl;
	AList* pt=new AList;
	pt=retrevial(k);
	for(int i=0;i<pt->numitems;i++){
		cout<<"the "<< i+1 <<"person has ID "<< pt->getitem(i)->person<<endl;
	}
}
    
  