#include <map>
#include <set>
#include <vector>
#include <iostream>
#include <fstream>
#include "searcheng.h"
#include "util.h"

using namespace std;

SearchEng::SearchEng(PageParser* noExtensionParser)
{
    if (NULL == noExtensionParser)
    {
        throw std::invalid_argument("default parser cannot be NULL");
    }
    this->noExtensionParser_ = noExtensionParser;

    // Add additional code if necessary
    //AM NOT registering this parser, so must delete seperately in destructor

}


/**
* Destructor
*/
SearchEng::~SearchEng()
{
    //need to clean up the parsers and webpages
    //clean up the pages
    map<string, WebPage*>::iterator pit;
    for(pit = pages_.begin(); pit != pages_.end(); ++pit)
    {
        delete pit->second;
    }

    //clean up the parsers
    map<string, PageParser*>::iterator parsit;
    for(parsit = parsers_.begin(); parsit != parsers_.end(); ++parsit)
    {
        delete parsit->second; 
    }

    //manually deleting the noExtensionParser
    delete noExtensionParser_;

}

void SearchEng::read_pages_from_index(const std::string& index_file)
{
    ifstream ifile(index_file.c_str());
    if(ifile.fail()) {
        cerr << "Unable to open index file: " << index_file << endl;
    }

    // Parse all the files
    string filename;
    while(ifile >> filename) {
#ifdef DEBUG
        cout << "Reading " << filename << endl;
#endif
        read_page(filename);
    }
    ifile.close();
}

/*
* Reads a single page at a time - inserting into map of filename -> pages & terms -> pages
* reads the pages terms and links
* sets the incoming links of the pages being linked to from curr page
* update the links and terms of the curr page
*/  
void SearchEng::read_page(const std::string& filename)
{
    //create this new page using filename
    set<string> links;
    set<string> terms;
    //WebPageSet outLinks;
    WebPage* page;

    //check our pages to see if this page already exists  
    map<string, WebPage*>::iterator pit = pages_.find(filename);
    
    if(pit != pages_.end())
    {
        //already exists
        page = pit->second;
    }
    else
    {
        //page does not already exist 
        page = new WebPage(filename);
        pages_.insert(make_pair(filename, page));
    }


    //choose parser
    string ext = extract_extension(filename);
    map<string, PageParser*>::const_iterator it = parsers_.find(ext);

    if(it != parsers_.end())
    {
        //it is registered, use that parser
        it->second->parse(filename, terms, links);
    }
    else
    {
        noExtensionParser_->parse(filename, terms,links);
    }

    //update data:

    //(1)update the newly parsed page's terms
    page->all_terms(terms);

    //(2)update the map of terms to webpage
    set<string>::const_iterator ti;

    //iterate through our set of search terms
    for(ti = terms.begin(); ti != terms.end(); ++ti)
    {
        //insert the current page in set related to curren term
        matches_[*ti].insert(page);
    }

    //(3)update the outgoing links of this page, and the incoming links of other pages
    set<string>::const_iterator lit;

    for(lit = links.begin(); lit != links.end(); ++lit)
    {
        //update the incoming link of the page
        pit = pages_.find(*lit);

        if(pit != pages_.end())
        {
            //page already exists, link it to our currently parsed page
            pit->second->add_incoming_link(page);
        }
        else
        {
            //page does not already exist, create and then link it to this page
            pages_[*lit] = new WebPage(*lit);

            pages_[*lit]->add_incoming_link(page);
        }
        //update currently parsed page's outgoing links
        page->add_outgoing_link(pages_[*lit]);
    }


}//EO READ_PAGE


/**
 * Register a parser for a particular file extension
 * @param[in] extension
 *   File type/extension that should use this parser
 * @param[in] parser
 *   Pointer to the parser
  *  SearchEng will now be responsible for this parser and its deallocation. IE dynamically allocate this baby
 */
void SearchEng::register_parser(const std::string& extension, PageParser* parser)
{
    map<string, PageParser*>::const_iterator it = parsers_.find(extension);



    if(it == parsers_.end())
    {
        //parser isn't already registered, do so
        //make sure not passing a NULL parser
        if(parser == NULL)
            throw std::invalid_argument("Cannot register NULL parser");
        parsers_[extension] = parser;
    }
    else
    {
        throw std::invalid_argument("Parser already exists");
    }
}// EO REGISTER PARSER




