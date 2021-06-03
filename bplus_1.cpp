#include "block.cpp"
#include <fstream>
#include <iostream>
using namespace std;
/*
Exercise 3(i)(ii)
This file implements a B+ tree making use of primary keys(patients' ID, the person fields)
and builds a B+ tree upon the block structures established in 'block.cpp'
*/

/*
CONSTANT DEFINITIONS
*/
enum SIBLING_DIRECTION{LEFT, RIGHT};   // 兄弟结点方向：左兄弟结点、右兄弟结点
const int ORDER = 4;                   // B+树的阶（非根内结点的最小子树个数）
const int MINNUM_KEY = ORDER-1;        // 最小键值个数
const int MAXNUM_KEYNUM_KEY = 2*ORDER-1;      // 最大键值个数
const int MINNUM_CHILD = MINNUM_KEY+1; // 最小子树个数
const int MAXNUM_KEYNUM_CHILD = MAXNUM_KEYNUM_KEY+1; // 最大子树个数
const int MINNUM_LEAF = MINNUM_KEY;    // 最小叶子结点键值个数
const int MAXNUM_KEYNUM_LEAF = MAXNUM_KEYNUM_KEY;    // 最大叶子结点键值个数

/*
RELEVANT CLASS DEFINITIONS
*/
//BASIC CLASS
class node {
    public:
    //modified for CA2
    int key[MAXNUM_KEYNUM_KEY];     //key 
    int key_num; //节点当前的关键码个数
    bool is_leaf;//1-叶节点   0-内部结点     

    int getKeyIndex(int key);
    int getKeyValue(int i) const {return key[i];}
    void setKeyNum(int n){key_num = n;}
	void setKeyValue(int i, int ID){key[i] = ID;}

    virtual void split(node* parentNode, int childIndex)=0;
    virtual void mergeChild(node* parentNode, node* childNode, int keyIndex)=0;
    virtual void removeKey(int keyIndex, int childIndex)=0; 
    virtual void borrowFrom(node* destNode, node* parentNode, int keyIndex, SIBLING_DIRECTION d)=0;
};
//LEAFNODES
class leafnode:public node{
    public:
    block* data[MAXNUM_KEYNUM_KEY]; //we store pointers to blocks in leaf nodes
    leafnode* leftSibling;
	leafnode* rightSibling;

    void split(node* parentNode, int childIndex);
    block* insert(int ID);
    void insert(int ID, block* place);

    leafnode(){
        is_leaf=1;
        for(int i=0;i<MAXNUM_KEYNUM_KEY;i++){
            data[i]=new block;    //initialize empty blocks
        }
		key_num=0;
        leftSibling=NULL;
        rightSibling=NULL;
    }
    virtual void mergeChild(node* parentNode, node* childNode, int keyIndex);
    virtual void removeKey(int keyIndex, int childIndex); 
    virtual void borrowFrom(node* destNode, node* parentNode, int keyIndex, SIBLING_DIRECTION d);
    
};
//INTERNALNODES
class internalnode:public node{
    public:
    node* child[MAXNUM_KEYNUM_KEY];
    void split(node* parentNode, int childIndex);
    void insert(int keyIndex, int childIndex, int key, node* childNode);
    void setChild(int i, node* kid){child[i] = kid;}

    internalnode(){
        is_leaf=0;
		key_num=0;
    }

    virtual void mergeChild(node* parentNode, node* childNode, int keyIndex);
    virtual void removeKey(int keyIndex, int childIndex); 
    virtual void borrowFrom(node* destNode, node* parentNode, int keyIndex, SIBLING_DIRECTION d);
	int getChildIndex(int key,int keyIndex);

};


//CLASS DEFINITION FOR a B+ TREE
class B_plus_tree
{
public:
//modified for CA2
    node *root;
    leafnode *leafhead;
//FOR ADDING
    void add(patient* p);
    block* insert(int ID);
    bool insert(int ID, block* place); //used when split of blocks reach max of leafnode
    block* recursive_insert(node* parentnode, int ID);
    void recursive_insert(node* parentNode, int ID, block* place);
//FOR SEARCHING
    patient* search(int ID);
    block* find(int ID);
//FOR DELETING
    void _delete(int ID);
    void remove(int ID);
    void recursive_remove(node* root, int ID);
    void changeKey(node *pNode, int oldKey, int newKey);
//FOR REPORTS
    void report();
	void monthlyreport(ofstream& ofile);
//FOR TESTINGs
    void print();
    B_plus_tree() 
    {
        root=NULL;
        leafhead=NULL;
    }
    ~B_plus_tree()
    {
    }
};




