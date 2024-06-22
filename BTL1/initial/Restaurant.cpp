#include "main.h"
class Queue{
	public:
	Restaurant::customer *head;
	Restaurant::customer *tail;
	int count;

	public:
	Queue(){
		head=tail=NULL;
		count=0;
	}
	void push(Restaurant::customer *cus){
		if( count>=MAXSIZE ) return ;
		else if( head==NULL ) head=tail=cus;
		else{
			cus->prev=tail;
			tail->next=cus;
			tail=cus;
		}
		++count;
	}
	void push(string name,int enegry){
		Restaurant::customer *cus=new Restaurant::customer(name,enegry,NULL,NULL);
		if( head==NULL ) head=tail=cus;
		else{
			cus->prev=tail;
			tail->next=cus;
			tail=cus;
		}
		++count;
	}
	void pop(){
		if( head==NULL ) return ;
		else if( head==tail ) head=tail=NULL;
		else{
			head=head->next;
			head->prev=NULL;
		}
		--count;
	}
	void pop(string name){
		Restaurant::customer *cus=head;
		while( cus->name!=name ){
			cus=cus->next;
		}
		if( cus==head ) pop();
		else if( cus==tail ){
			tail=tail->prev;
			tail->next=NULL;
			--count;
		}
		else{
			cus->prev->next=cus->next;
			cus->next->prev=cus->prev;
			--count;
		}
		delete cus;
	}
	Restaurant::customer* front(){
		return this->head;
	}
	bool check(string name){
		Restaurant::customer *k=head;
		while( k!=NULL ){
			if( k->name==name ) return true;
			k=k->next;
		}
		return false;
	}
	bool getnumberinlist(Queue listname,string name1,string name2){
		Restaurant::customer *cus=listname.head;
		while( cus!=NULL ){
			if( cus->name==name1 ) return false ;
			else if( cus->name==name2 ) return true ;
			cus=cus->next; 
		}
	}
	Restaurant::customer* setplus(Restaurant::customer *cus,int i,Restaurant::customer *pos){
		if( i>0 ){
			while( i>0 && cus->next!=pos->next ){
				cus=cus->next;
			    --i;
		    }
		}
		else{
			i=abs(i);
			while( i>0 && cus->prev!=NULL ){
				cus=cus->prev;
				--i;
			}
		}
		return cus;
	}
	void inssort(Restaurant::customer *k,int size,Restaurant::customer *incr,int incrpos,int &n,Queue listname,Restaurant::customer *pos){
		int i=incrpos;
		for( Restaurant::customer *cus=incr;i<size;cus=setplus(cus,1,pos) ){
			Restaurant::customer *k2=cus;
			int j=i;
			for( ;j>=incrpos;k2=setplus(k2,-incrpos,pos) ){
				Restaurant::customer *k3=k2;
				k3=setplus( k3,-incrpos,pos );
				if( abs(k3->energy)>abs(k2->energy) ) break ;
				else{
					if( abs(k3->energy)<abs(k2->energy) || getnumberinlist(listname,k3->name,k2->name)==true  ){
						int tempe=k3->energy;
						string tempn=k3->name;
						k3->energy=k2->energy;
						k3->name=k2->name;
						k2->energy=tempe;
						k2->name=tempn;
						++n;
					}
					j=j-incrpos;
				}
			}
			++i;
		}
	}
	int shellsort(Queue listname){
		if( this->count<=1 ) return 0;
		Restaurant::customer *cus=this->head;
		Restaurant::customer *pos=cus;
		int size=0;
		int i=1;
		while( cus!=NULL ){
			if( abs(cus->energy)>abs(pos->energy) ){ 
				pos=cus;
				size=i;
			}	
			else if( abs(cus->energy)==abs(pos->energy) && getnumberinlist(listname,cus->name,pos->name)==true ){ 
				pos=cus;
				size=i;
			}
			++i;
			cus=cus->next;
		}
		if( size<=1 ) return 0;
		if( size==2 ){
			int n=0;
			cus=this->head;
			if( (abs(cus->energy)<abs(cus->next->energy)) || (abs(cus->energy)==abs(cus->next->energy) && getnumberinlist(listname,cus->name,cus->next->name)==true ) ){
				string tempn=cus->name;
				int tempe=cus->energy;
				cus->name=cus->next->name;
				cus->energy=cus->next->energy;
				cus->next->name=tempn;
				cus->next->energy=tempe;
				return 1;
			}
			return 0;
		}
		int n=0;
		cus=this->head;
	    i=size/2;
		for(cus=setplus(cus,i,pos);i>2;){
			Restaurant::customer *k=this->head;
			int j=0;
			for(;j<i;k=setplus(k,1,pos)){
				inssort(k,size-j,cus,i,n,listname,pos);
				++j;
			}
		    cus=setplus(cus,-(i-i/2),pos);
			i=i/2;
		}
		inssort(this->head,size,this->head->next,1,n,listname,pos);
		return n ;
	}
};
class imp_res : public Restaurant
{
	private:
	customer *head;
	customer *tail;
	customer *now;
	int count;
	Queue listname;
	Queue queue;
	public:	
		imp_res(){
			this->head=NULL;
			this->tail=NULL;
			this->now=NULL;
			this->count=0;
		};
		void addbehind(customer *cus){
			if( now==tail ){
			    now->next=cus;
				cus->prev=now;
				tail=now=cus;
				cus->next=head;
				head->prev=cus;
			}
			else{
				cus->next=now->next;
				now->next->prev=cus;
				now->next=cus;
				cus->prev=now;
				now=cus;
			}
		}
		void addfront(customer *cus){
			if( now==head ){
				now->prev=cus;
				cus->next=now;
				head=now=cus;
				cus->prev=tail;
				tail->next=cus;
			}
			else{
				cus->prev=now->prev;
				now->prev->next=cus;
				cus->next=now;
				now->prev=cus;
				now=cus;
			}			
		}
		bool check( string name ){
			customer *cus=head;
			for(int i=0;i<count;++i){
				if( cus->name==name ) return true;
				cus=cus->next;
			}
			return false;
		}
		void RED(string name, int energy)
		{ 
			customer *cus = new customer (name, energy, nullptr, nullptr);
			if( cus->energy == 0 || listname.check( name )==true ) return ; //energy=0 khong nhan khach
			else if( this->count == MAXSIZE ){
				if( this->queue.count==MAXSIZE ) return ;
				else{
					listname.push( cus->name, cus->energy );
					queue.push(cus);
				}
			}
			else if( this->count == 0 ){   //khach dau tien 
				this->head=this->tail=this->now=cus;
				++this->count;
				listname.push(cus->name, cus->energy );
			}
			else if( this->count >= MAXSIZE/2 ){  //them khach khi lon hon hoac bang maxsize/2
			    ++this->count;
				listname.push(cus->name, cus->energy );
				customer *k=now;
				customer *rescus=NULL;
				int res=0;
				for(int i=0;i<this->count-1;++i){
					if( abs( cus->energy - k->energy )>res ){
						rescus=k;
						res=abs( cus->energy - k->energy );
					}
					k=k->next;
				}
				if( cus->energy - rescus->energy < 0 ){
					now=rescus;
					addfront( cus );
				}
				else{
					now=rescus;
					addbehind( cus );
				}
			} 
			else{   //them khach khi nho hon maxsize/2
				++this->count; //them so luong vao nha hang
				listname.push(cus->name, cus->energy );
				if( cus->energy >= this->now->energy ) addbehind(cus);
				else addfront(cus); 
			}
		}
		void RED1(customer *cus)
		{ 
		    if( this->count == 0 ){   //khach dau tien 
				this->head=this->tail=this->now=cus;
				++this->count;
			}
			else if( this->count >= MAXSIZE/2 ){  //them khach khi lon hon hoac bang maxsize/2
			    ++this->count;
				customer *k=now;
				customer *rescus=NULL;
				int res=0;
				for(int i=0;i<this->count-1;++i){
					if( abs( cus->energy - k->energy )>res ){
						rescus=k;
						res=abs( cus->energy - k->energy );
					}
					k=k->next;
				}
				if( cus->energy - rescus->energy < 0 ){
					now=rescus;
					addfront( cus );
				}
				else{
					now=rescus;
					addbehind( cus );
				}
			} 
			else{   //them khach khi nho hon maxsize/2
				++this->count; //them so luong vao nha hang
				if( cus->energy >= this->now->energy ) addbehind(cus);
				else addfront(cus); 
			}
		}
		void deletecustomer(string name){
			customer *k=this->head;
			while( k->name!=name ){
				k=k->next;
			}
			if( this->count==1 ){
				this->count=0;
				this->head=this->tail=this->now=NULL;
				return ;
			}
			else if( k==this->head ){
				this->head=k->prev->next=k->next;
				k->next->prev=tail;
			}
			else if( k==this->tail ){
				k->prev->next=head;
				this->tail=k->next->prev=k->prev;
			}
			else{
				k->prev->next=k->next;
				k->next->prev=k->prev;
			}
			if( k->energy>0 ) now=k->next;
			else now=k->prev;
			--this->count;
			delete k;
		}
		void BLUE(int num)
		{
			if( num>=MAXSIZE || num>=this->count ) num=this->count;
			while( num>0 ){
				Restaurant::customer *cus=listname.head;
				while( !check(cus->name) ){
					cus=cus->next;
				}
				deletecustomer( cus->name );
				listname.pop( cus->name );
				--num;
			}
			if( this->queue.count<=0 ) return ;
			else{
				while( count<MAXSIZE && queue.count>0 ){
					customer *cus=queue.front();
					queue.pop();
					RED1( cus );
				}
			}
		}
		void PURPLE()
		{
			if( queue.count <=1 ) return ;
			int n=queue.shellsort( listname );
			BLUE( n%MAXSIZE );
		}
		void REVERSAL()
		{
			if( this->count<=1 ) return ;
			Restaurant::customer *h=now;
			string nametarget=h->name;
			Queue listduong;
			Queue listam;
			do{
				if( h->energy>0 ) listduong.push(h->name,h->energy);
				else listam.push(h->name,h->energy);
				h=h->prev;
			} while( h!=now ) ;
			h=now;
			Restaurant::customer *tailduong=listduong.tail;
			Restaurant::customer *tailam=listam.tail;
			do{
				if( h->energy>0 ){
					h->name=tailduong->name;
					h->energy=tailduong->energy;
					Restaurant::customer *temp=tailduong;
					tailduong=tailduong->prev;
					delete temp;
				}
				else{
					h->name=tailam->name;
					h->energy=tailam->energy;
					Restaurant::customer *temp=tailam;
					tailam=tailam->prev;
					delete temp;
				}
				h=h->prev;
			}while( h!=now ) ;
			while( now->name!=nametarget ) now=now->next;
		}
		void UNLIMITED_VOID()
		{
			if( this->count<4 ) return ;
			Restaurant::customer *k=this->now;
			Restaurant::customer *i=NULL;
			Restaurant::customer *j=NULL;
			Restaurant::customer *pmin=NULL;
			int min=100000000;
			int time=0;
			int size=0;
			while( time<this->count ){
				int tempmin=0;
				int tempsize=0;
				Restaurant::customer *p=k;
				Restaurant::customer *tempn=p;
				for(int n=0;n<this->count;++n){
					tempmin+=p->energy;
					++tempsize;
					if( tempn->energy>p->energy ) tempn=p;
					if( tempmin<=min && n>=3 ){
						if( (tempmin<min) || (tempmin==min && tempsize>=size) ){
							i=k;
							j=p;
							min=tempmin;
							size=tempsize;
							pmin=tempn;
						}
					}
					p=p->next;
				}
				k=k->next;
				++time;
			}
			k=pmin;
			while( k!=j ) {
				k->print();
				k=k->next;
			}
			k->print();
		    k=i;
			while( k!=pmin ){
				k->print();
				k=k->next;
			}
		}
		void DOMAIN_EXPANSION()
		{
			if( listname.count <=1 ) return ;
			int sumofduong=0;
			int sumofam=0;
			Restaurant::customer *k=listname.head;
			while( k!=NULL ){
				if( k->energy>0 ) sumofduong+=k->energy;
				else sumofam+=k->energy;
				k=k->next;
			}			
			k=listname.head;
			if( abs(sumofduong)>=abs(sumofam) ){
				while( k!=NULL ){
					if( k->energy<0 ){
					    if( check(k->name)==true ) deletecustomer(k->name);
					    else queue.pop(k->name);
					}
					k=k->next;
				}
				k=listname.tail;
				while( k!=NULL ){
					if( k->energy<0 ){
						k->print();
						Restaurant::customer *h;
						if( k==listname.head ){
							h=k;
							if( listname.head==listname.tail ) listname.head=listname.tail=NULL;
							else{ 
								listname.head=k=k->next;
							    listname.head->prev=NULL;
							}
							--listname.count;
						}
		                else if( k==listname.tail ){
							h=k;
			                k=listname.tail=listname.tail->prev;
			                listname.tail->next=NULL;
			                --listname.count;
							delete h;
							continue;
		                }
		                else{
							h=k;
			                h->prev->next=h->next;
			                h->next->prev=h->prev;
			               --listname.count;
		                }
						k=k->prev;
		                delete h;
					}
					else k=k->prev;
				}
			}
			else{
				while( k!=NULL ){
					if( k->energy>0 ){
					    if( check(k->name)==true ) deletecustomer(k->name);
					    else queue.pop(k->name);
					}
					k=k->next;
				}
				k=listname.tail;
				while( k!=NULL ){
					if( k->energy>0 ){
						k->print();
						Restaurant::customer *h;
						if( k==listname.head ){
							h=k;
							if( listname.head==listname.tail ) listname.head=listname.tail=NULL;
							else{ 
								listname.head=k=k->next;
							    listname.head->prev=NULL;
							}
							--listname.count;
						}
		                else if( k==listname.tail ){
							h=k;
			                k=listname.tail=listname.tail->prev;
			                listname.tail->next=NULL;
			                --listname.count;
							delete h ;
							continue;
		                }
		                else{
							h=k;
			                h->prev->next=h->next;
			                h->next->prev=h->prev;
			               --listname.count;
		                }
						k=k->prev;
		                delete h;
					}
					else k=k->prev;
				}			
			}
			if( this->queue.count<=0 ) return ;
			else{
				while( count<MAXSIZE && queue.count>0 ){
					customer *cus=queue.front();
					queue.pop();
					RED1( cus );
				}
			}
		}
		void LIGHT(int num)
		{
			if( num==0 ){
				if( queue.count<=0 ) return ;
				Restaurant::customer *h=queue.head;
				while( h!=NULL ){
					h->print();
					h=h->next;
				}
			}
			else{
				if( count<=0 ) return ;
				Restaurant::customer *h=now;
				int time=this->count;
				while( time>0 ){
					h->print();
					if( num>0 ) h=h->next;
					else h=h->prev;
					--time;
				}
			}
		}
};