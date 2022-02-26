#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <set>
#include "md_parser.h"
#include "txt_parser.h"
#include "util.h"

using namespace std;

int main(int argc, char* argv[])
{
    if (argc < 3) {
        cout << "Must provide an index file and output file" << endl;
        return 1;
    }

    // ---------------------- [TO BE COMPLETED] -------------------------------    
    //read index file
    string seed = argv[1]; //input file
    string index = argv[2]; //output file
    string currf;

    PageParser* md = new MDParser;
    PageParser* txt = new TXTParser;

    map<string, PageParser*> parsers;
    //make map of supported parsers for bfs
    parsers.insert(make_pair("md", md));
    parsers.insert(make_pair("txt", txt));

    set<string> processed; 

    ifstream ifile(seed);
    if(ifile.fail()) throw invalid_argument("Bad file name for Seed file, crawl.cpp::main()");

    ofstream ofile(index);
    if(ofile.fail()) throw invalid_argument("Bad file name for Output file, crawl.cpp::main()");
    
    /* 
    TO do the DFS (in MDParser function), read seed, if find link call crawl on it? Or use stack?, and add it to visited/print in output file
    need to extract the extension of the linked file and call crawl on that using the parsers map
    */

    //read in all the seed files and crawl them using their parsers
    while(ifile >> currf)
    {
        if(!ifile.fail())
        {
            //seed file was read properly
            set<string>::const_iterator pit = processed.find(currf);
            if(pit == processed.end()) 
            {
                //file has not already been processed.
                string ext = extract_extension(currf);
                //search the parsers for this file types parser
                map<string, PageParser*>::const_iterator eit = parsers.find(ext); 
                if(eit != parsers.end()) 
                {
                    //parser for file exists, so crawl the current file
                    //make sure to update processed/output file
                    processed.insert(currf);
                    ofile << currf << '\n';
                    eit->second->crawl(parsers, currf, processed, ofile);
                }
            }
        }
    }

    // You may add cleanup code here if necessary

    return 0;
}