/*
PART1: FOR INSERTION INTO B-PLUS TREE
*/
void B_plus_tree::add(patient* p){
    block* placing=this->insert(p->person); //return block*
    if(placing==NULL) return;
	int max=placing->maxdata;
    block* check=placing->insert(p);
    if(check==NULL) {
		return;
	}

	insert(check->maxdata,check);
	// insert(check->frontP->maxdata,check->frontP);
	leafnode* search=leafhead;
	while(search!=NULL){
        int size=search->key_num;
        for(int i=0;i<size;i++){
            search->key[i]=search->data[i]->maxdata;
        }
        search=search->rightSibling;
    }
}
block* B_plus_tree::insert(int ID){
    if (search(ID)!=NULL)
	{
		return NULL;
	}
	// 找到可以插入的叶子结点，否则创建新的叶子结点
	if(root==NULL)  
	{
		root = new leafnode();
		leafhead=(leafnode*)root;
        root->key[0]=ID;
        root->key_num++;
        leafnode* tmp=(leafnode*) root;
        return tmp->data[0];
	}
	if (root->key_num>=MAXNUM_KEYNUM_KEY) // 根结点已满，分裂
	{
		internalnode* newNode = new internalnode();  //创建新的根节点
		newNode->child[0]=root;
		root->split(newNode, 0);    // 叶子结点分裂 改
		root = newNode;  //更新根节点指针
	}

	return recursive_insert(root, ID);
}

block* B_plus_tree::recursive_insert(node* root, int ID){
    if (root->is_leaf)  // 叶子结点，直接插入
	{
		return ((leafnode*)root)->insert(ID);
	}
	else
	{	
		// 找到子结点
		int keyIndex = ((internalnode*)root)->getKeyIndex(ID);
		int childIndex= ((internalnode*)root)->getChildIndex(ID, keyIndex); // 孩子结点指针索引
		node* childNode = ((internalnode*)root)->child[childIndex];
		if (childNode->key_num>=MAXNUM_KEYNUM_KEY)  // 子结点已满，需进行分裂
		{	
			childNode->split(root, childIndex);      
			if (root->getKeyValue(childIndex)<=ID)   // 确定目标子结点
			{
				childNode = ((internalnode*)root)->child[childIndex+1];
			}
		}
		return recursive_insert(childNode, ID);
	}
}

block* leafnode::insert(int ID){ //no need to consider if full or not because the case is considered in caller
    int i;
    if(ID > key[key_num-1]){

        key_num++;
		key[key_num-1]=ID;
        return data[key_num-1];
    }
	// if(rightSibling!=NULL) return rightSibling->insert(ID);
	for (i=0; i<key_num && key[i]<ID; i++);
    return data[i];
}

int internalnode::getChildIndex(int ID, int keyIndex)
{
	if (ID>=getKeyValue(keyIndex))
	{
		return keyIndex+1;
	}
	else
	{
		return keyIndex;
	}
}

int node::getKeyIndex(int key)
{
	int left = 0;
	int right = key_num-1;
	int current;
	while(left!=right)
	{
		current = (left+right)/2;
		int currentKey = getKeyValue(current);
		if (key>currentKey)
		{
			left = current+1;
		}
		else
		{
			right = current;
		}
	}
	return left;
}

void internalnode::split(node* parentNode, int childIndex)
{
	internalnode* newNode = new internalnode;//分裂后的右节点
	newNode->key_num=MINNUM_KEY;
	int i;
	for (i=0; i<MINNUM_KEY; ++i)// 拷贝关键字的值
	{
		newNode->key[i]=key[i+MINNUM_CHILD];
	}
	for (i=0; i<MINNUM_CHILD; ++i) // 拷贝孩子节点指针
	{
		newNode->child[i]=child[i+MINNUM_CHILD];
	}
	key_num=MINNUM_KEY;  //更新左子树的关键字个数
	((internalnode*)parentNode)->insert(childIndex, childIndex+1, key[MINNUM_KEY], newNode);
}

