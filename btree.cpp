/*
THIS FILE DEALS WITH SECONDARY KEYS IMPLEMENTATIONS BY A B-TREE
*/

#include <iostream>
#include <stdio.h>
#include "alist.cpp"
using namespace std;
using std::pair;
class BtreeNode
{
	
	pair<int,AList*>* *a;
	BtreeNode **C;
	int n;
	int t;
	bool leaf;
public:
	BtreeNode(int t,bool leaf)
    {
		this->t = t;
		this->leaf = leaf;
		a = new pair<int,AList*>* [2*t-1];
		C = new BtreeNode*[2*t];
		n = 0;
    }
	int findId(pair<int,AList*>* k);
	void remove(pair<int,AList*>*  k);
	void removeFromLeaf(int id); 
	void removeFromNonLeaf(int id);
	void merge(int id);
	void fill(int id);
	void traverse();
	int findSucc(int id);
	int findPrec(int id);
	BtreeNode *search(pair<int,AList*>*  k);
	void insertNonFull(pair<int,AList*>*  k);
	void splitChild(int i,BtreeNode *y);
	void borrowFromPrev(int id);
    void borrowFromSucc(int id);
	void find_pr(patient* p,AList* *py);
	void find_a(patient* p,pair<int,AList*>* *ta);
	void find_ms(int k,AList* *py);
friend class Btree;
};

class Btree
{
	BtreeNode* root;
	int t;
public:
	Btree(int t)
    {
		root = new BtreeNode(t,true);
		this->t = t;
    }
    
	void insert(pair<int,AList*>* k);
	void remove(pair<int,AList*>* k);
    int find_max();
    int find_min();
    int find_ParentP(BtreeNode *b,pair<int,AList*>* k);
	int find_ParentS(BtreeNode *b,pair<int,AList*>*  k);
	void report_btree(int k);
    //new code added here 
//	int find_s(int k){//find successor key in the bplustree
//    	if(root==NULL) return -999;//-999 means no successor
//    	if(k==find_max()) return -999;//if k is already the max, there's no successor
//    	else{
//    		BtreeNode *kk;
//    		kk=root->search(k);//find the node containing k;
//    		int id;
//    		for(int i=0;i<kk->n;i++){//find the index of k in the node
//    			if(kk->a[i]==k){
//    				id=i;
//    				break;
//				}
//			}
//			int a;
//			a=kk->findSucc(id);//the case where we only need to go down to children
//			if(a!=-999)
//    		return a; 
//    		else{
//    		return this->find_ParentS(root,k);// the case where we need to go up to parents
//		}}
//		
//	}
//    int find_p(int k){//find predecessor one of the given key
//    	if(root==NULL) return -999;
//    	if(k==find_min()) return -999;//if the key is already the min, there's no pre
//    	else{// tbe same process as find_s
//    		BtreeNode *kk;
//    		kk=root->search(k);// ����һ���ϵ�key 
//    		int id;
//    		for(int i=0;i<kk->n;i++){
//    			if(kk->a[i]==k){
//    				id=i;
//    				break;
//				}
//			}
//			int a;
//			a=kk->findPrec(id);
//			if(a!=-999)
//    		return a; 
//    		else{
//    			
//    		return this->find_ParentP(root,k);	
//    		
//    	}
//		}
//	
//	}
	//end
	void traverse()
	{
		if(root == NULL) return;
		else
			root->traverse();
	}
	BtreeNode *search(pair<int,AList*>* k)
	{
		return (root==NULL) ? NULL : root->search(k);
	}

	void add(patient* p); 
	void deletion(patient* p); 
	AList* retrieval(int k ){
		//Alist* t=new Alist;
		AList* *pt= new AList*;
		*pt=NULL;               //maybe problem
		root->find_ms(k,pt);
		if(*pt==NULL) {
			cout<<"no person has this medical status"<<endl;
		}
		return *pt;
	}

	
	
};

