#include "alist.cpp"

#include <vector>
#include "bplus_1.cpp" 

/*
CLUSTER ANALYSIS FOR EX4
*/

 


vector<patient*>* modify_arr(vector<patient*>* s1,B_plus_tree* Mytree){
	int flg=0;
 	vector<patient*> pa; 
 	int i;
 	int u;
 	leafnode* p=new leafnode;
 	p=Mytree->leafhead;
	 int cnt=-1;
    while(p!=NULL){
		
    	for(int i=0;i<p->key_num;i++){
		
    		for(int j=0;j<p->data[i]->main_blk.numitems;j++){
				
    			pa.push_back(p->data[i]->main_blk.getitem(j));
		
					
			}for(int j=0;j<p->data[i]->overflow_blk.numitems;j++){
				
				patient*a=new patient;
				a=p->data[i]->overflow_blk.getitem(j);
    			pa.push_back(a);   
			}
			
	
    		
		}
		p=p->rightSibling;
	
	}

	for(i=0;i<pa.size();i++){
		
	for(u=0;u<(*s1).size();u++){
		// cout<<"wrc"<<endl;
	
		// cout<<"s1 has person id"<<(*s1)[u]->person<<endl;
        // cout<<"pa has person id"<<pa[i]->person<<endl;
		if(pa[i]->person==(*s1)[u]->person){
			(*s1)[u]->treatment=(pa[i]->treatment);
             flg=1;
		
		}
	}	
	
	}
	//cout<<"the person is "<<(*s1)[0]<<endl;
	
	return s1;
	

}

       AList** divede_group (int n,vector<patient*>* strArray1,vector<patient*>* strArray2,vector<patient*>* strArray3){
		  
	int u,flg;
	int i;
	int x,x1,y,y1;
    int ever=0;
	AList** arr;
	AList* pt=new AList;
	AList* pr=new AList;
	arr=new AList* [40];// 20 list that represent the gourp(some is empty)
	for(u=0;u<=39;u++){
		arr[u]=0;
	}
	
	for(i=0;i<strArray1->size();i++){
		ever=0;
		u=0;
		x=(*strArray1)[i]->address_x;
		y=(*strArray1)[i]->address_y;
	    while(arr[u]!=0){
			flg=0;    //to check whether it satisfy the condition ,if yes ,keep flg=0
	    	pt=arr[u];
	    	for(int w=0;w<pt->numitems;w++){
	    		x1=pt->getitem(w)->address_x;
	    		y1=pt->getitem(w)->address_y;
	    		if(((x1-x)*(x1-x)+(y1-y)*(y1-y))>n*n){
	    			flg=1;
	    			break;
				}
			}
	    	
			if(flg==0){
				(*pt).append((*strArray1)[i]);
				ever=1;
				
				break;
			}
			
			u++;
    }
	if(ever==0){
		
		u=0;
		while(arr[u]!=0){
		u++;
	}
           
		   pr=new AList;
			arr[u]=pr;
			(*pr).append((*strArray1)[i]);
	}   
	
	}         // never have been added ever
	for(i=0;i<strArray2->size();i++){
		// cout<<"Array2222222222222222"<<endl;
		ever=0;
		u=0;
		x=(*strArray2)[i]->address_x;
		y=(*strArray2)[i]->address_y;
		
	    while(arr[u]!=0){
			flg=0;    //to check whether it satisfy the condition ,if yes ,keep flg=0
			
	    	pt=arr[u];
	    	for(int w=0;w<pt->numitems;w++){
	    		x1=pt->getitem(w)->address_x;
	    		y1=pt->getitem(w)->address_y;
	    		if(((x1-x)*(x1-x)+(y1-y)*(y1-y))>n*n){
	    			flg=1;
	    			break;
				}
			}
	    	
			if(flg==0){
				(*pt).append((*strArray2)[i]);
				ever=1;
				
				break;
			}
			
			u++;
    }
	if(ever==0){
		u=0;
		while(arr[u]!=0){
		u++;
	}
	      pr=new AList;
          
			arr[u]=pr;
			(*pr).append((*strArray2)[i]);
	}             // never have been added ever
	}
	for(i=0;i<strArray3->size();i++){
		// cout<<"Array333333"<<endl;
		ever=0;
		u=0;
		x=(*strArray3)[i]->address_x;
		y=(*strArray3)[i]->address_y;
		
	    while(arr[u]!=0){
			flg=0;    //to check whether it satisfy the condition ,if yes ,keep flg=0
			
	    	pt=arr[u];
	    	for(int w=0;w<pt->numitems;w++){
	    		x1=pt->getitem(w)->address_x;
	    		y1=pt->getitem(w)->address_y;
	    		if(((x1-x)*(x1-x)+(y1-y)*(y1-y))>n*n){
	    			flg=1;
	    			break;
				}
			}
	    	
			if(flg==0){
				(*pt).append((*strArray3)[i]);
				ever=1;
				
				break;
			}
			
			u++;
    }
	if(ever==0){
		u=0;
		while(arr[u]!=0){
		u++;
	}
	pr= new AList;
         
			arr[u]=pr;
			(*pr).append((*strArray3)[i]);
	}   
	          // never have been added ever
	}
	
	   
	     i=0;
		while(arr[i]!=0){
			
			i++;
		}
	
	
	return arr;
}
 
 int recieved_percent(AList** arr,B_plus_tree* Mytree,int treat){
 	int i,cnt;
 	double per;
 	int num=0;
	
   for(int u=0;u<40;u++){
 	while(arr[u]!=0){
		// cout<<"u is "<< endl;
 		cnt=0;
		//cout<<"size is" <<(*arr[u]).numitems<<endl;
 		for(i=0;i<(*arr[u]).numitems;i++){
			 cout<<" In the group "<<u+1<<" the "<<i+1<< "th person "<<" has treatment "<< (*arr[u]).getitem(i)->treatment<<endl;
 		if((*arr[u]).getitem(i)->treatment==treat){
 			cnt++;
			
			// cout<<"entry"<<u<< "the person "<<i<<"counted"<<endl;
		 }	
		 }
		
		 
		 double a=cnt;
		 double b=arr[u]->numitems;
		 per=a/b;

		
		 if (per>=0.2){
		 	num++;
		 }
		 u++;
	 }
	 
	 }
	 return num;
	 
 	
 	
 }
	
int find_percent(int treatment,int n,vector<patient*>* v1,vector<patient*>*v2,vector<patient*>* v3,B_plus_tree* Mytree)
{
 	vector<patient*>* p1= new vector<patient*>;
 	vector<patient*>* p2= new vector<patient*>;
 	vector<patient*>* p3= new vector<patient*>;
 	p1=modify_arr(v1,Mytree);
 	p2=modify_arr(v2,Mytree);
 	p3=modify_arr(v3,Mytree);
 	AList** pt=new AList*;
 	pt=divede_group(n,p1,p2,p3);
 	int result=recieved_percent(pt,Mytree,treatment);
	
 	return result;
 }


	