void internalnode::insert(int keyIndex, int childIndex, int ID, node* childNode){
	int i;
	for (i=key_num; i>keyIndex; --i)//将父节点中的childIndex后的所有关键字的值和子树指针向后移一位
	{
		setChild(i+1,child[i]);
		setKeyValue(i,key[i-1]);
	}
	if (i==childIndex)
	{
		setChild(i+1, child[i]);
	}
	setChild(childIndex, childNode);
	setKeyValue(keyIndex, ID);
	key_num++;
}

void leafnode::split(node* parentNode, int childIndex)
{
	leafnode* newNode = new leafnode();//分裂后的右节点

	newNode->setKeyNum(MINNUM_LEAF+1);
	newNode->rightSibling=rightSibling;
	rightSibling=newNode;
	newNode->leftSibling=this;
	int i;
	for (i=0; i<MINNUM_LEAF+1; ++i)// 拷贝关键字的值
	{
		newNode->setKeyValue(i, key[i+MINNUM_LEAF]);
	}
    for(i=0;i<MINNUM_LEAF+1;i++){
        newNode->data[i]=data[i+MINNUM_LEAF];
    }
	for(i=MINNUM_LEAF;i<key_num;i++){
       data[i]=new block;
    }
	setKeyNum(MINNUM_LEAF);  
	((internalnode*)parentNode)->insert(childIndex, childIndex+1, data[key_num-1]->maxdata, newNode);
}

bool B_plus_tree::insert(int ID, block* place){

	// 找到可以插入的叶子结点，否则创建新的叶子结点
	if(root==NULL)  
	{
		root = new leafnode();
		leafhead = (leafnode*)root;
	}
	if (root->key_num>=MAXNUM_KEYNUM_KEY) // 根结点已满，分裂
	{
		internalnode* newNode = new internalnode();  //创建新的根节点
		newNode->setChild(0, root);
		root->split(newNode, 0);    // 叶子结点分裂
		root = newNode;  //更新根节点指针
	}
	recursive_insert(root, ID, place);
	return true;
}
 
void B_plus_tree::recursive_insert(node* parentNode, int ID, block* place)
{
	if (parentNode->is_leaf)  // 叶子结点，直接插入
	{
		((leafnode*)parentNode)->insert(ID, place);
	}
	else
	{
		// 找到子结点
		int keyIndex = ((internalnode*)parentNode)->getKeyIndex(ID);
		int childIndex= ((internalnode*)parentNode)->getChildIndex(ID, keyIndex); // 孩子结点指针索引
		node* childNode = ((internalnode*)parentNode)->child[childIndex];
		if (childNode->key_num>=MAXNUM_KEYNUM_KEY)  // 子结点已满，需进行分裂
		{
			childNode->split(parentNode, childIndex);      
			if (parentNode->getKeyValue(childIndex)<=ID)   // 确定目标子结点
			{
				childNode = ((internalnode*)parentNode)->child[childIndex+1];
			}
		}
		recursive_insert(childNode, ID, place);
	}
}

void leafnode::insert(int ID, block* place)
{
	int i;
	for (i=key_num; i>=1 && key[i-1]>ID; --i)
	{
		setKeyValue(i, key[i-1]);
		data[i]=data[i-1];
	}
	setKeyValue(i, ID);
	data[i]=place;
	key_num++;
}


/*
PART2: FOR SEARCHING IN B-PLUS TREE
*/
patient* B_plus_tree::search(int ID)
{
    block* fnd=find(ID);
    if(fnd==NULL) {
		return NULL;
	}
	return fnd->retrieval(ID);
}
 
block* B_plus_tree::find(int ID)
{

    leafnode* check=leafhead;
    while(check!=NULL && check->key[check->key_num-1]<ID) check=check->rightSibling;
    if(check==NULL) return NULL;
    int i;
    for(i=0;i<check->key_num && check->key[i]<ID;i++);
    return check->data[i];
}


