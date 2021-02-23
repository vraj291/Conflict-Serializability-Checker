#include <iostream>
#include <set>
#include <vector>
using namespace std;

// Standard Implementation of a Queue
class Queue{
    int front,rear,height;
    int* arr;
    public:
    Queue(int len):height(len){
        front=rear=-1;
        arr=new int[len];
    }
    bool isEmpty(){
        return (front == -1)&&(rear==-1);
    }
    void enqueue(int c){
        if(front ==-1 and rear == -1){
            front=0;
            rear=0;
        }else
            rear++;
        arr[rear]=c;
    }
    int dequeue(){
        int val=arr[front];
        if(front == rear){
            front=-1;
            rear=-1;
        }else
            front++;
        return val;
    }
    int peek(){
        return arr[front];
    }
};

// Standard Implementation of a Stack
class Stack{
    int height;
    int top;
    int* arr;
    public:
    Stack(int len):height(len){
        top=-1;
        arr=new int[len];
    }
    bool isEmpty(){
        return top==-1;
    }
    void push(int c){
        arr[++top]=c;
    }
    int pop(){
        if (!isEmpty())
            return arr[top--];
        return '$';
    }
};

class Schedule{

    vector<string> op;
    set<char> nodes;
    int** graph;
    bool* visited;
    public:

    Schedule(){
    }

    void evaluate(string inp){
        preprocess(inp);
        createGrid();
        getmatrix();
        if(!(detectCycles()))
            cout<<"\nThe input schedule is conflict serializable.\n";
        else
             cout<<"\nThe input schedule is not conflict serializable.\n";
        reset();
    }

    void reset(){
        op.clear();
        nodes.clear();
        graph=NULL;
        visited=NULL;
    }

    string checkInput(string inp){
        string out;
        for(int i=0;i<inp.length();i++){
            if(isalnum(inp.at(i)))
                out+=inp.at(i);
        }
        return out;
    }

    // Filtering input
    void preprocess(string inp){
        inp=checkInput(inp);
        cout<<"\nInput Schedule: \n\n";
        for(int i=0;i<inp.length();i++){
            if (tolower(inp.at(i))=='r' || tolower(inp.at(i))=='w'){
                op.push_back(inp.substr(i,3));
                nodes.insert(inp.at(i+1));
            }
        }
        for (int i=0;i<nodes.size();i++){
            cout<<" T"<<(char)(i+49)<<"     ";
        }
        cout<<"\n";
        for (int i=0;i<nodes.size();i++){
            cout<<"-------";
        }
        cout<<"\n";
        for(int i=0;i<op.size();i++){
            int tranc=distance(nodes.begin(),nodes.find(op[i].at(1)));
            while(tranc){
                cout<<"\t";
                tranc--;
            }
            cout<<op[i]<<"\n";
        }
    }

    void createGrid(){
        graph=new int*[nodes.size()];
        visited=new bool[nodes.size()];
        for(int i=0;i<nodes.size();i++){
            graph[i]=new int[nodes.size()];
            for(int j=0;j<nodes.size();j++){
                graph[i][j]=0;
            }
            visited[i]=false;
        }

        // Creating a precedence graph with the transactions as vertices and the conflicting instructions as edges
        for(int i=0;i<op.size();i++){
            for(int j=i+1;j<op.size();j++){
                if((op[i].at(1)!=op[j].at(1)) && (op[i].at(2)==op[j].at(2)) && (tolower(op[i].at(0))== 'w' || tolower(op[j].at(0))== 'w')){
                    graph[distance(nodes.begin(),nodes.find(op[i].at(1)))][distance(nodes.begin(),nodes.find(op[j].at(1)))] = 1;
                }
            }
        }
    }

    // To get the node with the least in-degree as the staring node for DFS Traversal
    int getStartNode(){
        int mini=INT_MAX,index=-1,sum=0;
        for(int j=0;j<nodes.size();j++){
            sum=0;
            for(int i=0;i<nodes.size();i++){
                sum+=graph[i][j];
            }
            if(sum < mini){
                mini=sum;
                index=j;
            }
        }
        return index;
    }

    // Detecting cycles in produced precedence graph
    bool detectCycles(){
        int start=getStartNode(),curr;
        Stack s1(nodes.size());
        s1.push(start);
        Queue temp(nodes.size());

        // DFS Traversal using a stack
        while(!(s1.isEmpty())){
            curr=s1.pop();
            visited[curr]=true;
            for(int j =0;j<nodes.size();j++){
                if((visited[j] == false) && graph[curr][j]){

                    // Check for back edges
                    for(int i =0;i<nodes.size();i++){
                        if((visited[i] == true) && graph[j][i]){
                            return true;
                        }
                    }

                    // Queue to store adjacent child vertices
                    temp.enqueue(j);
                    s1.push(j);
                }
            }

            // Discarding edges between adjacent child vertices
            while(!(temp.isEmpty())){
                visited[temp.dequeue()]=true;
            }
        }
        return false;
    }

    // Display the adjacency matrix of the created precedence graph for debugging
    void getmatrix(){
        cout<<"\nAdjacency matrix obtained : \n\n";
        for(int i=0;i<nodes.size();i++){
            for(int j=0;j<nodes.size();j++){
                cout << graph[i][j] <<" ";
            }
            cout << "\n";
        }
    }
};

int main(){
    // True Sample Output: r1(X); r3(Y); r3(X); r2(Y); r2(Z); w3(Y); w2(Z); r1(Z); w1(X); w1(Z)
    // False Sample Output: r1(X); r3(Y); r2(Y); r3(X); r1(Z); r2(Z); w3(Y); w1(X); w2(Z); w1(Z)
    string inp;
    Schedule s1;
    cout<<"Enter all operations in a schedule in [R1(X)] format : ";
    getline(cin,inp);
    s1.evaluate(inp);
}
