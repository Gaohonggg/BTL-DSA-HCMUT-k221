#include "main.h"
int MAXSIZE;
struct cmp{
    bool operator() (pair<int,char> p1,pair<int,char> p2){
        if( p1.first==p2.first ){
            if( (islower(p1.second)&&islower(p2.second)) || (isupper(p1.second)&&isupper(p2.second)) ) return p1.second<p2.second;
            else if( islower(p1.second) ) return true ;
            else return false ;
        }
        return p1.first<p2.first;
    }
};
map<char,int> getfofname(string name){
	map<char,int> list;
	int size=name.size();
    for( int i=0;i<size;++i ){
        if( list.find(name[i])==list.end() ) list[name[i]]=1;
        else list[name[i]]+=1; 
    }
	return list;
}
vector<pair<int,char>> casesar(map<char,int> list){
    map<char,int> newlist;
    for( auto k : list ){
        char temp;
        if( k.first>=65 && k.first<=90 ) temp=(k.first-65+k.second)%26+65;
        else temp=(k.first-97+k.second)%26+97;
        if( newlist.find(temp)==newlist.end() ) newlist[temp]=k.second;
        else newlist[temp]+=k.second;
    }
    vector<pair<int,char>> vec;
    for( auto k : newlist ){
        vec.push_back({k.second,k.first});
    }
    sort( vec.begin(),vec.end(),cmp() );
	return vec;
}
class Huffnode{
    public:
    virtual ~Huffnode(){}
    virtual int weight()=0 ;
    virtual bool isLeaf()=0 ;
    virtual char val()=0 ;
    virtual Huffnode* nodeleft()=0;
    virtual Huffnode* noderight()=0;
    virtual int getlabel()=0;
    virtual void updateleft(Huffnode *p)=0;
    virtual void updateright(Huffnode *p)=0;
	virtual void updatelabel(int i)=0;
};
class LeafNode : public Huffnode{
    private:
    char data;
    int wgt;
    public:
    LeafNode(int data,int freq){
        this->data=data;
        this->wgt=freq;
    }
    int weight(){ return this->wgt; }
    char val(){ return this->data; }
    bool isLeaf(){ return true; }
    int getlabel(){ return 0; }
    Huffnode* nodeleft(){ return NULL; }
    Huffnode* noderight(){ return NULL; }
    void updateleft(Huffnode *p){ return; }
    void updateright(Huffnode *p){ return; }
    void updatelabel(int i){ return; }
};
class IntNode : public Huffnode{
    private:
    Huffnode *left;
    Huffnode *right;
    int wgt;
    int label;
    public:
    IntNode(Huffnode *l,Huffnode *r,int label){
        this->left=l;
        this->right=r;
        this->wgt=l->weight() + r->weight();
        this->label=label;
    }
    int weight(){ return this->wgt; }
    char val(){ return '\0';}
    bool isLeaf(){ return false; }
    int getlabel(){ return label; }
    Huffnode* nodeleft(){ return this->left; }
    Huffnode* noderight(){ return this->right; }
    void setleft(Huffnode *b){ this->left=b; }
    void setright(Huffnode *b){ this->right=b; }
    void updateleft(Huffnode *p){ this->left=p; }
    void updateright(Huffnode *p){ this->right=p; }
    void updatelabel(int i){ this->label=i; }
};
class Hufftree{
    private:
    Huffnode *root;
    public:
    Hufftree(){ root=NULL; }
    Hufftree(char val,int freq){ this->root=new LeafNode(val,freq); }
    Hufftree(Hufftree *l,Hufftree *r,int i){
        this->root=new IntNode(l->getroot(),r->getroot(),i);
    }
    char getdata(){ return root->val(); }
    Huffnode* getroot(){ return this->root; }
    int getweight(){ return this->root->weight(); }
    bool check(){ return this->root->isLeaf(); }
    char getleft(){ return this->root->nodeleft()->val(); }
    char getright(){ return this->root->noderight()->val(); }
    int getlabel(){ return this->root->getlabel(); }
    int getheight(Huffnode *p){
        if( p==NULL ) return 0;
        return max( getheight( p->nodeleft() ), getheight( p->noderight() ) ) + 1;
    }
    Huffnode* rotateright(Huffnode *p,int i){
        Huffnode *temp=p->nodeleft();
        p->updateleft( temp->noderight() );
        temp->updateright( p );
		temp->updatelabel(i);
        return temp;
    }
    Huffnode* rotateleft(Huffnode *p,int i){
        Huffnode *temp=p->noderight();
        p->updateright( temp->nodeleft() );
        temp->updateleft( p );
		temp->updatelabel(i);      
        return temp;
    }
    Huffnode* balancepreorder(Huffnode *temp,int bal){
        if( bal>1 && getheight(temp->nodeleft()->nodeleft()) >= getheight(temp->nodeleft()->noderight()) ){ 
            temp=rotateright( temp,0 );
        }
        else if( bal>1 && getheight(temp->nodeleft()->nodeleft()) < getheight(temp->nodeleft()->noderight())){
            temp->updateleft( rotateleft(temp->nodeleft(),0) );
            temp=rotateright( temp,0 );
        }
        else if( bal<-1 && getheight(temp->noderight()->nodeleft())<=getheight( temp->noderight()->noderight()) ){ 
            temp=rotateleft( temp,0 );
        }
        else if( bal<-1 && getheight(temp->noderight()->nodeleft())>getheight( temp->noderight()->noderight()) ){
            temp->updateright( rotateright(temp->noderight(),0) );
            temp=rotateleft( temp,0 );
        }
		return temp;
    }
	Huffnode* checkpreorder(Huffnode *root,int &time){
		if( root==NULL ) return NULL;
		int bal= getheight( root->nodeleft() ) - getheight( root->noderight() );
		if( (bal>1 || bal<-1) && time==1 ){
			--time;
			root=balancepreorder(root,bal);
		}
		root->updateleft( checkpreorder(root->nodeleft(),time) );
	    root->updateright( checkpreorder(root->noderight(),time)  );
		return root;
	}
    Hufftree* buildtree(vector<pair<int,char>> vec){
        if( vec.size()==1 ){
            Hufftree *p=new Hufftree(vec.front().second,vec.front().first);
            return p;
        }
        priority_queue<Hufftree*,vector<Hufftree*>,cmppq> pq;
        for( auto k : vec ){
            Hufftree *p=new Hufftree(k.second,k.first);
            pq.push(p);
        }
        int i=0;
        while( pq.size()>1 ){
            Hufftree *l=pq.top(); pq.pop();
            Hufftree *r=pq.top(); pq.pop();
            Hufftree *temp=new Hufftree(l,r,i);
            int bal= getheight( temp->getroot()->nodeleft() ) - getheight( temp->getroot()->noderight() );
            if( bal>1 && getheight(temp->getroot()->nodeleft()->nodeleft()) >= getheight(temp->getroot()->nodeleft()->noderight()) ) temp->root=rotateright( temp->root,i );
            else if( bal>1 && getheight(temp->getroot()->nodeleft()->nodeleft()) < getheight(temp->getroot()->nodeleft()->noderight())){
                temp->getroot()->updateleft( rotateleft(temp->getroot()->nodeleft(),i) );
                temp->root=rotateright( temp->root,i );
            }
            else if( bal<-1 && getheight(temp->getroot()->noderight()->nodeleft())<=getheight( temp->getroot()->noderight()->noderight()) ) temp->root=rotateleft( temp->root,i );
            else if( bal<-1 && getheight(temp->getroot()->noderight()->nodeleft())>getheight( temp->getroot()->noderight()->noderight()) ){
                temp->getroot()->updateright( rotateright(temp->getroot()->noderight(),i) );
                temp->root=rotateleft( temp->root,i );
            }
            if( bal>1 || bal<-1 ){
                int time=1;
                temp->root=checkpreorder(temp->getroot(),time);
				if( time==0 ){
					time=1;
				    temp->root=checkpreorder(temp->getroot(),time);
				}
            }
            ++i;
            pq.push(temp);
        }
        return pq.top();
    }
	void inorder(Huffnode *root){
		if( root==NULL ) return ;
		inorder( root->nodeleft() );
		if( root->isLeaf() ) cout<< root->val() <<endl;
		else cout<< root->weight() <<endl;
		inorder( root->noderight() );
	}
	~Hufftree(){}
    class cmppq
    {
        public:
        friend class Hufftree;
        bool operator() (Hufftree *p1,Hufftree *p2){
            if( p1->getweight()==p2->getweight() ){
                if( p1->getdata()=='\0' && p2->getdata()=='\0' ) return p1->getlabel()>p2->getlabel() ;
                else if( p1->getdata()=='\0' || p2->getdata()=='\0' ){
                    if( p1->getdata()=='\0' ) return true ;
                    else return false ;
                }
                else if( (islower(p1->getdata())&&islower(p2->getdata())) || (isupper(p1->getdata())&&isupper(p2->getdata())) ) return p1->getdata()>p2->getdata();
                else if( islower(p1->getdata()) ) return false ;
                else return true ;
            }
            return p1->getweight()>p2->getweight(); 
        };
    };
};
void getallstring(Huffnode *root,map<char,string> &m,string s){
    if( root==NULL ) return ;
    if( root->isLeaf() ){
        if( m.find( root->val() )==m.end() ) m[ root->val() ]=s; 
    }
    string temp=s;
    getallstring(root->nodeleft(),m,temp+"0");
    temp=s;
    getallstring(root->noderight(),m,temp+"1");
}
int trans(vector<pair<int,char>> vec,Hufftree *tree,string name,map<char,int> fname){
    if( vec.size()==1 ) return 0;
    map<char,string> m;
    getallstring(tree->getroot(),m,"");
    string s="";
	int i=name.size();
	while( s.size()<10 && i>0  ){
		char a=name[i-1];
        if( a>=65 && a<=90 ) a=(a-65+fname[a])%26+65;
        else a=(a-97+fname[a])%26+97;
		s= m[a] + s;
		--i; 
	}
    if( s.size()>10 ) s.erase(0,s.size()-10);
    reverse(s.begin(),s.end());
    int binary=stoi(s);
    int deci=0;
    int temp=0;
    int remainder;
    while( binary!=0 ){
        remainder=binary%10;
        binary/=10;
        deci+=remainder*pow(2,temp);
        ++temp;
    }
	//cout<< deci <<endl;
    return deci;
}
struct customer {
	string name;
	map<char,int> fname;
	vector<pair<int,char>> freq;
	int result;
	customer *left;
	customer *right;
	Hufftree *tree;
	~customer(){
		left=right=NULL;
		tree->~Hufftree();	
	}
};
customer* createcus(string name){
	customer *cus=new customer();
	cus->name=name;
	cus->fname=getfofname(name);
	if( cus->fname.size()<3 ) return NULL;
	cus->freq=casesar(cus->fname);
	cus->tree=cus->tree->buildtree(cus->freq);
	if( cus->tree==NULL ) return NULL;
	cus->result=trans(cus->freq,cus->tree,name,cus->fname);
	cus->left=cus->right=NULL;
	return cus;
}
class Gojo{
	private:
	vector<customer *> h;
	vector<queue<customer *>> timeline;
	public:
	Gojo(int size){
		h.assign(size,NULL);
		timeline=vector<queue<customer *>>(size); 
	}
	customer* pushbst(customer *root,customer *cus){
		if( root==NULL ) return cus;
		else if( root->result>cus->result ) root->left=pushbst(root->left,cus);
		else root->right=pushbst(root->right,cus);
		return root;
	}
	void add(customer *cus,int id){
		customer *root=h[id-1];
		h[id-1]=pushbst(root,cus);
		timeline[id-1].push(cus);
	}
	customer* find(customer *root){
		while( root->left!=NULL ){ root=root->left; }
		return root;
	}
	customer *removecus(customer *root,customer *cus){
		if( root==NULL ) return NULL;
		if( root->result>cus->result ) root->left = removecus(root->left,cus);
		else if( root->result<cus->result ) root->right = removecus(root->right,cus);
		else{
			//if( root!=cus ) root->right=removecus(root->right,cus);
			
				if( root->right==NULL && root->left==NULL ){
				    delete cus;
					return NULL;
				}
				else if( root->left==NULL ){
					customer *p=root->right;
					delete root;
					return p;				
				}
				else if( root->right==NULL ){
					customer *p=root->left;
					delete root;
					return p;
				}
				else{
					customer *p=find(root->right);
					root->name=p->name;
					root->result=p->result;
					root->tree=p->tree;
					root->right=removecus(root->right,p);
				}
			
		}
		return root;
	}
	void remove(int y,int id){
		while( y>0 && (!timeline[id].empty()) ){
			customer *temp=timeline[id].front();
			timeline[id].pop();
		    h[id]=removecus(h[id],temp);
			--y;
		}
		if( timeline[id].empty() ) h[id]=NULL;
	}
    int CountCustomer(customer *root){
		return root == nullptr ? 0 : 1 + CountCustomer(root->left) + CountCustomer(root->right);
	}
	unsigned long long Pascal(int n, int k)
	{
		unsigned long long pascal[n + 1][k + 1];
		for (int i = 0; i <= n; ++i)
		{
			for (int j = 0; j <= k && j <= i; ++j)
			{
				if (j == 0 || j == i)
				{
					pascal[i][j] = 1;
					pascal[i][j]%=MAXSIZE;
				}
				else
				{
					pascal[i][j] = pascal[i - 1][j - 1] + pascal[i - 1][j];
					pascal[i][j]%=MAXSIZE;
				}
			}
		}

		return pascal[n][k];
	}
	unsigned long long CountPermutation(customer *root){
		if (root == NULL) return 1;
		unsigned long long sizeleft = CountCustomer(root->left);
		unsigned long long sizeright = CountCustomer(root->right);
		return (((Pascal(sizeleft + sizeright, sizeleft) * CountPermutation(root->left))%MAXSIZE) * CountPermutation(root->right))%MAXSIZE;
	}
	void kokusen(){
		for(int i=0;i<MAXSIZE;++i){
			if( h[i]!=NULL ){ 
				int y=CountPermutation(h[i]);
			    y=y%MAXSIZE;
			    remove(y,i);
			}
		}
	}
	void inorder(customer *root){
		if( root==NULL ) return ;
		inorder( root->left );
		cout<< root->result <<endl;
		inorder( root->right );
	}
	void print(int num){
		if( num<=0 || num>MAXSIZE ) return ;
		int id=num-1;
		inorder( h[id] );
	}
	~Gojo(){}
};
struct area{
	int label;
	int size;
    int time;
	vector<customer *> cusinarea;
};
class Sukuna{
	private:
	int heapsize;
	vector<area> heap;
	int totaltime;
	public:
	Sukuna(){ 
        this->heapsize=0;
        this->totaltime=0; 
    }
	int left(int i){ return i*2+1; }
	int right(int i){ return i*2+2; }
	int parent(int i){ return (i-1)/2; }
	void swap(area &a1,area &a2){
		area temp=a1;
		a1=a2;
		a2=temp;
	}
	void reheapup(int i){
		if( i<=0 || i>=heapsize ) return ;
		int p=parent(i);
		if( heap[p].size>heap[i].size ){
			swap(heap[p],heap[i]);
			reheapup(p);
		}
		else if( heap[p].size == heap[i].size ){
			if(  heap[p].time < heap[i].time  ) return ;
			else{
				swap(heap[p],heap[i]);
			    reheapup(p);
			}
		}
		else return ;
	}
	void reheapdown(int i){
		if( i<0 || i>=heapsize ) return ;
		int l=left(i);
		int r=right(i);
		int temp=i;
		if( l<heapsize && ( heap[l].size<heap[temp].size || (heap[l].size==heap[temp].size && heap[l].time< heap[temp].time) ) ) temp=l;
		if( r<heapsize && ( heap[r].size<heap[temp].size || (heap[r].size==heap[temp].size && heap[r].time< heap[temp].time) ) ) temp=r;
		if( temp!=i ){
			swap(heap[i],heap[temp]);
			reheapdown( temp );
		}
	}
	void pushheap(area a){
		++heapsize;
		if(heap.size()==0 ) heap.push_back(a);
		else{
			heap.push_back(a);
			reheapup( heap.size()-1 );
		}
	}
	void add(customer *cus,int id){
		int i=0;
		while( i<heapsize ){
			if( heap[i].label==id ) break;
			++i;
		}
		if( i>=heapsize ){
			area newarea;
			newarea.label=id;
			newarea.size=1;
			newarea.cusinarea.push_back(cus);
            newarea.time=totaltime+1;
            ++totaltime;
			pushheap( newarea );
		}
		else{
			++heap[i].size;
			heap[i].cusinarea.push_back(cus);
            heap[i].time=totaltime+1;
            ++totaltime;
			reheapup( i );
			reheapdown( i );
		}
	}
	void remove(area &a,int num,vector<pair<customer *,int>> &dethcus){
		while( num>0 && a.size>0 ){
			customer *cus=a.cusinarea.front();
			a.cusinarea.erase( a.cusinarea.begin() );
			--a.size;
			--num;
			pair<customer*,int> p=make_pair(cus,a.label);
			dethcus.push_back( p );
		}
	}
    int getposoflabel(int label){
        for(int i=0;i<heapsize;++i){
            if( heap[i].label==label ) return i;
        }
		return INT_MAX;
    }
	void keitenken(int num){
		if( num<=0 ) return ;
		vector<pair<customer *,int>> dethcus;
		int k=num;
        auto cmp=[](area a1,area a2){
            if( a1.size== a2.size ){ return a1.time<a2.time; }
            return a1.size<a2.size;
        };
        vector<area> vec;
        for( auto k : heap ){
            vec.push_back( k );
        }
        sort( vec.begin(),vec.end(),cmp );
        while( k>0 && vec.size()>0 ){
            int i=getposoflabel( vec[0].label );
            remove( heap[i],num,dethcus );
            if( heap[i].size!=0 ){
                heap[i].time=totaltime+1;
                ++totaltime;
            }
            else{
                heap[i]=heap[ heapsize-1 ];
                heap.pop_back();
                --heapsize;
                reheapdown( i );
                reheapup( i );
            }
            vec.erase( vec.begin() );
            --k;
        }
		while( dethcus.size()>0 ){
			cout<<dethcus[0].first->result<<"-"<<dethcus[0].second<<endl;
			dethcus.erase( dethcus.begin() );
		}
	}
	void lifo(area a,int num){
		int size=a.size;
		while( num>0 && size>0 ){
			cout<<a.label<<"-"<< a.cusinarea[ size-1 ]->result <<endl;
			--num;
			--size;
		}
	}
	void print(int i,int num){
		if( i>=heapsize || num<=0 ) return ;
		lifo(heap[i],num);
		print(left(i),num);
		print(right(i),num);
	}
    void allcusin(area a){
        cout<<"khu vuc: "<< a.label <<endl;
        for(int i=0;i<a.size;++i){
            cout<<a.cusinarea[i]->name<<"-"<<a.cusinarea[i]->result<<" ";
        }
        cout<<endl;
    }
    void print1(){
        for(int i=0;i<heapsize;++i){
            allcusin(heap[i]);
        }
    }
	~Sukuna(){
		for(int i=0;i<heapsize;++i){
			while( heap[i].cusinarea.size()>0 ){
				heap[i].cusinarea.back()->~customer();
				heap[i].cusinarea.pop_back();
			}
		}
	}
};
class imp_res{
	private:
	Gojo *restofgojo=new Gojo(MAXSIZE);
	Sukuna *restofsukuna=new Sukuna();
	customer lastcus;
	public:
	imp_res(){
		lastcus.tree=new Hufftree();
	}
	void LAPSE(string name){
		//cout<<"Lapse "<<endl;
		customer *cus=createcus(name);
		if( cus==NULL ) return ; 
		int id=cus->result % MAXSIZE +1 ;
		lastcus.tree=cus->tree;
	    if( cus->result%2 != 0 ){ restofgojo->add(cus,id); }
		else{ restofsukuna->add(cus,id); }
	}
	void KOKUSEN(){
		//cout<<"Kokusen"<<endl;
		//solution<<"KOKUSEN"<<endl;
		restofgojo->kokusen();
	}
	void KEITEIKEN(int num){
		//cout<<"Keiteiken "<< num <<endl;
		//solution<<"KEITEIKEN"<<endl;
		restofsukuna->keitenken(num);
	}
	void HAND(){
		//cout<<"Hand"<<endl;
		//solution<<"HAND"<<endl;
		lastcus.tree->inorder( lastcus.tree->getroot() );
	}
	void LIMITLESS(int num){
		//cout<<"Limitless "<< num <<endl;
		//solution<<"LIMITLESS"<<" "<<num<<endl;
		restofgojo->print(num);
	}
	void CLEAVE(int num){
		//cout<<"Cleave "<< num <<endl;
		//solution<<"CLEAVE"<<endl;
		restofsukuna->print(0,num);
	}
	~imp_res(){
		delete restofsukuna;
		delete restofgojo;
	}
};
void simulate(string filename)
{
	string maxsize;
	ifstream ss(filename);
	ss>>maxsize;
	ss>>maxsize;
	MAXSIZE=stoi(maxsize);
	imp_res *r=new imp_res();
	string str,name,num;
	while( ss>>str ){
		if( str=="LAPSE" ){
			ss>>name;
			r->LAPSE(name);
		}
		else if( str=="KOKUSEN"){
			r->KOKUSEN();
		}
		else if( str=="KEITEIKEN" ){
			ss>>num;
			r->KEITEIKEN( stoi(num) );
		}
		else if( str=="HAND" ){
			r->HAND();
		}
		else if( str=="LIMITLESS" ){
			ss>>num;
			r->LIMITLESS(stoi(num));
		}
		else{
			ss>>num;
			r->CLEAVE(stoi(num));
		}
	}
	delete r;
}