/*
PART3: FOR DELETION
*/
void B_plus_tree::_delete(int ID){
    block* fnd=find(ID);
    if(fnd==NULL) return;
    block* place=fnd->delete_(ID);//pointer to the deleted block
    if(place==NULL) {
		return;
	}
    remove(place->maxdata);       //we are removing the key pointing to the deleted block

}
void B_plus_tree::remove(int ID)
{
	if (root->key_num==1)//特殊情况处理
	{
		if (root->is_leaf)
		{
			return;
		}
		else
		{
			node *pChild1 = ((internalnode*)root)->child[0];
			node *pChild2 = ((internalnode*)root)->child[1];
			if (pChild1->key_num==MINNUM_KEY && pChild2->key_num==MINNUM_KEY)
			{
				pChild1->mergeChild(root, pChild2, 0);
				root = pChild1;
			}
		}
	}
	recursive_remove(root, ID);
	return;
}
void B_plus_tree::recursive_remove(node* parentNode, int ID)
{
	int keyIndex = parentNode->getKeyIndex(ID);
	int childIndex= ((internalnode*)parentNode)->getChildIndex(ID, keyIndex); // 孩子结点指针索引
	if (parentNode->is_leaf)// 找到目标叶子节点
	{
		parentNode->removeKey(keyIndex, childIndex);  // 直接删除
		// 如果键值在内部结点中存在，也要相应的替换内部结点
		if (childIndex==0 && root->is_leaf==0 && parentNode!=leafhead)
		{
			changeKey(root, ID, parentNode->getKeyValue(0));
		}
	}
	else // 内结点
	{
		node *pChildNode = ((internalnode*)parentNode)->child[childIndex]; //包含key的子树根节点
		if (pChildNode->key_num==MINNUM_KEY)                       // 包含关键字达到下限值，进行相关操作
		{
			node *pLeft = childIndex>0 ? ((internalnode*)parentNode)->child[childIndex-1] : NULL;                       //左兄弟节点
			node *pRight = childIndex<parentNode->key_num ? ((internalnode*)parentNode)->child[childIndex+1] : NULL;//右兄弟节点
			// 先考虑从兄弟结点中借
			if (pLeft && pLeft->key_num>MINNUM_KEY)// 左兄弟结点可借
			{
				pChildNode->borrowFrom(pLeft, parentNode, childIndex-1, LEFT);
			}
			else if (pRight && pRight->key_num>MINNUM_KEY)//右兄弟结点可借
			{
				pChildNode->borrowFrom(pRight, parentNode, childIndex, RIGHT);
			}
			//左右兄弟节点都不可借，考虑合并
			else if (pLeft)                    //与左兄弟合并
			{
				pLeft->mergeChild(parentNode, pChildNode, childIndex-1);
				pChildNode = pLeft;
			}
			else if (pRight)                   //与右兄弟合并
			{
				pChildNode->mergeChild(parentNode, pRight, childIndex);
			}
		}
		recursive_remove(pChildNode, ID);
	}
}

void B_plus_tree::changeKey(node *pNode, int oldKey, int newKey)
{
	if (pNode!=NULL && pNode->is_leaf==0)
	{
		int keyIndex = pNode->getKeyIndex(oldKey);
		if (keyIndex<pNode->key_num && oldKey==pNode->getKeyValue(keyIndex))  // 找到
		{
			pNode->setKeyValue(keyIndex, newKey);
		}
		else   // 继续找
		{
			changeKey(((internalnode*)pNode)->child[keyIndex], oldKey, newKey);
		}
	}
}

void internalnode::mergeChild(node* parentNode, node* childNode, int keyIndex)
{
	// 合并数据
	insert(MINNUM_KEY, MINNUM_KEY+1, parentNode->getKeyValue(keyIndex), ((internalnode*)childNode)->child[0]);
	int i;
	for (i=1; i<=childNode->key_num; ++i)
	{
		insert(MINNUM_KEY+i, MINNUM_KEY+i+1, childNode->getKeyValue(i-1), ((internalnode*)childNode)->child[i]);
	}
	//父节点删除index的key
	parentNode->removeKey(keyIndex, keyIndex+1);
	delete ((internalnode*)parentNode)->child[keyIndex+1];
}

void leafnode::mergeChild(node* parentNode, node* childNode, int keyIndex)
{
	// 合并数据
	for (int i=0; i<childNode->key_num; ++i)
	{
		insert(childNode->getKeyValue(i), ((leafnode*)childNode)->data[i]);
	}
	rightSibling=((leafnode*)childNode)->rightSibling;
	//父节点删除index的key，
	parentNode->removeKey(keyIndex, keyIndex+1);
}

