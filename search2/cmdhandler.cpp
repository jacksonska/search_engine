#include "cmdhandler.h"
#include "util.h"
#include "combiners.h"
#include "webpage.h"
#include "msort.h"
#include <string>
#include <sstream>
#include <vector>

using namespace std;


struct name 
{
    bool operator()(const pair<WebPage*, double>& p1,
     pair<WebPage*, double>& p2);
};
//functor used for sorting by name
bool name::operator()(const pair<WebPage*, double>& p1,
     pair<WebPage*, double>& p2)
     {
        string page1 = p1.first->filename();
        string page2 = p2.first->filename();

        return page1 < page2;
     }

struct srank 
{
    bool operator()(const pair<WebPage*, double>& p1,
     pair<WebPage*, double>& p2);
};

bool srank::operator()(const pair<WebPage*, double>& p1,
     pair<WebPage*, double>& p2)
     {
         return p1.second > p2.second;
     }

QuitHandler::QuitHandler()
{

}

QuitHandler::QuitHandler(Handler* next)
    : Handler(next)
{

}

bool QuitHandler::canHandle(const std::string& cmd) const
{
    return cmd == "QUIT";

}

Handler::HANDLER_STATUS_T QuitHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr)
{
    return HANDLER_QUIT;
}

PrintHandler::PrintHandler()
{

}

PrintHandler::PrintHandler(Handler* next)
    : Handler(next)
{

}

bool PrintHandler::canHandle(const std::string& cmd) const
{
    return cmd == "PRINT";

}

Handler::HANDLER_STATUS_T PrintHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr)
{
    string name;

    if (!(instr >> name)) {
        return HANDLER_ERROR;
    }
    try {
        eng->display_page(ostr, name);
    }
    catch (std::exception& e) {
        return HANDLER_ERROR;
    }
    return HANDLER_OK;
}

//AND HANDLER
ANDHandler::ANDHandler()
{
   //do i need to account for if they do not give me a combiner
    combiner_ = new ANDWebPageSetCombiner;
}

ANDHandler::~ANDHandler()
{
    delete combiner_;
}

ANDHandler::ANDHandler(Handler* next, WebPageSetCombiner* combiner) 
: Handler(next)
{
    combiner_ = combiner;
}

bool ANDHandler::canHandle(const string& cmd) const
{
    return cmd == "AND";
    
}

Handler::HANDLER_STATUS_T ANDHandler::process(SearchEng* eng, istream& instr, ostream& ostr)
{
    string term, line;
    vector<string> terms;
     
    //read in the terms
    getline(instr, line);

    stringstream ss(line);

    //keep reading words to push into the terms vector
    while(ss >> term)
    {
        terms.push_back(conv_to_lower(term));
    }

    //call the search function using the AndCombiner
    WebPageSet result = eng->search(terms, combiner_);

    display_hits(result, ostr);

    return HANDLER_OK;    

}


//OR HANDLER
ORHandler::ORHandler()
{
    //do i need to account for if they do not give me a combiner
    combiner_ = new ORWebPageSetCombiner;
}

ORHandler::~ORHandler()
{
    delete combiner_;
}

ORHandler::ORHandler(Handler* next, WebPageSetCombiner* combiner) 
: Handler(next)
{
    combiner_ = combiner;
}

bool ORHandler::canHandle(const string& cmd) const
{
    return cmd == "OR";
}

Handler::HANDLER_STATUS_T ORHandler::process(SearchEng* eng, istream& instr, ostream& ostr)
{
    string term, line;
    vector<string> terms;
     
    //read in the terms
    getline(instr, line);

    stringstream ss(line);

    //keep reading words to push into the terms vector
    while(ss >> term)
    {
        terms.push_back(conv_to_lower(term));
    }

    //call search on combiner, performing action, and print
    WebPageSet result = eng->search(terms, combiner_);
    display_hits(result, ostr);


    return HANDLER_OK;

}

//PAGERANK OR HANDLER
PRORHandler::PRORHandler()
{
    combiner_ = new ORWebPageSetCombiner;
}
PRORHandler::~PRORHandler()
{
    delete combiner_;
}
PRORHandler::PRORHandler(Handler* next, WebPageSetCombiner* combiner)
: Handler(next)
{
    combiner_ = combiner;
}
bool PRORHandler::canHandle(const string& cmd) const
{
    return cmd == "PROR";
}
Handler::HANDLER_STATUS_T PRORHandler::process(SearchEng* eng, istream& instr, ostream& ostr)
{
    string term, line;
    vector<string> terms;
    
    char c;

    //read in the terms
    getline(instr, line);

    stringstream ss(line);
    
    ss >> c; //get the char for sorting the page rank

    //keep reading words to push into the terms vector
    while(ss >> term)
    {
        terms.push_back(conv_to_lower(term));
    }

    //call search on combiner, performing OR
    WebPageSet result = eng->search(terms, combiner_);
    //rank the pages
    vector< pair<WebPage*, double> > prresult;
    prresult = eng->pageRank(result);

    //sort the results based r or n
    if(c == 'r')
    {
        //want sorted based on rank
        mergeSort(prresult, srank());

    }
    else if (c == 'n')
    {
        mergeSort(prresult, name());
    }
    else
    {
        return HANDLER_ERROR; //the handler doesn't know how to sort results
    }

    display_ranked_hits(prresult, cout);

    return HANDLER_OK;
}

