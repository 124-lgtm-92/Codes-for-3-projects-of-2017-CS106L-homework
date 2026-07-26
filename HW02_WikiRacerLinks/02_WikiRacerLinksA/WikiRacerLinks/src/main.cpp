#include <iostream>
#include <fstream>
#include <unordered_set>
#include<string>
#include<algorithm>
#include<numeric>
using std::cout;            using std::endl;
using std::cin;             using std::find;
using std::string;          using std::unordered_set;
using std::ifstream;        using std::string;
using std::search;          using std::all_of;
using std::equal;
unordered_set<string> findWikiLinks(const string& page_html);
string& readfiles(string&myfiles);
int main() {
	
    /* TODO: Write code here! */

    /* Note if your file reading isn't working, please follow the
     * steps posted on the front page of the CS106L website. Essentially
     * go to the projects tab on the panel on the left, and in the run
     * section, uncheck the "Run in terminal" box and re-check it. This
     * should fix things.
     */
    string myfiles="";
    myfiles=readfiles(myfiles);
    unordered_set<string>myset=findWikiLinks(myfiles);
    for(auto i:myset)
    {
        cout<<i<<endl;
    }

}
string& readfiles(string&myfiles)
{   while(true)
    {
        cout<<"Please input a filename!"<<endl;
        string line;
        getline(cin,line);
        ifstream input(line);
        if(!input.is_open())
        {
            cout<<"The system couldn't open your file!Please retry!"<<endl;
            continue;
        }
        string obj;
        while(getline(input,obj))
        {
            myfiles+=obj;
        }
        break;
    }
    return myfiles;
}

unordered_set<string> findWikiLinks(const string& page_html)
{
    unordered_set<string>myset;
    auto startitr=page_html.begin();
    auto enditr=page_html.end();
    string startslice="<a href=";
    string endslice="</a>";
    while(startitr!=enditr)
    {
        auto findfpos=search(startitr,enditr,startslice.begin(),startslice.end());
        if(findfpos!=enditr)
        {
            auto findlpos=search(findfpos+8,enditr,endslice.begin(),endslice.end());
            if(findlpos!=enditr)
            {
                string temp="";
                auto copyfindfpos=findfpos;
                while(copyfindfpos!=findlpos+4)
                {
                    temp+=*copyfindfpos;
                    ++copyfindfpos;
                }
              string totest="";
              if(temp.length()<15)
              {     startitr=findlpos+4;
                    continue;
              }
              auto totestitr=temp.begin()+9;
              auto totestitr2=temp.begin()+15;
              while(totestitr!=totestitr2)
              {
                    totest+=*totestitr;
                    ++totestitr;
              }
              string totest1="/wiki/";
              if(!equal(totest1.begin(),totest1.end(),totest.begin()))
              {     startitr=findlpos+4;
                    continue;
              }

              if(all_of(temp.begin(),temp.end(),[](char i){return i!='#'&&i!=':';}))
              {  string copyones="";
                   auto copytempitr1=temp.begin()+15;
                   auto copytempitr2=find(temp.begin()+15,temp.end(),'"');
                   while(copytempitr1!=copytempitr2)
                   {
                       copyones+=*copytempitr1;
                       ++copytempitr1;
                   }
                   myset.insert(copyones);
              }
                startitr=findlpos+4;
            }
            else
            {
                break;
            }
        }
        else
        {
            break;
        }
    }
    return myset;
}
