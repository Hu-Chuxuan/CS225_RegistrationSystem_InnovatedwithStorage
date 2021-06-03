#include <iostream>
#include <fstream>
#include <cstdlib>
#include "alist.cpp"
using namespace std;
#define max_main 6
#define max_of 2


//Exercise1(iii)-(v)
//operations and definitions on blocks
 class block{
    public:
    	block(void);
        AList main_blk;
        AList overflow_blk;
        block *frontP;//指向前面的block
        block *backP;//指向后面的block
        block* dummy1;
        block* dummy2;
        int maxdata;
        block* insert(patient* a);
        block* delete_(int key);
        block* merge(void);
        void sorting(block* p);
        block* split(void);
        void print(void);
        patient* retrieval(int key);
        void report_(ofstream& ofile);
        
};
 block::block(){
 	main_blk=AList(6);
 	overflow_blk=AList(2);
    frontP=NULL;
    backP=NULL;
    maxdata=0;
}

//INSERTION
 block* block::insert(patient* a){//自己定的of一定要能在split后全部放在第一个main
    if (overflow_blk.numitems<max_of){//直接加在overflow中
        overflow_blk.append(a);//Gai
        if(a->person>maxdata){
            maxdata=a->person;
        }
    }else{//overflow满了
        int i=0;
        while(main_blk.numitems<max_main&&i<max_of){//先将overflow中的数据放到main中
            main_blk.append(overflow_blk.getitem(overflow_blk.numitems-1));
            overflow_blk.numitems--;
            i++;
        }
        if(overflow_blk.numitems!=0){//如果main超了max,需要分裂
            sorting(this);
            block* p=new block;
            p=split();
            
            p->frontP=this;//tttt
            p->backP=backP;
            if(backP!=NULL){
            backP->frontP=p;}
			backP=p;//不知道会不会有问题
            if(overflow_blk.numitems==max_of){//如果of是满的，再放入main,然后再把a加入of
                int i;
                for(i=0;i<max_of;i++){
                if(overflow_blk.getitem(overflow_blk.numitems-1)->person>maxdata){
                	(p->main_blk).append(overflow_blk.getitem(overflow_blk.numitems-1));
                	overflow_blk.numitems--;
				}else{
					main_blk.append(overflow_blk.getitem(overflow_blk.numitems-1));
					overflow_blk.numitems--;
				}
		} sorting(this); 
     		
                sorting(p); 
               
                 if(maxdata<a->person){
                 	
                     p->overflow_blk.append(a);
                     
                 }else{
                    overflow_blk.append(a);
                     
                 }
                 return p;
            }else{int temp= overflow_blk.numitems;
			for(i=0;i<temp;i++){
            	if(overflow_blk.getitem(overflow_blk.numitems-1)->person>maxdata){
                	(p->overflow_blk).append(overflow_blk.getitem(overflow_blk.numitems-1));
                	overflow_blk.numitems--;
				}
			}
                if(maxdata<a->person){
                     p->overflow_blk.append(a);
                 }else{
                     overflow_blk.append(a);
                 };
                return p;
            }
        }else{
            overflow_blk.append(a);
            
            sorting(this);
        } 
        return  NULL;    
    }return NULL;
}

//DELETION
 block* block::delete_(int key){
    int i;
    int flg=0;
    for(i=0;i<overflow_blk.numitems;i++){
        if(key==(overflow_blk.getitem(i))->person){
            overflow_blk.remove(i+1);//改
            return NULL;
        }
    }for(i=0;i<main_blk.numitems;i++){
    	
        if(key==(main_blk.getitem(i))->person){
            main_blk.remove(i+1);//改
     if(main_blk.numitems>0){
            maxdata=main_blk.getitem(main_blk.numitems-1)->person;}
            else{
            	maxdata=0;
			}
            flg=1;
            break;
        }
    }if(flg==0){
        cout<<"error:item not found"<<endl;
        return NULL;
    }
    if(main_blk.numitems<max_main/2){
         block* p=new block;    	
         p=merge();
         return p;//再进行deleteKEY判断是否要加p的key
    }
	return NULL;//啥都不用做
}

//REORGANIZATION:SPLITTING
 block* block::split(){
    block* p=new block;
    int j;
    int temp=main_blk.numitems;
    for(j=temp/2;j<temp;j++){
         (p->main_blk).append(main_blk.getitem(j));/////删除指针会不会有什么问题
         
     }main_blk.numitems/=2;j=(p->main_blk).numitems-1;
     p->maxdata=((p->main_blk).getitem(j))->person;
     j=main_blk.numitems-1;
     maxdata=((main_blk).getitem(j))->person;
    //  for(int i=0;i<overflow_blk.numitems;i++){
    //      if(overflow_blk.getitem(i)->person>maxdata){
    //          maxdata=overflow_blk.getitem(i)->person;
    //      }
    //  }for(int i=0;i<p->overflow_blk.numitems;i++){
    //      if(p->overflow_blk.getitem(i)->person>p->maxdata){
    //          p->maxdata=p->overflow_blk.getitem(i)->person;
    //      }
    //  }
     return p;
}