void Btree::add(patient* p){
	AList* *pt=new AList*;
	*pt=NULL;
	root->find_pr(p,pt);
	if(*pt!=NULL){ 
	// for the key of patient p has been already in the B tree
		(*pt)->append(p);
		
	}
	else{
		AList* ptt=new AList;
		
		pair<int,AList*>* t= new pair<int,AList*>;
		*t=pair<int,AList*> (p->medical_status,ptt);
		pair<int,AList*>** pa= new pair<int,AList*>*;
		*pa=t;
		 // create a new pair
		insert(*pa);
		(*ptt).append(p);//*pt ��һ��AList* 		
	}
	
}
void BtreeNode ::find_pr(patient* p,AList* *py) //use it as root.find(); 
{
  
	int i;
	
	for(i=0;i<n;i++)
	{
		if(leaf == false)
			C[i]->find_pr(p,py);
		
	    int medical_key=a[i]->first;
			if(medical_key==p->medical_status)
			{			
				*py=a[i]->second;
			} 
		}
	 
		
	if(leaf == false)
      	C[i]->find_pr(p,py);
}
void BtreeNode ::find_ms(int k,AList* *py) //use it as root.find(); 
{
  
	int i;
	
	for(i=0;i<n;i++)
	{
		if(leaf == false)
			C[i]->find_ms(k,py);
		
	    int medical_key=a[i]->first;
			if(medical_key==k)
			{			
				*py=a[i]->second;
			} 
		}
	 
		
	if(leaf == false)
      	C[i]->find_ms(k,py);
}
void BtreeNode ::find_a(patient* p,pair<int,AList*>* *ta) //use it as root.find(); 
{   
	int i;
	for(i=0;i<n;i++)
	{
		if(leaf == false)
			C[i]->find_a(p,ta);
		
	    int medical_key=a[i]->first;
			if(medical_key==p->medical_status)
			{			
				*ta=a[i];
			} 
		}
	 
		
	if(leaf == false)
      	C[i]->find_a(p,ta);
}




void Btree :: deletion(patient* p){
	AList* *pt=new AList*;
	*pt=NULL;
	root->find_pr(p,pt);// change the pt if it has a key in btree
	if(*pt==NULL){
		cout<<"no such person"<<endl;
		return ;
	} 
    for(int u=0;u<(*pt)->numitems;u++){
		
    	if ((*pt)->getitem(u)==p)
    	{
		(*pt)->remove(u+1);
    	break;
		}
	}
	if((*pt)->numitems==0){
		pair<int,AList*>* t= new pair<int,AList*>;
		*t=pair<int,AList*> (p->medical_status,*pt);
		pair<int,AList*>** pa= new pair<int,AList*>*;
		*pa=t;
		 // create a new pair
		
		remove(*pa);  //pair*  (*pa) is pair*
	}
	
	
}
int BtreeNode :: findId(pair<int,AList*>* k)
{
	int i ;
	for(i=0;i<n && a[i]->first < k->first ;i++);
	return i;

}
void BtreeNode :: remove(pair<int,AList*>* k)
{   
	int id = findId(k);
	if(id < n && a[id]->first == k->first)
	{
		if(leaf)
			removeFromLeaf(id);
		else
			removeFromNonLeaf(id);
	}
	else
	{
		if(leaf)
		{
		cout<<"Element does not exist"<<endl;
		return;
		}
		else
		{
			bool flag;
			if(n == id)
				flag = true;
			else
				flag = false;

			if(C[id]->t < t)
				fill(id);

			if(flag && id > n)
				C[id-1]->remove(k);
			else
				C[id]->remove(k);
		}


	}


}

void BtreeNode::removeFromLeaf(int id)
{
	for(int i = id+1;i < n;i++)
	{
		a[i-1] = a[i];
	}
	n--;
}

void BtreeNode :: removeFromNonLeaf(int id)
{
	if(C[id]->t >= t)
	{
		a[id]->first=findPrec(id);
		C[id]->remove(a[id]);
	}
	else if(C[id+1]->t >= t)
	{
		a[id]->first=findSucc(id);
		C[id+1]->remove(a[id]);
	}
	else
	{
		pair<int,AList*>* d = a[id];
		merge(id);
		C[id]->remove(d);
	}
}

