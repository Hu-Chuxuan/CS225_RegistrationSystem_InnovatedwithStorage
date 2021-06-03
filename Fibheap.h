

/*
Fibonacci Heap

Created by Hu Chuxuan on 04.04.2021
*/
#ifndef Fibheap_h
#define Fibheap_h

#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include<io.h>
#include "alist.h"
#include <math.h>
#include "alist.cpp"
#include<algorithm>
#include <iomanip>
#include <cmath>
using namespace std;


 
class FibNode : public patient {
      public:
        int key;   // timestamp
        int degree;            // 度数
        FibNode *left;    // 左兄弟
        FibNode *right;    // 右兄弟
        FibNode *child;    // 第一个孩子节点
        FibNode *parent;    // 父节点
        bool marked;        // 是否被删除第一个孩子
        int priority();
  
        FibNode() {
            degree = 0;
            marked = false;
            left   = this;
            right  = this;
            parent = NULL;
            child  = NULL;
         }
};

//Exercise1(ii)
//Three types of treatments, each with a different priority queue
int FibNode::priority(){
    if(treatment==1) return person;
    if(treatment==2) return medical_status*1000+person;
    if(treatment==3) return registration*1000+person;
    else return 0;
} 


//Centralized
class FibHeap {
    public:
        int keyNum;         // 堆中节点的总数
        int maxDegree;      // 最大度
        FibNode *min;    // 最小节点(某个最小堆的根节点)
        FibNode **cons;    // 最大度的内存区域
        //When waiting time exceeds this, pop out unconditionally
        int Deadline; 
 
         FibHeap();
         ~FibHeap();

         //get the minimum
        FibNode* getmin();

         //For Updates everyday
        void daily(FibNode* root); 

        //For Withdrawals
        void withdraw(int ID);
 
         // For adding to centralized
        void insert(FibNode *node);    //IMPORTANT ASSUMPTION:
                                       //before popping from queues, it is necessary to transform into a FibNode
        void addNode(FibNode *node, FibNode *root);



        // For popping the minimum
        bool checkMin();
        patient* removeMin();
        void removeNode(FibNode *node);
        FibNode* extractMin();
        void consolidate();
        void link(FibNode* node, FibNode* root);
        void makeCons();

        //For updates
        void update(int identity, int newval,int item);
        void decrease(FibNode *node, int newkey,int item);
        void increase(FibNode *node, int newkey,int item);   
        void update(FibNode *node, int newkey,int item);   
        // 在最小堆root中查找键值为key的节点
        FibNode* search(FibNode *root, int identity);
                 // 在斐波那契堆中查找键值为key的节点
        FibNode* search(int identity);
        bool contains(int identity);
        void cut(FibNode *node, FibNode *parent);
        void cascadingCut(FibNode *node);
        void renewDegree(FibNode *parent, int degree);
};



//Constructor
FibHeap::FibHeap()
{
     keyNum = 0;
     maxDegree = 0;
     min = NULL;
     cons = NULL;
     Deadline=5;
}


FibHeap::~FibHeap() 
{
}

//From Locals to Centralized
void FibHeap::addNode(FibNode *node, FibNode *root)
{
    node->left        = root->left;
    root->left->right = node;
    node->right       = root;
    root->left        = node;
}

void FibHeap::insert(FibNode *node)
{
    if(contains(node->person)){
        char flg;
        cout<<"This person is already there!\n";
        cout<<"Do you want to update his information?y(yes)n(no)\n";
        cin>>flg;
        if(flg=='n') return;
        cout<<"Which item you want to update his information?profession(0)risk(1)\n";
        int choice;
        cin>>choice;
        cout<<"To what?\n";
        int newkey;
        cin>>newkey;
        update(node,newkey,choice);
        printf("%d\n",node->profession);
        return;
    }
     if (keyNum == 0) min = node;
     else{
         addNode(node, min);
         if (node->key < min->key)
             min = node;
        }
     keyNum++;
}


//From Centralized to Hospitals
void FibHeap::removeNode(FibNode *node)
{
     node->left->right = node->right;
     node->right->left = node->left;
}

bool FibHeap::checkMin()
{
	FibNode* m = min;
	if (min==NULL){
         return 0;
     }
     if(m->suppose>0) return 0;
     return 1;
}


patient* FibHeap::removeMin()
{
     if (min==NULL){
         return NULL;
     }
 
    FibNode* child=NULL;
    FibNode* m = min;

    //MODIFIED FOR CA2
    patient* ret=(patient*)m;

    while (m->child != NULL)
    {
         child = m->child;
         removeNode(child);
         if (child->right == child)
             m->child = NULL;
         else
             m->child = child->right;
 
         addNode(child, min);
         child->parent = NULL;
    } 

    removeNode(m);

    if(m->right == m) {
         min = NULL;
    }
    else{
         min = m->right;
         consolidate();
    }

    keyNum--;
    return ret;
}


void FibHeap::link(FibNode* node, FibNode* root)
{
     // 将node从双链表中移除
     removeNode(node);
     // 将node设为root的孩子
     if (root->child == NULL)
         root->child = node;
     else
         addNode(node, root->child); 
     node->parent = root;
     root->degree++;
     node->marked = false;
}
  
/* 
 * 创建consolidate所需空间
 */

