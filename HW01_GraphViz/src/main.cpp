#include <iostream>
#include "SimpleGraph.h"
#include <vector>
#include <fstream>
#include<string>
#include<cmath>
#include<sstream>
#include<ctime>
#include<cstdlib>
using std::cin;  using std::cout;
using std::endl;  using std::vector;
using std::ifstream;  using std::getline;
using std::string;  using std::sin;
using std::cos;  using std::atan2;
using std::istringstream; using std::time;
using std::sqrt;using std::difftime;
using std::srand;using std::rand;

void Welcome();
void Readgraph(SimpleGraph& graph,int& requirednodenums);
SimpleGraph& construtNodes(SimpleGraph&graph,const int&nodenums);
double Getruntime();
void moveNodes(SimpleGraph&graph);
void formalDrawing(SimpleGraph&graph,int&nodenums);
// Main method
int main() {
    Welcome();
    SimpleGraph simplegraph;
    int nodenums=0;
    formalDrawing(simplegraph,nodenums);
    return 0;
}

void Readgraph(SimpleGraph& graph,int& requirednodenums)
{   while(true)
    {
    cout<<"Please input a filename!"<<endl;
    string line;
    getline(cin,line);
    ifstream input(line);
    if(!input.is_open())
    {
        cout<<"Couldn't open the related file!"<<endl;
        continue;
    }
    int linenums=0;
    string obj;
    while(getline(input,obj))
    {
        linenums+=1;
    }
    input.clear();
    input.seekg(0);
    for(int k=0;k<linenums;++k)
    {
        if(k==0)
        {
           if(!(input>>requirednodenums))
            {
                cout<<"Couldn't extract the nodenums!The program is broken!"<<endl;
                return;
            }
        }
        else
        {   size_t startnode;
            size_t endnode;
            if(!(input>>startnode>>endnode))
            {
                 cout<<"Couldn't correctly extract the edges!The program is broken!"<<endl;
                 return;
            }
            else
            {graph.edges.emplace_back(Edge{startnode,endnode});
            }
        }

    }
    return;
    }
}
SimpleGraph& construtNodes(SimpleGraph&graph,const int&nodenums)
{   for(int i=0;i<nodenums;++i)
    {
        double x_x=cos(2*kPi*i/nodenums);
        double y_y=sin(2*kPi*i/nodenums);
        graph.nodes.emplace_back(Node{x_x,y_y});
    }
    return graph;
}
double Getruntime()
{
    string prompt="Please input the correct run time!It shoule be positive!";
    while(true)
    {
        cout<<prompt<<endl;
        string rtpiece;
        getline(cin,rtpiece);
        istringstream iss(rtpiece);
        double timetorun;
        char testchar;
        if(!(iss>>timetorun))
        {
            cout<<"You have provided the wrong time!"<<endl;
            continue;
        }
        if(iss>>testchar)
        {
            cout<<"You have provided more information!"<<endl;
            continue;
        }
        if(timetorun<=0)
        {
            cout<<"You have provided the wrong time!"<<endl;
            continue;
        }
        return timetorun;

    }
}
void moveNodes(SimpleGraph&graph)
{   vector<double>xnetforce(graph.nodes.size(),0);
    vector<double>ynetforce(graph.nodes.size(),0);
    for(int i=0;i<graph.nodes.size()-1;++i)
    {   auto& node1=graph.nodes[i];
        for(int k=i+1;k<graph.nodes.size();++k)
        {
            auto& node2=graph.nodes[k];
            double Frepel=Krepel/sqrt((node2.y-node1.y)*(node2.y-node1.y)+(node2.x-node1.x)*(node2.x-node1.x));
            double theta=atan2(node2.y-node1.y,node2.x-node1.x);
            xnetforce[i]-=Frepel*cos(theta);
            ynetforce[i]-=Frepel*sin(theta);
            xnetforce[k]+=Frepel*cos(theta);
            ynetforce[k]+=Frepel*sin(theta);
        }
    }
    for(int i=0;i<graph.edges.size();++i)
    {
        auto& eachEdge=graph.edges[i];
        auto& startnode=graph.nodes[eachEdge.start];
        auto& endnode=graph.nodes[eachEdge.end];
        double Fattract=Kattract*((endnode.y-startnode.y)*(endnode.y-startnode.y)+(endnode.x-startnode.x)*(endnode.x-startnode.x));
        double theta=atan2(endnode.y-startnode.y,endnode.x-startnode.x);
        xnetforce[eachEdge.start]+=Fattract*cos(theta);
        ynetforce[eachEdge.start]+=Fattract*sin(theta);
        xnetforce[eachEdge.end]-= Fattract*cos(theta);
        ynetforce[eachEdge.end]-=Fattract*sin(theta);

    }
    srand(time(NULL));
    for(int i=0;i<graph.nodes.size();++i)
    {
        graph.nodes[i].x+=xnetforce[i];
        graph.nodes[i].y+=ynetforce[i];
    }
    DrawGraph(graph);
}
void formalDrawing(SimpleGraph&graph,int&nodenums)
{
    string prompt="Do you want to get try?Please input 0/1. 0 refers to break,and 1 refers to go on!";
    cout<<prompt<<endl;
    string newline;
    getline(cin,newline);
    if(newline=="1")
    {
        Readgraph(graph,nodenums);
        construtNodes(graph,nodenums);
        double Timetorun=Getruntime();
        time_t startTime=time(NULL);
        InitGraphVisualizer(graph);
        while(true)
        {moveNodes(graph);
         double elapsedTime=difftime(time(NULL),startTime);
         if(elapsedTime>=Timetorun)
            break;
        }
        formalDrawing(graph,nodenums);
    }
    else
    {   cout<<"See you next time!"<<endl;
        return;
    }


}

/* Prints a message to the console welcoming the user and
 * describing the program. */
void Welcome() {
    cout << "Welcome to CS106L GraphViz!" << endl;
    cout << "This program uses a force-directed graph layout algorithm" << endl;
    cout << "to render sleek, snazzy pictures of various graphs." << endl;
    cout << endl;
}