int BtreeNode :: findPrec(int id)
{
	if(this->leaf==true){//if the key is in leafnode
		if(id!=0){//id isn't 0 means we do not need to go up to find predecessor in parents
		return this->a[id-1]->first; }//so the pre is just one element before it.
		else {
			return -999;//-999 means we need to go up to parents
		}
	} 
	BtreeNode *curr = C[id];
	while(!curr->leaf)//if the key is not in leafnode
		curr = curr->C[curr->n];//go down to chirldren
	return curr->a[curr->n-1]->first;//find the biggest key in a leafnode
}
//edit findSucc
int BtreeNode :: findSucc(int id)
{
	if(this->leaf==true){
		if(id!=n-1){//id isn't n-1 means we do not need to go up to find predecessor in parents
		return (this->a[id+1])->first; }
		else{
			return -999;
		}
	}
	BtreeNode *curr = C[id+1];
	while(!curr->leaf)
		curr = curr->C[0];
	return (curr->a[0])->first;
}



void BtreeNode ::merge(int id)
{
	BtreeNode *child = C[id];
	BtreeNode *sibling = C[id+1];

	child->a[child->n] = a[id];

	for(int i=0;i<sibling->n;i++)
		child->a[child->n+1+i] = sibling->a[i];

	if(!child->leaf)
	{	for(int i=0;i<=sibling->n;i++)
				child->C[child->n+1+i] = sibling->C[i];
	}

	for(int i=id+1;i<n;i++)
		a[i-1] = a[i];
	for(int i=id+1;i<=n;i++)
		C[i-1]=C[i];
	child->n += sibling->n+1;
	n--;
	delete(sibling);
	return ;
}

void Btree :: remove(pair<int,AList*>* k)
{
	if(!root)
	{
		cout<<"The tree is empty\n";
		return;
	}
	root->remove(k);
	if(root->n == 0)
	{
		BtreeNode *t = root;
		if(root->leaf)
			root = NULL;
		else
			root = root->C[0];
		delete t;
	}
	return;
}

void BtreeNode ::traverse()
{
	int i;
	for(i=0;i<n;i++)
	{
		if(leaf == false)
			C[i]->traverse();
		cout<<a[i]->first<<" ";
	}
	if(leaf == false)
		C[i]->traverse();
}
void BtreeNode :: fill(int id)
{
	if(id!=0 && C[id-1]->n>=t)
		borrowFromPrev(id);
	else if(id!=n && C[id+1]->n>=t)
		borrowFromSucc(id);
	else
	{
		if(id!=n)
			merge(id);
		else
			merge(id-1);
	}

}void BtreeNode :: borrowFromPrev(int id)
{
	BtreeNode *child = C[id];
	BtreeNode *sibling = C[id-1];

	for(int i = child->n-1;i >=0 ;i--)
		child->a[i+1] = child->a[i];

	if(!child->leaf)
	{
		for(int i=child->n;i>=0;i++)
			child->C[i+1] = child->C[i];
	}
	child->a[0] = a[id-1];
	if(!child->leaf)
		child->C[0] = sibling->C[sibling->n];
	a[id-1] = sibling->a[sibling->n-1];
	(child->n)++;
	(sibling->n)--;

}

void BtreeNode :: borrowFromSucc(int id)
{

	BtreeNode *child = C[id];
	BtreeNode *sibling = C[id+1];

	child->a[child->n] = a[id];
	a[id] = sibling->a[0];

	if(!child->leaf){
	child->C[(child->n)+1] = sibling->C[0];
	}

	for(int i = 1;i<sibling->n;i++ )
		sibling->a[i-1] = sibling->a[i];
	if(!child->leaf){
	    for(int i = 1;i<=sibling->n;i++ )
		    sibling->C[i-1] = sibling->C[i];
	}

	(sibling->n)--;
	(child->n)++;
}