/**
* Uses the given search terms and combiner to find the set of webpages
*  that match the query
* @param[in] terms
*   words/terms to search for
* @param[in] combiner
*   Combiner object that implements the strategy for combining the
*   pages that contain each term (i.e. AND, OR, DIFF, etc.)
*
* @return Set of webpages matching the query
*/
WebPageSet SearchEng::search(const std::vector<std::string>& terms, WebPageSetCombiner* combiner) const
{
    //matches_ = the map relating terms to pages
    map<string, WebPageSet>::const_iterator mitA;//iterator for the matches_ map

    WebPageSet finalSet; //set with our pages that match the querie


    if(terms.size() == 0) return finalSet;

    //find the first term in matches_ 
    mitA = matches_.find(terms[0]);

    if(mitA != matches_.end())
    {
        //term is registered to a webpage
        //put this terms pages into the finalS
        finalSet = mitA->second;

    }
    
    //iterate through all the terms after the first 
    for(size_t i = 1; i < terms.size(); i++)
    {
         WebPageSet otherSet;
        //there is another term after this one
        map<string, WebPageSet>::const_iterator mitB;

        mitB = matches_.find(terms[i]);

        if(mitB != matches_.end())
        {
            //i+1st term is accociated with pages
            otherSet= mitB->second;
        }
        //use the combiner on that terms set to update the final set
        finalSet = combiner->combine(finalSet, otherSet);
    
    }
    return finalSet;

}//EO SEARCH


 /**
 * Retrieves the WebPage object for a given page/file
 * @param[in] page_name
 *   Name of page/file to retrieve
 *
 * @return Pointer to the corresponding WebPage object.  Should not be used
 *   to alter or deallocate the object.
 *   Return NULL if page_name does not exist.
 */
WebPage* SearchEng::retrieve_page(const std::string& page_name) const
{
    //i store all registered pages in map <string webpage*>
    map<string, WebPage*>::const_iterator pit;

    //lookup the page/filename in our pages map
    pit = pages_.find(page_name);

    if(pit != pages_.end())
    {
        //page exists
        return pit->second;
    }
    else
    {
        //page does not exist
        return NULL;
    }
}

/**
 * Displays the contents of the page/file
 * @param[inout] ostr
 *   Output stream to display the contnents
 * @param[in] page_name
 *   Name of page/file to display
 *
 * @throw std::invalid_argument if the page_name does not exist
 * @throws std::logic_error
 *   If the file has an extension but no parser is registered for
 *   that extension
 */
void SearchEng::display_page(std::ostream& ostr, const std::string& page_name) const
{
    WebPage* p = retrieve_page(page_name);

    if(p == NULL) throw std::invalid_argument("Page does not exist");

    string ext = extract_extension(page_name);

    //choose parser for this page, need it to display page contents
    map<string, PageParser*>::const_iterator it = parsers_.find(ext);

    if(it != parsers_.end())
    {
        //it is registered, use that parser
        ostr << it->second->display_text(page_name);

    }
    else
    {
        //no parser for this file type
        throw std::logic_error("File type does not have a parser.");   
    }
}


/**
 * Performs the PageRank algorithm on the webpages in the input set
 * [TO BE WRITTEN]
 *
 * @param[in] in_pages
 *   Unexpanded candidate set of webpages to run the PageRank algorithm on
 *
 * @return vector of pairs of webpage pointers and their PageRank score
 */