void internalnode::removeKey(int keyIndex, int childIndex)
{
	for (int i=0; i<key_num-keyIndex-1; ++i)
	{
		setKeyValue(keyIndex+i, getKeyValue(keyIndex+i+1));
		setChild(childIndex+i, child[childIndex+i+1]);
	}
	key_num--;
}

void leafnode::removeKey(int keyIndex, int childIndex)
{
	for (int i=keyIndex; i<key_num-1; ++i)
	{
		setKeyValue(i, getKeyValue(i+1));
		data[i]=data[i+1];
	}
	key_num--;
}

void internalnode::borrowFrom(node* siblingNode, node* parentNode, int keyIndex, SIBLING_DIRECTION d)
{
	switch(d)
	{
	case LEFT:  // 从左兄弟结点借
		{
			insert(0, 0, parentNode->getKeyValue(keyIndex), ((internalnode*)siblingNode)->child[siblingNode->key_num]);
			parentNode->setKeyValue(keyIndex, siblingNode->getKeyValue(siblingNode->key_num-1));
			siblingNode->removeKey(siblingNode->key_num-1, siblingNode->key_num);
		}
		break;
	case RIGHT:  // 从右兄弟结点借
		{
			insert(key_num, key_num+1, parentNode->getKeyValue(keyIndex), ((internalnode*)siblingNode)->child[0]);
			parentNode->setKeyValue(keyIndex, siblingNode->getKeyValue(0));
			siblingNode->removeKey(0, 0);
		}
		break;
	default:
		break;
	}
}

void leafnode::borrowFrom(node* siblingNode, node* parentNode, int keyIndex, SIBLING_DIRECTION d)
{
	switch(d)
	{
	case LEFT:  // 从左兄弟结点借
		{
			insert(siblingNode->getKeyValue(siblingNode->key_num-1), ((leafnode*)siblingNode)->data[siblingNode->key_num-1]);
			siblingNode->removeKey(siblingNode->key_num-1, siblingNode->key_num-1);
			parentNode->setKeyValue(keyIndex, getKeyValue(0));			
		}
		break;
	case RIGHT:  // 从右兄弟结点借
		{
			insert(siblingNode->getKeyValue(0), ((leafnode*)siblingNode)->data[0]);
			siblingNode->removeKey(0, 0);
			parentNode->setKeyValue(keyIndex, siblingNode->getKeyValue(0));
		}
		break;
	default:
		break;
	}
}

/*
PART4: FOR TEST&OUTPUT
EX2
*/
void B_plus_tree::print(){
	leafnode* search=leafhead;
	int j=0;
	while(search!=NULL){
		cout<<"Leaf Node"<<j<<endl;
		int size=search->key_num;
		cout<<"size"<<size<<endl;
		for(int i=0;i<size;i++){
			search->data[i]->print();
		}
		search=search->rightSibling;
		j++;
	}
}

void B_plus_tree::report(){

	ofstream ofile;
	ofile.open( "./Reports.txt" );

	leafnode* search=leafhead;
	while(search!=NULL){
		int size=search->key_num;
		for(int i=0;i<size;i++){
			(search->data[i])->report_(ofile);
		}
		search=search->rightSibling;
	}

	ofile.close();
}