int Btree ::find_ParentP(BtreeNode *b,pair<int,AList*>*  k)
{
	int i;
	for(i=0;i<=b->n;i++)
	{
		if(b->leaf == false){
			if(b->C[i]->leaf==true &&b->C[i]->a[0]->first==k->first){
				return b->a[i-1]->first;
			}else{
			find_ParentP(b->C[i],k);}}
		//cout<<a[i]<<" ";
	}
	
}
int Btree ::find_ParentS(BtreeNode *b,pair<int,AList*>* k)
{
	int i;
	for(i=0;i<=b->n;i++)
	{
		if(b->leaf == false){
			if(b->C[i]->leaf==true &&b->C[i]->a[b->C[i]->n-1]->first==k->first){
				return b->a[i]->first;
			}else{
		find_ParentS(b->C[i],k);}}
		//cout<<a[i]<<" ";
	}
	
}
BtreeNode * BtreeNode :: search( pair<int,AList*>* k)
{	int i;
	for(i=0;i<n && a[i]->first<=k->first;i++){
	if(a[i]->first == k->first)
		return this;}

	if(leaf == true)
		return NULL;

	return C[i]->search(k);

}


void Btree ::insert( pair<int,AList*>* k)
{
	if(root == NULL)
	{
		root = new BtreeNode(t,true);
		root->a[0] = k;
		root->n = 1;
	}
	else
	{
		if(root->n == 2*t-1)
		{
			BtreeNode *p = new BtreeNode(t,false);
			p->C[0] = root;
			p->splitChild(0,p->C[0]);
			int i=0;
			if((p->a[0])->first < k->first)
				i++;
			p->C[i]->insertNonFull(k);
			root = p;
		}
		else
			root->insertNonFull(k);
	}
}

void BtreeNode :: insertNonFull(pair<int,AList*>* k)
{
 int i = n-1;
 if(leaf == true)
 {

	 while(i >=0 && k->first < a[i]->first)
		 {
		 a[i+1] = a[i];
		 i--;
		 }
	 a[i+1] = k;
	 n = n+1;
}
 else
 {
	 while(i >=0 && k->first<a[i]->first)
		 i--;
	 if(C[i+1]->n == 2*t-1)
	 {
		 splitChild(i+1,C[i+1]);
		 if(a[i+1]->first < k->first)
			 i++;
	 }
	 C[i+1]->insertNonFull(k);

 }
}

void BtreeNode::splitChild(int i,BtreeNode *y)
{
	BtreeNode *z = new BtreeNode(y->t,y->leaf);
	z->n = t-1;

	for(int j=0;j<t-1;j++)
		z->a[j] = y->a[j+t];
	if(y->leaf == false)
	{
		for(int j=0;j<t;j++)
			z->C[j] = y->C[j+t];
	}
	y->n = t-1;
	for(int j=n;j>=i+1;j--)
		C[j+1] = C[j];
	C[i+1] = z;
	for(int j=n-1;j>=i;j--)
		a[j+1] = a[j];
	a[i]=y->a[t-1];
	n = n+1;
}
//find the max key stored in B-tree//
int Btree::find_max()
{
    //Implement your code here//
    if(root==NULL){//no max
    	return -999;
	}if(root->leaf==true) return root->a[root->n-1]->first;//if root is leafnode, return the biggest key in leafnode
    int b;
    b=root->findPrec(root->n);//if not, go find the predecessor of the biggest key in root
    return b;
}

//find the min key stored in B-tree//
int Btree::find_min()
{
    //Implement your code here//
    if(root==NULL){//no min
    
    	return -999;
	}if(root->leaf==true) return root->a[0]->first;
    int b;
    b=root->findSucc(-1);//if not, go find the successor of the smallest key in root
    return b;
}


void Btree::report_btree(int k){
	cout<<"These patients in the B-TREE has secondary key "<<k<<":"<<endl;
	int i;
	AList* pt =new AList;
	pt=retrieval(k);
	for(i=0;i<pt->numitems;i++){
		cout<<"the "<< i+1 <<"person has ID "<< pt->getitem(i)->person<<endl;
	}
}



