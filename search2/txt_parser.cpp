#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cctype>
#include "txt_parser.h"
#include "util.h"

using namespace std;

void TXTParser::parse(std::string filename, std::set<std::string>& allSearchableTerms, std::set<std::string>& allOutgoingLinks)
{
    // Attempts to open the file.
    ifstream wfile(filename.c_str());
    if (!wfile) {
        throw std::invalid_argument("Bad webpage filename in TXTParser::parse()");
    }

    // Remove any contents of the sets before starting to parse.
    allSearchableTerms.clear();
    // No links are present in a text parser
    allOutgoingLinks.clear();

    // Initialize the current term and link as empty strings.
    string term = "";
    string link = "";

    // Get the first character from the file.
    char c = wfile.get();

    // Continue reading from the file until input fails.
    while(!wfile.fail())
    {
        // Is c a character to split terms?
        if (!isalnum(c))
        {
            // If we have a term to add, convert it to a standard case and add it
            if(term != "")
            {
                term = conv_to_lower(term);
                allSearchableTerms.insert(term);
            }
            term = "";
        }
        // Otherwise we continually add to the end of the current term.
        else
        {
            term += c;
        }

        // Attempt to get another character from the file.
        c = wfile.get();
    }
    // Since the last term in the file may not have punctuation, there may be a valid term in
    // the "term" variable, so we need to insert it into the allSearchableTerms set.
    if(term != "")
    {
        term = conv_to_lower(term);
        allSearchableTerms.insert(term);
    }
    // Close the file.
    wfile.close();
}

std::string TXTParser::display_text(std::string filename)
{
    // Attempts to open the file.
    ifstream wfile(filename.c_str());
    if (!wfile) {
        throw std::invalid_argument("Bad webpage filename in TXTParser::parse()");
    }
    std::string retval;

    char c = wfile.get();

    // Continue reading from the file until input fails.
    while (!wfile.fail()) {
        retval += c;
        c = wfile.get();
    }
    return retval;
}

 /**
     * Crawls a webpage/file by performing a DFS on the links in the file
     * in the order they appear, writing each linked filename to an output
     * (index) file, provided it exists, has not been processed before,
     * and has a parser available.
     *
     * Upon invocation, a valid file should be added to the processed set
     * and output stream and a DFS of any of its links should be performed.
     * Otherwise (for an invalid file), simply return.
     *
     * @param[in] parsers
     *   Map of extensions to the PageParser for each extension/file type
     * @param[in] filename
     *   The file to be crawled
     * @param[inout] processed
     *   The set of files on which DFS has already started (i.e. the files
     *   already in process)
     * @param[out] ofile
     *   Output stream to write the order of processed files
     *
     */
void TXTParser::crawl(
    const std::map<std::string, PageParser*>& parsers,
    std::string filename,
    std::set<std::string>& processed,
    std::ostream& ofile)
{
    
    //not sure any of this will actually execute?

    ifstream ifile(filename); //ifstream is just used to check if the filename provided exists
    if(ifile.fail()) return; //a check to see if the file exists
    //check if the file was added to the processed 
    set<string>::const_iterator pit = processed.find(filename);
    if(pit == processed.end())
    {
        //this file is not in the processed set, add it
        map<string, PageParser*>::const_iterator eit = parsers.find("txt");
        if(eit != parsers.end())
        {
            //txt parser exists for txt file, then add to processed.
            processed.insert(filename);
        }
    }
}