//REORGANIZATION:MERGING
 block* block::merge(){//左右，统一合并到左边   4种情况 返回分裂的pointer，或NULL用于改最大值
 	if(backP!=NULL){
        block* p=new block;
		p=backP;///
        int i=0;
        int temp=overflow_blk.numitems;
		int max=maxdata;
        for(i=0;i<temp;i++){
        	 (main_blk).append(overflow_blk.getitem(overflow_blk.numitems-1));
        	 overflow_blk.numitems--;
		}	
		sorting(this);
		if(main_blk.numitems<max_main/2){
			temp=main_blk.numitems;
		while((p->main_blk).numitems<max_main&&i<temp){
            (p->main_blk).append(main_blk.getitem(main_blk.numitems-1));
            main_blk.numitems--;
            i++;
        }sorting(p);	      
        if(main_blk.numitems!=0){//��Ҫ���� 
        temp=p->main_blk.numitems;
        int j;
    	for(j=0;j<temp/2;j++){
         (main_blk).append(p->main_blk.getitem(j));/////删除指针会不会有什么问题
         
     }for(j=0;j<temp/2;j++){
         p->main_blk.remove(1);
         
     }
	 sorting(this);
	 j=(p->main_blk).numitems-1;
     	p->maxdata=((p->main_blk).getitem(j))->person;
    	j=main_blk.numitems-1;
     	maxdata=((main_blk).getitem(j))->person;
     	//overflow
     	temp=p->overflow_blk.numitems;
     	for(i=0;i<temp;i++){
     		if(p->overflow_blk.getitem(p->overflow_blk.numitems-1)->person<maxdata){
                	(overflow_blk).append(p->overflow_blk.getitem(p->overflow_blk.numitems-1));
                	p->overflow_blk.numitems--;
          }
		 }
    	return NULL;
    	}else{
    		p->frontP=frontP;
    		if(frontP!=NULL){
			frontP->backP=p;} 
			maxdata=max;
    		return this; 
		} 
		} else{
			return NULL;
		}}
		
		 
            

   }

//REORGANIZATION:SORTING
 void block::sorting(block* p){//对p的main排序
    int i,j;
    
    for(i=1;i<(p->main_blk).numitems;i++){
        for(j=0;j<(p->main_blk).numitems-i;j++){
            patient* t=new patient;
            //cout<<"shishi"<<j<<endl;
            
            if(((p->main_blk).getitem(j))->person>((p->main_blk).getitem(j+1))->person){
                t=(p->main_blk).getitem(j);
                (p->main_blk).setitem(j,(p->main_blk).getitem(j+1));
                (p->main_blk).setitem(j+1,t);
            }
        }
    }//cout<<(p->main_blk).numitems<<endl;
    
    p->maxdata=((p->main_blk).getitem(p->main_blk.numitems-1))->person;
}

//RETRIEVAL
 patient* block::retrieval(int key){
    int i;
    int flg=0;
    for(i=0;i<overflow_blk.numitems;i++){
        if(key==(overflow_blk.getitem(i))->person){
            return overflow_blk.getitem(i);
        }
    }for(i=0;i<main_blk.numitems;i++){
        if(key==(main_blk.getitem(i))->person){
           return main_blk.getitem(i);
        }
    }
        return NULL;
    }


//FOR TESTING
void block::print(void){
    cout<<"main_blk"<<endl;
	for(int i=0;i<main_blk.numitems;i++){
		cout<<main_blk.getitem(i)->person<<endl;
	}
    cout<<"overflow_blk"<<endl;
	for(int i=0;i<overflow_blk.numitems;i++){

		cout<<overflow_blk.getitem(i)->person<<endl;
	}
	
}

//FOR REPORTING
//EX2
void block::report_(ofstream& ofile){

	for(int i=0;i<main_blk.numitems;i++){
		ofile<<"TREATED PATIENT"<<endl;
		ofile<<"his/her ID is"<<main_blk.getitem(i)->person<<endl;
		ofile<<"his/her profession is"<<main_blk.getitem(i)->profession<<endl;
		ofile<<"his/her age is"<<main_blk.getitem(i)->age<<endl;
		ofile<<"his/her risk is"<<main_blk.getitem(i)->risk<<endl;
		ofile<<"his/her waiting time is"<<main_blk.getitem(i)->treat_day-main_blk.getitem(i)->registration<<endl;
	}
	for(int i=0;i<overflow_blk.numitems;i++){
		ofile<<"TREATED PATIENT"<<endl;
	    ofile<<"his/her ID is"<<overflow_blk.getitem(i)->person<<endl;
		ofile<<"his/her profession is"<<overflow_blk.getitem(i)->profession<<endl;
		ofile<<"his/her age is"<<overflow_blk.getitem(i)->age<<endl;
		ofile<<"his/her risk is"<<overflow_blk.getitem(i)->risk<<endl;
		ofile<<"his/her waiting time is"<<overflow_blk.getitem(i)->treat_day-overflow_blk.getitem(i)->registration<<endl;	
	}
}
