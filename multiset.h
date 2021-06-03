#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <utility>
#include "alist.cpp"
#ifndef multiset_h
#define multiset_h

/*
THIS FILE DEALS WITH SECONDARY KEYS IMPLEMENTATIONS BY A HASH-SET
*/



class MultiSet_Chaining{
   public:
    void report_multiset(int k);
    MultiSet_Chaining();
    //    virtual ~hashset();
       
        // The operation add inserts an element into the hashtable, remove deletes an
        // element and member checks, if the element is in the hashtable
            void insertion(patient* p);
            AList* retrevial(int k);
            void deletion(patient* p);
            int size() {
                return maxsize;
            }
            // The function display is only used for testing purposes. It shows the size of
            // the hashtable, the number of elements, and displays all entries
                void display(void);
            private:
                int numitems;           // contains the number of elements in the hashtable
                AList a;
                AList **reprarray;


                /* reprarray defines a pointer to (an array of) pointers to values of type T. We will use a null pointer, if an entry is undefined, and a fixed pointer as placeholder for free entries (after remove) */

                    int maxsize;            // defines the size of the hashtable
                   
                    // rehash is an internal function to enlarge or shrink the hashtable,
                    // if the fill factors 0.75 or 0.25 are exceeded or undershot, resp.
};
#endif