//PAGERANK AND HANDLER
PRANDHandler::PRANDHandler()
{
    combiner_ = new ANDWebPageSetCombiner;
}
PRANDHandler::PRANDHandler(Handler* next, WebPageSetCombiner* combiner)
: Handler(next)
{
    combiner_ = combiner;
}
PRANDHandler::~PRANDHandler()
{
    delete combiner_;
}
bool PRANDHandler::canHandle(const string& cmd) const
{
    return cmd == "PRAND";
}

Handler::HANDLER_STATUS_T PRANDHandler::process(SearchEng* eng, istream& instr, ostream& ostr)
{
    string term, line;
    vector<string> terms;

    char c;
    //read in the terms
    getline(instr, line);

    stringstream ss(line);
    ss >> c; //get the char for sorting the page rank

    //keep reading words to push into the terms vector
    while(ss >> term)
    {
        terms.push_back(conv_to_lower(term));
    }

    //call search on combiner, performing AND
    WebPageSet result = eng->search(terms, combiner_);
    //rank the pages
    vector< pair<WebPage*, double> > prresult;
    prresult = eng->pageRank(result);

    //sort the results based r or n
    if(c == 'r')
    {
        //want sorted based on rank
        mergeSort(prresult, srank());

    }
    else if (c == 'n')
    {
        mergeSort(prresult, name());
    }
    else
    {
        return HANDLER_ERROR; //the handler doesn't know how to sort results
    }

    display_ranked_hits(prresult, cout);

    return HANDLER_OK;
}


//DIFF HANDLER
DIFFHandler::DIFFHandler()
{
    //do i need to account for if they do not give me a combiner
    combiner_ = new DiffWebPageSetCombiner;
}

DIFFHandler::~DIFFHandler()
{
    delete combiner_;
}

DIFFHandler::DIFFHandler(Handler* next, WebPageSetCombiner* combiner) 
: Handler(next)
{
    combiner_ = combiner;
}

bool DIFFHandler::canHandle(const std::string& cmd) const
{
    return cmd == "DIFF";
}

Handler::HANDLER_STATUS_T DIFFHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr)
{
    string term, line;
    vector<string> terms;
     
    //read in the terms
    getline(instr, line);

    stringstream ss(line);

    //keep reading words to push into the terms vector
    while(ss >> term)
    {
        terms.push_back(conv_to_lower(term));
    }

    WebPageSet result = eng->search(terms, combiner_);
    display_hits(result, ostr);

    return HANDLER_OK;

}

//INCOMING HANDLER
IncomingHandler::IncomingHandler()
{

}

IncomingHandler::IncomingHandler(Handler* next) 
: Handler(next)
{

}

bool IncomingHandler::canHandle(const std::string& cmd) const
{
    return cmd == "INCOMING";

}

Handler::HANDLER_STATUS_T IncomingHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr)
{
    string filename;

    //read the filename
    if(!(instr >> filename)) //check filename was read properly
        return HANDLER_ERROR;
    
    //search for the page
    WebPage* page = eng->retrieve_page(filename);

    if(page == NULL)
    {
        //page does not exist
        return HANDLER_ERROR;
    }
    //display the incoming links of this page
    display_hits(page->incoming_links(), ostr);

    return HANDLER_OK;
    
}


//OUTGOING HANDLER
OutgoingHandler::OutgoingHandler()
{

}

OutgoingHandler::OutgoingHandler(Handler* next) 
: Handler(next)
{

}


bool OutgoingHandler::canHandle(const std::string& cmd) const
{
    return cmd == "OUTGOING";
}


Handler::HANDLER_STATUS_T OutgoingHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr)
{
    string filename;

    //read the filename
    if(!(instr >> filename)) //check filename was read properly
        return HANDLER_ERROR;
    
    //search for the page
    WebPage* page = eng->retrieve_page(filename);

    if(page == NULL)
    {
        //page does not exist
        return HANDLER_ERROR;
    }
    //display the incoming links of this page
    display_hits(page->outgoing_links(), ostr);

    return HANDLER_OK;
}