std::vector<std::pair<WebPage*, double> > SearchEng::pageRank(const WebPageSet& in_pages)
{
    //create the candidate set:
    WebPageSet candidate;
    WebPageSet::const_iterator inputit;
    

    //iterate through the in_pages set and add all outgoing and incoming links to the candidate set
    for(inputit = in_pages.begin(); inputit != in_pages.end(); ++inputit)
    {
        WebPage* pagei = *inputit;
        //iterating through the out-links, add to candidate
        WebPageSet::const_iterator outit;
        for(outit = pagei->outgoing_links().begin(); 
        outit != pagei->outgoing_links().end();
        ++outit)
        {
            WebPage* page = *outit;
            //this page is in the pages we care about
            candidate.insert(page);
            
        }
        //iterating through the in-links, add to candidate
        WebPageSet::const_iterator initerator;
        for(initerator = pagei->incoming_links().begin();
        initerator != pagei->incoming_links().end();
        ++initerator)
        {
            WebPage* page = *initerator;
            //this page is in the pages we care about
            candidate.insert(page);
            
        }
        candidate.insert(pagei);//actually insert the page acquired from the query
    }

    //mapping all pages in the candidate set with their outgoing links (and incoming) to pages within the candidate set, and adding self loop.
    map<WebPage*, WebPageSet> subgraph; //stores webpages and their outgoings within candidate set
    map<WebPage*, WebPageSet> subgraphinc; //stores webpages and their incomings within candidate set
    
    WebPageSet::iterator cit;
    for(cit = candidate.begin(); cit != candidate.end(); ++cit)
    {
        WebPage* page = *cit; //current page
        WebPageSet pinc = page->incoming_links();
        WebPageSet pageout;
        WebPageSet pageinc;
        
        //iterate through the outgoing links of the current page and only add links within the candidate set to the subgraph
        WebPageSet::const_iterator oit;
        for(oit = page->outgoing_links().begin(); oit != page->outgoing_links().end();
        ++oit )
        {
            WebPage* p = *oit;
            //verify the current page is in the candidate set
            if(candidate.find(p) != candidate.end())
            {
                //p is in the candidate set, and thus should be in subgraph of outgoing links
                pageout.insert(p);
            }
        }

        WebPageSet::const_iterator iit;
        //iterate through the incoming links of each page in the candidate set and accumulate their incomings in Cset
        for(iit = pinc.begin(); iit != pinc.end(); 
        ++iit)
        {  
            WebPage* p = *iit;
            //verify the current page is in the candidate set
            if(candidate.find(p) != candidate.end())
            {
                //p is in the candidate set, and thus should be in subgraph of incoming links
                pageinc.insert(p);
            }

        }
        
        pageout.insert(page); //adding self loop to outgoing
        subgraph.insert(make_pair(page, pageout)); //map the page with its outgoing links within the subgraph
        pageinc.insert(page); //adding self loop to incoming
        subgraphinc.insert(make_pair(page, pageinc)); //map the page with its incoming links within the graph
    }


    double N = subgraph.size(); //number of pages
    double damp = 0.15; //dampening factor of 0.15
    vector<pair<WebPage*, double> > prsol; //vector to store all of the pageranks
    
    map<WebPage*, double> temp; //temporary map to store the values of pr calculated during iteration t. Map for fast lookup
    map<WebPage*, double> ranknext; //temporary map to store the values of pr calculated during iteration t+1. Map for fast lookup
    
    //place every page in the subgraph into the solution vector temp map with equal ranks to start
    map<WebPage*, WebPageSet>::iterator sit1;
    for(sit1 = subgraphinc.begin(); sit1 != subgraphinc.end(); ++sit1)
    {
        temp.insert(make_pair(sit1->first, (double)(1/N)));
        ranknext.insert(make_pair(sit1->first, (double)(-1.0)));
        // prsol.push_back(make_pair(sit1->first, 1/N)); //probability surfer starts at any node in subgraph
    }
    

    //implement the page rank formula for t = 20 iterations
    map<WebPage*, WebPageSet>::const_iterator i;
    for(int t = 0; t < 20; t++)
    {  
        // //USED FOR DEBUGGING
        // cout << "iteration:" << t << endl;
        // for(map<WebPage*, double>::const_iterator r = temp.begin(); r != temp.end(); ++r)
        // {
        //     cout << r->first->filename() << ":" << r->second << endl;
        // }


        for(i = subgraphinc.begin(); i != subgraphinc.end(); ++i)
        {
            WebPage* ipage = i->first;//page to calculate
            double pri; //pr of page i
            double prpred = 0; //pr of page i's predecessors - represents the final answer of summation in formula
            double outd;
            
            WebPageSet predi = i->second; //webpage set representing predecessors of pagei
            //iterate through page i's predecessors to calculate pagerank of predecessors over their outdegrees -SUMMATION PART OF FORMULA
            WebPageSet::const_iterator j;
            for(j = predi.begin(); j != predi.end();
            ++j)
            {
                
                map<WebPage*, WebPageSet>::iterator pagej;
                pagej = subgraph.find(*j);//find pagej within subgraph of webpage-outgoing link, we need it's outgoing links 
                outd = pagej->second.size(); //number of outgoing links of page j within candidate set -correct
                //find place where pr of j at time t (PR(J) part of formula)
                map<WebPage*, double>::iterator prevRank = temp.find(*j);
                double pj = prevRank->second;//pr at time t of j 
                double j = pj/outd;
                
                
                prpred += j;
            }

            pri = (damp/N) + (1 - damp) * prpred;

            map<WebPage*, double>::iterator rnext = ranknext.find(ipage); //find place to store pr solution at time t+1 (pr(i) part of formula)
            rnext->second = pri;
        }

        //now update the PR from the previous iteration with the PR just calculated
        map<WebPage*, double>::iterator newr;
        for(newr = ranknext.begin(); newr != ranknext.end(); ++newr)
        {
            map<WebPage*, double>::iterator old = temp.find(newr->first); //get the webpage
            old->second = newr->second;//update the web pages pr values
        }
        
       

    }

    //iterate, and copy pr values computed in the map to output vector
    map<WebPage*, double>::const_iterator solit;
    for(solit = temp.begin(); solit != temp.end(); ++solit)
    {
        //push the webpage and its pr into vector
        prsol.push_back(make_pair(solit->first, solit->second));
    }

    return prsol;
    
}