void FibHeap::makeCons()
{
     int old = maxDegree;
 

     maxDegree = (log(keyNum)/log(2.0)) + 1;
     if (old >= maxDegree)
         return ;

     cons = (FibNode **)realloc(cons, sizeof(FibHeap*) * (maxDegree + 1));
}
void FibHeap::consolidate()
{
     int i;
     int d;
     int D;
     FibNode* x;
     FibNode* y;
     FibNode* tmp;
 
     makeCons();
     D = maxDegree + 1;
 
     for (i = 0; i < D; i++)
         cons[i] = NULL;
  
 
     while (min != NULL){
        x = extractMin();              
        d = x->degree;              
        while (cons[d] != NULL){
            y = cons[d];           
            if (x->key > y->key){        
                 swap(x, y);
            }
            link(y, x);
            cons[d] = NULL;
            d++;
        }
        cons[d] = x;
    }
    
    min = NULL;
  
    for(i=0; i<D; i++)
     {
         if (cons[i] != NULL)
         {
             if (min == NULL)
                 min = cons[i];
             else
             {
                 addNode(cons[i], min);
                 if ((cons[i])->key < min->key)
                     min = cons[i];
             }
         }
     }
 }


//Updates

FibNode* FibHeap::search(FibNode *root, int identity)
{
    FibNode *t = root;
    FibNode *p = NULL;
    if (root==NULL)
         return root;
    do
    {
         if (t->person == identity)
         {
             p = t;
             break;
         } 
         else
         {
             if ((p = search(t->child, identity)) != NULL) 
                 break;
         }    
         t = t->right;
     } while (t != root);
     return p;
}

FibNode* FibHeap::search(int identity)
{
     if (min==NULL)
         return NULL;
 
     return search(min, identity);
}
bool FibHeap::contains(int identity)
{
     return search(identity)!=NULL ? true: false;
}

void FibHeap::decrease(FibNode *node, int newkey,int item){
    //item 0 means we do PROFESSION adjustments
    FibNode* parent; 
    if (min==NULL ||node==NULL) return ;

    if(item==0) node->profession = newkey;
    else{
        node->risk=newkey;
        if(node->risk==0 || node->risk==1) node->suppose=0;
        else if(node->risk==2) node->suppose=30;
    }

    node->key=node->priority();
    parent = node->parent;
    if (parent!=NULL && node->key < parent->key)
    {
        // 将node从父节点parent中剥离出来，并将node添加到根链表中
        cut(node, parent);
        cascadingCut(parent);
    }
    // 更新最小节点
    if (node->key < min->key) min = node;
    return;
}
void FibHeap::cut(FibNode *node, FibNode *parent)
{
     removeNode(node);
     renewDegree(parent, node->degree);
     // node没有兄弟
     if (node == node->right) 
         parent->child = NULL;
     else 
         parent->child = node->right;
 
     node->parent = NULL;
     node->left = node->right = node;
     node->marked = false;
     // 将"node所在树"添加到"根链表"中
     addNode(node, min);
}

void FibHeap::cascadingCut(FibNode *node) 
{
     FibNode *parent = node->parent;
     if (parent != NULL)
     {
         if (node->marked == false) 
             node->marked = true;
         else
         {
             cut(node, parent);
             cascadingCut(parent);
         }
     }
}

void FibHeap::renewDegree(FibNode *parent, int degree)
{
     parent->degree -= degree;
     if (parent-> parent != NULL)
         renewDegree(parent->parent, degree);
}

void FibHeap::increase(FibNode *node, int newkey, int item)
{
     FibNode *child, *parent, *right;
 
     if (min==NULL ||node==NULL) 
         return ; 

     // 将node每一个儿子(不包括孙子,重孙,...)都添加到"斐波那契堆的根链表"中
     while (node->child != NULL)
     {
         child = node->child;
         removeNode(child);               // 将child从node的子链表中删除
         if (child->right == child)
             node->child = NULL;
         else
             node->child = child->right;
 
         addNode(child, min);       // 将child添加到根链表中
         child->parent = NULL;
     }
    node->degree = 0;
    if(item==0) node->profession = newkey;
    else{
        node->risk=newkey;
        if(node->risk==0 || node->risk==1) node->suppose=0;
        else if(node->risk==2) node->suppose=30;
    }
    node->key=node->priority();

    parent = node->parent;
    if(parent != NULL)
    {
         cut(node, parent);
         cascadingCut(parent);
    }
     else if(min == node)
     {
         right = node->right;
         while(right != node)
         {
             if(node->key > right->key)
                 min = right;
             right = right->right;
         }
     }
}


void FibHeap::update(FibNode *node, int newkey,int item){
    if(item==0 && newkey < node->profession)
         decrease(node, newkey,item);
    else if(item==0 && newkey > node->profession)
         increase(node, newkey,item);
    else if(item==1&&newkey > node->risk)
        increase(node, newkey,item);   
    else
        decrease(node,newkey,item);
}  

void FibHeap::update(int identity, int newval,int item)
{
     FibNode *node;
 
     node = search(identity);
     if (node!=NULL)
         update(node, newval,item);
}


// void FibHeap::withdraw(int identity){
//     FibNode* removal=search(identity);
//     if(removal==NULL){
//         return;
//     }
//     WithDrawals.push_back(identity);
//     removal->left->right = removal->right;
//     removal->right->left = removal->left;
//     keyNum--;
// }

void FibHeap::daily(FibNode* root){
    if(root==NULL) return;

    root->suppose-=0.5;
    root->waiting+=0.5;
    if(root->waiting>=Deadline){
        root->key=0;
    }else{
        root->key=root->priority();
    }

    FibNode* t=root;
    do
    {
        daily(t->child); 
        t = t->right;
     } while (t != root);

}





FibNode* FibHeap::extractMin()
{
     FibNode *p = min;
 
     if (p == p->right)
         min = NULL;
     else
     {
         removeNode(p);
         min = p->right;
     }
     p->left = p->right = p;
 
     return p;
}


FibNode* FibHeap:: getmin(){
    return min;
};


#endif


