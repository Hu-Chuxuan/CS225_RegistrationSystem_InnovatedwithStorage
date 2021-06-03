#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h> 
#include <time.h>  
#include<vector>
#include <sstream>
// #include "alist.cpp"
// #include "bplus_1.cpp"
#include <iostream>
#include "Fibheap.h"
#include "multiset_c.cpp"
#include "btree.cpp"
#include "cluster.cpp"
using namespace std;

const int SIZE = 8;  //number of patients popped(later added to B-Plus Tree)

//Wrapping up all the three in one class
class Big_Tree{
	public:
	B_plus_tree* T1;
	Btree* T2;
	MultiSet_Chaining* T3;

	Big_Tree(){
		T1=new B_plus_tree;
		T2=new Btree(3);
		T3=new MultiSet_Chaining;
	}

	void INSERT(patient* p){
		T1->add(p);
		T2->add(p);
		T3->insertion(p);
	}
	void REMOVE(int ID){
		patient* p=T1->search(ID);
		if(p==NULL) return;
		T1->_delete(ID);
		T2->deletion(p);
		T3->deletion(p);
	}
	void RETRIEVAL_FROM_SECONDARY(int ID){
		T2->report_btree(ID);
		T3->report_multiset(ID);
	}
	patient* RETRIEVAL(int ID){
		patient* p= T1->search(ID);
		if(p==NULL) return NULL;
		cout<<"The person with ID "<<ID<<" has profession "<<p->profession<<" has age "<<p->age<<" has risk "<<p->risk<<" has waiting time "<< p->treat_day-p->registration<<endl;
		return p;
	}
	void REPORT(){
		T1->print();
		T1->report();
		ofstream ofile;
		ofile.open( "./Reports_monthly.txt" );
		T1->monthlyreport(ofile);
		ofile.close();
	}
};

