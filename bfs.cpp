//
//  bfs.cpp
//  561assign1
//
//  Created by Aditya Raghuwanshi on 9/18/12.
//

#include <iostream>
#include <stdio.h>
#include <queue>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>


using namespace std;

struct pnode {
    int info; 
    int cost;
};

class CompareNode {
public:
    bool operator()(pnode& t1, pnode& t2)
    {
        if(t1.cost>t2.cost) return true;
        return false;
    }
};

struct node {
    int info;
    node *next;
};

class Queue {
public:
    Queue();
    ~Queue();
    bool isEmpty();
    void add(int);
    int get();
private:
    node *first, *last;
};

class Graph {
public:
    Graph(int size = 2);
    ~Graph();
    bool isConnected(int, int);
    // adds the (x, y) pair to the edge set
    void addEdge(int x, int y);
    // searches for the minimum length path
    // between the start and target vertices
    void UCS(int start, int target, int up, int right, int down, int left, ofstream &outfile);
    private :
    int n;
    int sq;
    int **A;
};

Queue::Queue() {
    first = new node;
    first->next = NULL;
    last = first;
}

Queue::~Queue() {
    delete first;
}

bool Queue::isEmpty() {
    return (first->next == NULL);
}

void Queue::add(int x) {
    node *aux = new node;
    aux->info = x;
    aux->next = NULL;
    last->next = aux;
    last = aux;
}

int Queue::get() {
    node *aux = first->next;
    int value = aux->info;
    first->next = aux->next;
    if (last == aux) last = first;
    delete aux;
    return value;
}

Graph::Graph(int size) {
    int i, j;
    sq=size;
    size=size*size;
    if (size < 2) n = 2;
    else n = size;
    A = new int*[n];
    for (i = 0; i < n; ++i)
        A[i] = new int[n];
    for (i = 0; i < n; ++i)
        for (j = 0; j < n; ++j)
            A[i][j] = 0;
}

Graph::~Graph() {
    for (int i = 0; i < n; ++i)
        delete [] A[i];
    delete [] A;
}

bool Graph::isConnected(int x, int y) {
    return (A[x-1][y-1] == 1);
}

void Graph::addEdge(int x, int y) {
    A[x-1][y-1] = A[y-1][x-1] = 1;
}

void Graph::UCS(int startA, int startB,int up, int right, int down, int left, ofstream &outfile) {
    priority_queue<pnode, vector<pnode>, CompareNode> pq;
    int *cost = new int[n+1];
    int i, p, q;
    bool found;
    struct aux { int current, prev; };
    aux *X = new aux[n+1];
    aux *B = new aux[n+1];
    int *Y = new int[n+1];
    bool *visited = new bool[n+1];
    for (i = 1; i <= n; ++i)
    {
        visited[i] = false;
        cost[i]=0;
    }
    pnode A = {startA,0};
    pq.push(A);
    visited[startA] = true;
    found = false;
    p = q = 0;
    X[0].current = startA;
    X[0].prev = 0;
    B[0].current = startB;
    B[0].prev = 0;
    while (!pq.empty() && !found) {
        pnode visit = pq.top();
        pq.pop();
        int k=visit.info;
        p=0;
        for(int pcount=0;pcount<=n;pcount++){   //increase level
            if(k==X[pcount].current)
                break;
            p++;
        }
        for (i = 1; i <= n && !found; ++i) {
            if (isConnected(k, i) && !visited[i]) {
                int Tempq,currB,lastB;
                Tempq=q+1;
                pnode add;
                add.cost=0;
                if(i==(k-1))
                {
                    currB=B[p].current-sq;
                    lastB=B[p].current;
                    if(currB<=0)
                        continue;
                    if(!isConnected(currB,lastB))
                        continue;
                    B[Tempq].current=currB;
                    add.info=i;
                    add.cost=left+cost[k];
                    cost[i]=add.cost;
                }
                else if(i==(k+1))
                {
                    currB=B[p].current+sq;
                    lastB=B[p].current;
                    if(currB>n)
                        continue;
                    if(!isConnected(currB,lastB))
                        continue;
                    B[Tempq].current=currB;
                    add.info=i;
                    add.cost=right+cost[k];
                    cost[i]=add.cost;
                }
                else if(i==(k+sq))
                {
                    currB=B[p].current-1;
                    lastB=B[p].current;
                    if(currB==0)
                        continue;
                    if(!isConnected(currB,lastB))
                        continue;
                    B[Tempq].current=currB;
                    add.info=i;
                    add.cost=down+cost[k];
                    cost[i]=add.cost;
                }
                else if(i==(k-sq))
                {
                    currB=B[p].current+1;
                    lastB=B[p].current;
                    if(currB>n)
                        continue;
                    if(!isConnected(currB,lastB))
                        continue;
                    B[Tempq].current=currB;
                    add.info=i;
                    add.cost=up+cost[k];
                    cost[i]=add.cost;
                }
                pq.push(add);
                visited[i]=true;
                ++q;
                X[q].current = i;
                X[q].prev = p;
                B[q].prev = p;
                visited[i] = true;
                if(X[q].current==B[q].current) {
                    found=true;
                }
            }//end if
        }//end for
    }//end while
    
    if(!found) {
        outfile << "-1"<<endl<<endl;
        return;
    }
    outfile<<cost[X[q].current]<<endl;
    p = 0;
    while (q) {
        Y[p] = X[q].current;
        q = X[q].prev;
        ++p;
    }

    Y[p] = X[0].current;
    for (q = 0; q <= p/2; ++q) {
        int temp = Y[q];
        Y[q] = Y[p-q];
        Y[p-q] = temp;
    }
    int currPos,currdiv;
    currPos=Y[0]/sq;
    currdiv=Y[0]%sq;
    if(currdiv==0)
        outfile<<sq<<" "<<currPos<<endl;
    else
        outfile<<currdiv<<" "<<currPos+1<<endl;
    for (q = 0; q <= p; ++q) {
        if(q!=0) {
            currPos=Y[q]/sq;
            currdiv=Y[q]%sq;
            if(currdiv==0)
                outfile<<sq<<" "<<currPos<<endl;
            else
                outfile<<currdiv<<" "<<currPos+1<<endl;
        }
    }
    outfile << endl;
    delete [] visited;
    delete [] X;
    delete [] Y;
    delete [] B;
    delete [] cost;
}

