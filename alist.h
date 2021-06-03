/*
This file defines our basic class definitions
*/

#ifndef alist_h
#define alist_h
class patient{
	public:
    //CA2
    //Exercise1(i)
    int person;
    int medical_status;
    int registration; //whenever taken from fibheap
    int treat_day;    //outer-loop j, whenever taken from vector
    int treatment;    //type of treatment required
    int address_x;
    int address_y;
    patient(void);



    //CA1
    int profession;
    int age;
    int risk;

    //Updated daily
    double waiting=0;
    double suppose=0;

};
patient::patient(){
	person=-1;
	medical_status=-1;
	registration=-1;
	treat_day=-1;
}
class AList
{
public:
    AList(int size = 20);
//    virtual ~AList();
    patient* &operator[](int index);
    void setitem(int index, patient* value);
    patient* getitem(int index);
    void append(patient* value);
    void remove(int index);
    int maxsize, minsize;
    int numitems;
    patient* *reprarray;
};

AList::AList(int size)
{
    maxsize = size;
    if (size < 20)
        minsize = size;
    else
        minsize = 20;
    numitems = 0;
    reprarray = new patient*[maxsize];
}

//template<class patient*> AList<patient*>::~AList()
//{
//    delete[] reprarray;
//}

#endif /* alist_h */