/*
MAIN TEST FUNCTIONS
*/
int main()
{
    /*
    TEST 1: REGISTRATION -> FIBHEAP -> B-PLUS TREES -> REPORTS
    */
    Big_Tree* our_tree=new Big_Tree;
    FibHeap* F1=new FibHeap;
	FibHeap* F2=new FibHeap;
	FibHeap* F3=new FibHeap;

	/*
	INPUTS: WE GENERATE RANDOM NUMBERS IN VARIOUS FILES AS FIELDS OF PATIENTS
	*/
	int cnt1=0;
	int last1=0,last2=0,last3=0; 
	int ctn1=1,ctn2=1,ctn3=1;
	int cnt2=0;
	int cnt3=0;
	srand( (unsigned)time( NULL ) );
	int a[30]; 
	vector<patient*>* v1=new  vector<patient*>;
        vector<patient*>* v2=new  vector<patient*>;
        vector<patient*>* v3=new  vector<patient*>;

	for(int j=0;j<7;j++){
		 cnt1+=10;
		 cnt2+=10;
		 cnt3+=10; 
		ifstream inFile1("./shuju/1.csv", ios::in);
		string lineStr; 
		vector<patient*> strArray1;
		for(int k=0;k<=last1;k++){
			getline(inFile1, lineStr);
		}
		for(int i=0;i<cnt1-last1;i++){	
			if(ctn1==0) break;
			if(getline(inFile1, lineStr)){
				// cout << lineStr << endl;
				stringstream ss(lineStr);
				string str;
				patient*p=new patient;
				getline(ss, str, ',');
				p->person=atoi(str.c_str());
				getline(ss, str, ',');
				p->medical_status=atoi(str.c_str());
				getline(ss, str, ',');
				p->registration=j;
				getline(ss, str, ',');
				p->treat_day=atoi(str.c_str());
				getline(ss, str, ',');
				p->treatment=atoi(str.c_str());
				getline(ss, str, ',');
				p->address_x=atoi(str.c_str());
				getline(ss, str, ',');
				p->address_y=atoi(str.c_str());
				getline(ss, str, ',');
				p->profession=atoi(str.c_str());
				getline(ss, str, ',');
				p->age=atoi(str.c_str());
				getline(ss, str, ',');
				p->risk=atoi(str.c_str());
				getline(ss, str, ',');
				p->waiting=atoi(str.c_str());
				getline(ss, str, ',');
				p->suppose=atoi(str.c_str());
				strArray1.push_back(p);
				}else ctn1=0;
	}
	ifstream inFile2("./shuju/2.csv", ios::in);
	string lineStr2;
	vector<patient*> strArray2;
	
	for(int k=0;k<=last2;k++){
		getline(inFile2, lineStr2);
	}	
	for(int i=0;i<cnt2-last2;i++)
	{if(ctn2==0){
		break;
	}
	if(getline(inFile2, lineStr2)){
		
		stringstream ss(lineStr2);
		string str;

		patient*p=new patient;
		getline(ss, str, ',');
		p->person=atoi(str.c_str());
		getline(ss, str, ',');
		p->medical_status=atoi(str.c_str());
		getline(ss, str, ',');
		p->registration=j;
		getline(ss, str, ',');
		p->treat_day=atoi(str.c_str());
		getline(ss, str, ',');
		p->treatment=atoi(str.c_str());
		getline(ss, str, ',');
		p->address_x=atoi(str.c_str());
		getline(ss, str, ',');
		p->address_y=atoi(str.c_str());
		getline(ss, str, ',');
		p->profession=atoi(str.c_str());
		getline(ss, str, ',');
		p->age=atoi(str.c_str());
		getline(ss, str, ',');
		p->risk=atoi(str.c_str());
		getline(ss, str, ',');
		p->waiting=atoi(str.c_str());
		getline(ss, str, ',');
		p->suppose=atoi(str.c_str());
		strArray2.push_back(p);
	}else{
		cnt2=0;
	}
	}

	ifstream inFile3("./shuju/3.csv", ios::in);
	string lineStr3;
	vector<patient*> strArray3;
	for(int k=0;k<=last3;k++){
		getline(inFile3, lineStr3);
	}
	for(int i=0;i<cnt3-last3;i++)
	{	if(ctn3==0){
		break;
	}
	if(getline(inFile3, lineStr3)){
		
		stringstream ss(lineStr3);
		string str;

		patient*p=new patient;
		getline(ss, str, ',');
		p->person=atoi(str.c_str());
		getline(ss, str, ',');
		p->medical_status=atoi(str.c_str());
		getline(ss, str, ',');
		p->registration=j;
		getline(ss, str, ',');
		p->treat_day=atoi(str.c_str());
		getline(ss, str, ',');
		p->treatment=atoi(str.c_str());
		getline(ss, str, ',');
		p->address_x=atoi(str.c_str());
		getline(ss, str, ',');
		p->address_y=atoi(str.c_str());
		getline(ss, str, ',');
		p->profession=atoi(str.c_str());
		getline(ss, str, ',');
		p->age=atoi(str.c_str());
		getline(ss, str, ',');
		p->risk=atoi(str.c_str());
		getline(ss, str, ',');
		p->waiting=atoi(str.c_str());
		getline(ss, str, ',');
		p->suppose=atoi(str.c_str());
		strArray3.push_back(p);
	}else{
		ctn3=0;
	}} 

	
	
	last1=cnt1;
	last2=cnt2;
	last3=cnt3;
    
	


	//INSERTION INTO FIBHEAP
	for(int i=0;i<strArray1.size();i++){
		patient* t=strArray1[i];
		FibNode* f=new FibNode;
		f->person=t->person;
		f->medical_status=t->medical_status;
		f->registration=t->registration;
		f->treatment=t->treatment;
		f->address_x=t->address_x;
		f->address_y=t->address_y;
		f->age=t->age;
		f->profession=t->profession;
		f->risk=t->risk;
		f->suppose=t->suppose;
		f->waiting=t->waiting;
		f->key=f->priority();
		F1->insert(f);
	}
	for(int i=0;i<strArray2.size();i++){
		patient* t=strArray2[i];
		FibNode* f=new FibNode;
		f->person=t->person;
		f->medical_status=t->medical_status;
		f->registration=t->registration;
		f->treatment=t->treatment;
		f->address_x=t->address_x;
		f->address_y=t->address_y;
		f->age=t->age;
		f->profession=t->profession;
		f->risk=t->risk;
		f->suppose=t->suppose;
		f->waiting=t->waiting;
		f->key=f->priority();
		F2->insert(f);
	}
	for(int i=0;i<strArray3.size();i++){
		patient* t=strArray3[i];
		FibNode* f=new FibNode;
		f->person=t->person;
		f->medical_status=t->medical_status;
		f->registration=t->registration;
		f->treatment=t->treatment;
		f->address_x=t->address_x;
		f->address_y=t->address_y;
		f->age=t->age;
		f->profession=t->profession;
		f->risk=t->risk;
		f->suppose=t->suppose;
		f->waiting=t->waiting;
		f->key=f->priority();
		F3->insert(f);
	}
	//TREATED: POP INTO B-PLUS TREE
	for(int i=0;i<SIZE/3;i++){
		patient* p1=F1->removeMin();
		patient* p2=F2->removeMin();
		patient* p3=F3->removeMin();
		p1->treat_day=j;
		p2->treat_day=j;
		p3->treat_day=j;


		our_tree->INSERT(p1);

		our_tree->INSERT(p2);
		our_tree->INSERT(p3);
	}

	/*
	TEST FOR EX4: CLUSTER ANALYSIS
	*/
	if(j==1){
		
        *v1=strArray1;
	    *v2=strArray2;
	    *v3=strArray3;
	}
	if(j==2){
		cout<<"OUR CLUSTER ANALYSIS FOR EX4 FROM DAY 1 IS AS FOLLOW:"<<endl;
		B_plus_tree* Mytree= new B_plus_tree;
	    Mytree=our_tree->T1;
		int c;
		int n;
		cout <<"PLEASE TYPE IN n(RECOMMANDED 7 TO MAKE THE RESULT EXPLICIT)"<<endl;
		cin>>n;
		cout <<"PLEASE TYPE IN THE TREATMENT TYPE"<<endl;
		cin>>c;
		int percent_result=find_percent(c,n,v1,v2,v3,Mytree);
	    cout<<" The number of clusters that reaches percent result is "<<percent_result <<endl; //can be adjusted
	}
	
	    cout<<"DAY "<<j+1<<" ENDS"<<endl;
		cout<<"PRESS ENTER TO GO TO NEXT DAY"<<endl;
		getchar();
	}
	
	//REPORT GENERATING & B_PLUS_TREE DATA DISPLAY
	our_tree->REPORT();

    int c;
	cout <<"REFERRING TO OUR OUTPUTS, PLEASE ENTER THE PERSON'S ID WHOSE INFORMATION YOU WANT TO RETRIEVE"<<endl;
	cin>>c;
	our_tree->RETRIEVAL(c);


	/*
    TEST 2: GET ALL PATIENTS OF A SPECIFIC MEDICAL STATUS BY B-TREE & HASHSET
    */
   int check;      //can be adjusted
   cout <<"PLEASE ENTER THE MEDICAL STATE YOU WANT TO CHECK FOR (RANGING FROM 0 TO 29)"<<endl;
   cin>>check;
   our_tree->RETRIEVAL_FROM_SECONDARY(check);
   /*
   TEST 3: DELETION & RETRIEVAL FOR B-PLUS TREE
   */
   //At this stage, please pick IDs from the printed output as check
   cout <<"REFERRING TO OUR OUTPUTS, PLEASE ENTER THE PERSON'S ID YOU WANT TO REMOVE"<<endl;
   cin>>check;
   our_tree->REMOVE(check);
   cout<<"AFTER REMOVAL WE HAVE"<<endl;
   our_tree->REPORT();   

   /*
   THE END
   */
   cout<<"THE END! THANKS FOR USING"<<endl;
   return 0;
}