int main() {
    string line;
    int nodes,edges;
    int up, right, down, left;
    int sA[2],sB[2];
    int u,v;
    int i=0;
    int*obstacles;
    int onum=0;
    ifstream myfile ("input.txt");
    
    if (myfile.is_open()) 
    {
        //get size
        getline (myfile,line);
        nodes=atoi(line.c_str());
        //get costs
        getline (myfile,line);
        up=atoi(line.c_str());
        getline (myfile,line);
        right=atoi(line.c_str());
        getline (myfile,line);
        down=atoi(line.c_str());
        getline (myfile,line);
        left=atoi(line.c_str());
        //get startA
        getline (myfile,line);
        stringstream streamA(line);
        string startA;
        while( getline(streamA, startA, ' ') ) {
            sA[i++]=atoi(startA.c_str());
        }
        i=0;
        //get startB
        getline (myfile,line);
        stringstream streamB(line);
        string startB;
        while( getline(streamB, startB, ' ') ) {
            sB[i++]=atoi(startB.c_str());
        }
        
        u=((sA[1]-1)*nodes)+sA[0];
        v=((sB[1]-1)*nodes)+sB[0];
        
        obstacles=new int[nodes*nodes];
        while ( myfile.good() ) {
            getline (myfile,line);
            stringstream streamO(line);
            string obStr;
            int obI[2];
            i=0;
            while( getline(streamO, obStr, ' ') ) {
                obI[i++]=atoi(obStr.c_str());
            }
            obstacles[onum++]=((obI[1]-1)*nodes)+obI[0];
        }
    } 
    else {
        return 0;
    }
    myfile.close();
    //initialize graph
    Graph f(nodes);
    
    for(int x=1;x<nodes*nodes;x++) {
        bool obstaclefound=false;
        for(int y=0;y<onum;y++) {
            if(obstacles[y]==x || obstacles[y]==x+1) {
                obstaclefound=true;
                break;
            }
        }
        if(!obstaclefound && x%nodes>0) {
            f.addEdge(x,x+1);
        }
    }
    
    for(int x=1;x<=((nodes*nodes)-nodes);x++) {
        bool obstaclefound=false;
        for(int y=0;y<onum;y++) {
            if(obstacles[y]==x || obstacles[y]==x+nodes) {
                obstaclefound=true;
                break;
            }
        }
        if(!obstaclefound) {
            f.addEdge(x,x+nodes);
        }
    }
        
    
    //Run BFS/UCS
    ofstream outfile ("output.txt");
    f.UCS(u,v,1,1,1,1,outfile);
    f.UCS(u,v,up,right,down,left,outfile);
    outfile.close();
    return 0;
}