void  B_plus_tree::monthlyreport(ofstream& ofile){
	vector<patient*> pa; 
	vector<int> pr0,pr1,pr2,pr3;
	vector<int> pt0,pt1,pt2,pt3;
	vector<int> ptt0,ptt1,ptt2;
 	int i;
 	int u;
 	int m;
 	leafnode* p=new leafnode;
 	p=leafhead;
	int r0,r1,r2,r3,r4,r5,r6=0;
	int t0,t1,t2,t3,t4,t5,t6=0;
	int tt0,tt1,tt2=0;
    while(p!=NULL){
    	for(int i=0;i<p->key_num;i++){
    		for(int j=0;j<p->data[i]->main_blk.numitems;j++){
    			pa.push_back(p->data[i]->main_blk.getitem(j));
			}for(int j=0;j<p->data[i]->overflow_blk.numitems;j++){
    			pa.push_back(p->data[i]->overflow_blk.getitem(j));
			}
    		
		}
		p=p->rightSibling;
	}
	for(m=0;m<pa.size();m++){
		
		if(pa[m]->registration==0){
			r0++;
			pr0.push_back(pa[m]->person);
		}
		if(pa[m]->registration==1){
			r1++;
			pr1.push_back(pa[m]->person);
		}
		if(pa[m]->registration==2){
			r2++;
			pr2.push_back(pa[m]->person);
		}
		if(pa[m]->registration==3){
			r3++;
			pr3.push_back(pa[m]->person);
		}
	
	} 
	for(m=0;m<pa.size();m++){
		if(pa[m]->treat_day==0)
		{
		t0++;
		pt0.push_back(pa[m]->person);
			}	
		if(pa[m]->treat_day==1)
		{
			t1++;
			pt1.push_back(pa[m]->person);
		}
		if(pa[m]->treat_day==2){
			t2++;
			pt2.push_back(pa[m]->person);
		}
		if(pa[m]->treat_day==3){
			t3++;
			pt3.push_back(pa[m]->person);
		}
		if(pa[m]->treat_day==4){
			t4++;
		}
		if(pa[m]->treat_day==5){
			t5++;
		}if(pa[m]->treat_day==6){
			t6++;
		}
}

    for(m=0;m<pa.size();m++){
        if(pa[m]->treatment==0){
        	ptt0.push_back(pa[m]->person);
        	tt0++; 
		}
		if(pa[m]->treatment==1){
			ptt1.push_back(pa[m]->person);
        	tt1++; 
		}
		if(pa[m]->treatment==2){
			ptt2.push_back(pa[m]->person);
        	tt2++; 
		}
		}
		ofile<<"This month the  people who were registered on week1 has ID: "<<endl;
		for(m=0;m<pr0.size();m++){
			ofile<<pr0[m]<<endl;
		}
		ofile<<"This month the number of people who were registered on week1 is "<< r0<<endl;
		ofile<<"This month the  people who were registered on week2 has ID: "<<endl;
		for(m=0;m<pr1.size();m++){
			ofile<<pr1[m]<<endl;
		}
		ofile<<"This month the number of people who were registered on week2 is "<< r1<<endl;
		ofile<<"This month the  people who were registered on week3 has ID: "<<endl;
		for(m=0;m<pr2.size();m++){
			ofile<<pr2[m]<<endl;
		}
		ofile<<"This month the number of people who were registered on week3 is "<< r2<<endl;
		ofile<<"This month the  people who were registered on week4 has ID: "<<endl;
		for(m=0;m<pr3.size();m++){
			ofile<<pr3[m]<<endl;
		}
			ofile<<"This month the number of people who were registered on week4 is "<< r3<<endl;
		
		ofile<<"This month the  people who were treated on week1 has ID: "<<endl;
		for(m=0;m<pt0.size();m++){
			ofile<<pt0[m]<<endl;
		}
		ofile<<"This month the number of people who were treated on week1 is "<< t0<<endl;
		ofile<<"This month the  people who were treated on week2 has ID: "<<endl;
		for(m=0;m<pt1.size();m++){
			ofile<<pt1[m]<<endl;
		}
		ofile<<"This month the number of people who were treated on week2 is "<< t1<<endl;
		ofile<<"This month the  people who were treated on week3 has ID: "<<endl;
		for(m=0;m<pt2.size();m++){
			ofile<<pt2[m]<<endl;
		}
		ofile<<"This month the number of people who were treated on week3 is "<< t2<<endl;
		ofile<<"This month the  people who were treated on week4 has ID: "<<endl;
		for(m=0;m<pt3.size();m++){
			ofile<<pt3[m]<<endl;
		}
		ofile<<"This month the number of people who were treated on week4 is "<< t3<<endl;
		
		ofile<<"This month the  people who registered on vaccination on has ID: "<<endl;
		for(m=0;m<ptt0.size();m++){
			ofile<<ptt0[m]<<endl;
		}
			ofile<<"This month the number of people who registered on vaccination is "<< tt0<<endl;
		ofile<<"This month the  people who were treated on surgery  has ID: "<<endl;
		for(m=0;m<ptt1.size();m++){
			ofile<<ptt1[m]<<endl;
		}
		ofile<<"This month the number of people who registered surgery is "<< tt1<<endl;
		ofile<<"This month the  people who were treated on take pills  has ID: "<<endl;
		for(m=0;m<ptt2.size();m++){
			ofile<<ptt2[m]<<endl;
		}
	ofile<<"This month the number of people who registered on pills is "<< tt2<<endl;
		
		
		}
