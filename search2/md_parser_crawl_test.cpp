#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <set>
#include "md_parser.h"
// #include "txt_parser.h"
#include "util.h"

using namespace std;
int main(int argc, char* argv[])
{   
    if(argc < 3)
    {
        cout << "Must provide an index file and output file" << endl;
        return 1;
    }
 //read index file
    string seed = argv[1]; //input file
    string index = argv[2]; //output file
    string currf;

    PageParser* md = new MDParser;

    map<string, PageParser*> parsers;
    //make map of supported parsers for dfs
    parsers.insert(make_pair("md", md));
    set<string>processed;

    ofstream ofile(index);
    if(ofile.fail())
    return -2; //failed to open the output file
    
    ifstream ifile(seed);
    if(ifile.fail()) return -1;//failed to open the seed file

    getline(ifile, currf);

    md->crawl(parsers, currf, processed, ofile); //should cout "pr-small/c.md pr-small/d.md "

    // getline(ifile, currf);
    
    // md->crawl(parsers, currf, processed, ofile); //should print pr-small/c.md pr-small/f.md " 

    return 0;
}