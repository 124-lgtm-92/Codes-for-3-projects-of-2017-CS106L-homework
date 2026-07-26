#include <iostream>
#include "wikiscraper.h"
#include "error.h"
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
using std::equal;           using std::cerr;

WikiScraper::WikiScraper() {
    // could use shared_ptr for this and avoid hacky
    // preprocessor stuff
    /*
    if(QCoreApplication::instance() == nullptr) {
        std::cout << "Making" << std::endl;
        argv = new char*[2];
        argv[0] = "SharedLib";
        argv[1] = nullptr;
        QCoreApplication *a = new QCoreApplication(argc, argv);
    } else {
        cout << "Didn't make" << endl;
    } */
    (void)getPageSource("Main_Page");
#ifdef _WIN32
    // define something for Windows
    system("cls");
    cout << string(5, '-') << "> Windows clear" << endl;
#else
    // define it for a Unix machine
    system("clear");
    cout << string(5, '-') << "> Unix clear" << endl;
#endif
    cout << endl;

}

string createPageUrl(const string& page_name) {
    return "https://en.wikipedia.org/wiki/" + page_name;
}


void notFoundError(const string& msg, const string& page_name, const string& url) {
    const string title = "    AN ERROR OCCURED DURING EXECUTION.    ";
    const string border(title.size() + 4, '*');
    cerr << endl;
    errorPrint(border);
    errorPrint("* " + title + " *");
    errorPrint(border);
    errorPrint();
    errorPrint("Reason: " + msg);
    errorPrint();
    errorPrint("Debug Information:");
    errorPrint();
    errorPrint("\t- Input parameter: " + page_name);
    errorPrint("\t- Attempted url: " + url);
    errorPrint();
}

string WikiScraper::getPageSource(const string &page_name) {
    const static string not_found = "Wikipedia does not have an article with this exact name.";
    if(memo.find(page_name) == memo.end()) {
        QUrl url(createPageUrl(page_name).c_str()); // need c string to convert to QString

        QNetworkRequest request(url);

        QNetworkReply *reply(manager.get(request));

        QEventLoop loop;
        QObject::connect(reply, SIGNAL(finished()) , &loop, SLOT(quit()));
        loop.exec();

        string ret = reply->readAll().toStdString();
        if(std::search(ret.begin(), ret.end(), not_found.begin(), not_found.end()) != ret.end()){
            notFoundError("Page does not exist!", page_name, url.toString().toStdString());
            //        throw std::domain_error("Page does not exist");
            return "";
        }
        size_t indx = ret.find("plainlinks hlist navbar mini");
        if(indx != string::npos) {
            return ret.substr(0, indx);
        }
        memo[page_name] = ret;
    }
    return memo[page_name];

}

unordered_set<string> WikiScraper::getLinkSet(const string& page_html)
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
