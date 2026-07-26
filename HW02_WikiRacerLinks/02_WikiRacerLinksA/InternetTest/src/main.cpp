
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <set>
#include "wikiscraper.h"
#include<algorithm>
using std::cout;    using std::cin;
using std::endl;    using std::string;
using std::vector;  using std::find;
vector<string> findWikiLadder(const string& start_page,const string& end_page);
void waitForInput() {
    string msg = " Take screenshot, then press enter to continue: ";
    cout << string(msg.size(), '=') << endl;
    cout << msg << endl;
    cout << string(msg.size(), '=') << endl;
    string s;
    (void)std::getline(std::cin, s);
}

int main() {
       WikiScraper w;
       waitForInput();
       cout <<   w.getPageSource("Strawberry") <<  endl;
       waitForInput();
       cout << "========================" <<  endl;
       cout <<  w.getPageSource("Mathematics") <<  endl;
        waitForInput();
       cout << "========================" <<  endl;
       cout <<  w.getPageSource("Stanford_Universitya") <<  endl;
       waitForInput();
       cout << "Done!" << endl;
       return 0;
}
vector<string> findWikiLadder(const string& start_page,const string& end_page)
{
    WikiScraper scraper;
    auto target_set=scraper.getLinkSet(end_page);
    auto cmpFn=[&target_set,&scraper](auto ladder1,auto ladder2)
    {
        auto page1=ladder1.back();
        auto page2=ladder2.back();
        auto page1set=scraper.getLinkSet(page1);
        auto page2set=scraper.getLinkSet(page2);
        int num1=0;
        for(auto i:page1set)
        {
            for(auto j:target_set)
            {
                if(i==j)
                {
                    num1+=1;
                }
            }
        }
        int num2=0;
        for(auto i:page2set)
        {
            for(auto j:target_set)
            {
                if(i==j)
                {
                    num2+=1;
                }
            }
        }
        return num1<num2;
    };
    std::priority_queue<vector<string>,vector<vector<string>>,decltype(cmpFn)>ladderQueue(cmpFn);
    vector<string>fladder;
    fladder.push_back(start_page);
    ladderQueue.push(fladder);
    while(!ladderQueue.empty())
    {
        auto topladder=ladderQueue.top();
        cout<<"{";
        for(auto i:topladder)
        {
            cout<<i<<" ";
        }
        cout<<"}"<<endl;
        auto backoftopladder=topladder.back();
        auto backset=scraper.getLinkSet(backoftopladder);
        bool ifcontains=false;
        for(auto i:backset)
        {
            if(end_page==i)
            {
                ifcontains=true;
                break;
            }
        }
        if(ifcontains)
        {
            topladder.push_back(end_page);
            return topladder;
        }
        for(auto i:backset)
        {
            if(find(topladder.begin(),topladder.end(),i)==topladder.end())
            {
                auto copyladder=topladder;
                copyladder.push_back(i);
                ladderQueue.push(copyladder);
            }
        }
    }
    return vector<string>{};
}


