/*
This File Deals with Basic Array Operations
*/
#ifndef alist_func_h
#define alist_func_h

#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include "alist.h"
using namespace std;

patient* &AList::operator[](int index)
{
    return reprarray[index - 1];
}

void AList::setitem(int index, patient* value)
{
    if ((index >= 0) && (index < numitems))
    {
        reprarray[index] = value;
        return;
    }
    else
        cout << "Index error: index out or range\n";
}

patient* AList::getitem(int index)
{
    if ((index >= 0) && (index < numitems))
    {
        return reprarray[index];
    }
    else
    { 
        cout << "Index error: index out or range\n";
    }
}

 void AList::append(patient* a)
{
    reprarray[numitems] = a;//////
    ++numitems;
    return;
}

void AList::remove(int index)
{
    if (index <= numitems)
    {
        for (int j = index; j < numitems; ++j)
        {
            reprarray[j-1] = reprarray[j];
        }
        --numitems;
        return;
    }
    else
        cout << "Index error: list index out of range\n";
}


#endif
