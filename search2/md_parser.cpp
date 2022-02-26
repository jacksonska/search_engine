#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cctype>
#include "md_parser.h"
#include "util.h"

using namespace std;

typedef enum { NORMALTEXT, LINKTEXT, ISLINK, LINKURL } PARSE_STATE_T;

//when are state transitions happening?
void MDParser::parse(std::string filename, std::set<std::string>& allSearchableTerms, std::set<std::string>& allOutgoingLinks)
{
    // Attempts to open the file.
    ifstream wfile(filename.c_str());
    if(!wfile) {
        throw invalid_argument("Bad webpage filename in MDParser::parse()");
    }

    // Remove any contents of the sets before starting to parse.
    allSearchableTerms.clear();
    allOutgoingLinks.clear();

    // The initial state is parsing a normal term.
    PARSE_STATE_T state = NORMALTEXT;

    // Initialize the current term and link as empty strings.
    string term = "";
    string link = "";

    // Get the first character from the file.
    char c = wfile.get();

    // Continue reading from the file until input fails.
    while(!wfile.fail())
    {
        // Logic for parsing a normal term.
        if(state == NORMALTEXT)
        {
            // ADD YOUR CODE HERE
        	if(isalnum(c))
        	{
        		//not special character
        		term += c;
        	}
        	else
        	{
        		//c is a special character, add to results and clear term
                term = conv_to_lower(term); //convert to a common casing
        		allSearchableTerms.insert(term);
        		term = "";

        		//if char is '[' found link
        		if(c == '[')
        			state = LINKTEXT;
        	}

        }
        // Logic for parsing a link.
        else if (state == LINKTEXT)
        {
            // ADD YOUR CODE HERE
        	if(isalnum(c))
        	{
        		//reading the text between '[]'
        		term += c;
        	}
        	else
        	{
        		//reached the end of the []
                term = conv_to_lower(term); //convert to a common casing
        		allSearchableTerms.insert(term);
        		term = "";
        		//update state
        		if(c == ']')
        			state = ISLINK;

        		
        	}


        }
        else if( state == ISLINK )
        {
            // ADD YOUR CODE HERE
        	if(isalnum(c))
        	{
        		//not a link because no '('
        		term += c;
        	}
        	else
        	{
        		//clear link
        		link = "";
        		//update state
        		if(c =='(')
        			state = LINKURL;
        		else if (c != '(')
        			state = NORMALTEXT;
        	}



        }
        // Else we are in the LINKURL state.
        else
        {
            // ADD YOUR CODE HERE
        	if(c != ')')
    		{
        		link += c;
    		}
        	else
        	{
        		//reached the ')'
                //links are case sensitive
    			allOutgoingLinks.insert(link);
    			state = NORMALTEXT;		
        	}
        }
        // Attempt to get another character from the file.
        c = wfile.get();
    }//eo while
    // ADD ANY REMAINING CODE HERE - check if the last term hasn't been added
    if(term != "")
    {
        term = conv_to_lower(term);
        allSearchableTerms.insert(term);
    }

    // Close the file.
    wfile.close();
}

std::string MDParser::display_text(std::string filename)
{
    // Attempts to open the file.
    ifstream wfile(filename.c_str());
    if (!wfile) {
        throw std::invalid_argument("Bad webpage filename in TXTParser::parse()");
    }
    std::string retval;

    // The initial state is parsing a normal term.
    PARSE_STATE_T state = NORMALTEXT;

    char c = wfile.get();

    // Continue reading from the file until input fails.
    while (!wfile.fail()) {
        // Logic for parsing a normal term.
        if (state == NORMALTEXT)
        {
            // The moment we hit a bracket, we input our current working term
            // into the allSearchableTerms set, reset the current term, and move into
            // parsing a link.
            if (c == '[')
            {
                state = LINKTEXT;
            }
            retval += c;
        }
        // Logic for parsing a link.
        else if (state == LINKTEXT)
        {
            // When we hit the closing bracket, then we must be finished getting the link.
            if (c == ']')
            {
                state = ISLINK;
            }
            retval += c;
        }
        else if (state == ISLINK) {
            if (c == '(') {
                state = LINKURL;
            }
            else
            {
                state = NORMALTEXT;
                retval += c;
            }
        }
        // Else we are in the LINKURL state.
        else
        {
            // When we hit a closing parenthese then we are done, and the link can be inserted.
            if (c == ')')
            {
                state = NORMALTEXT;
            }
        }
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
void MDParser::crawl(
    const std::map<std::string, PageParser*>& parsers,
    std::string filename,
    std::set<std::string>& processed,
    std::ostream& ofile)
{
    ifstream ifile(filename);
    if(ifile.fail())
    {
        return;
    }

    // Implemented the link searching using some of the FSM logic in Parse function above.
    PARSE_STATE_T state = NORMALTEXT;
    string link = "";
    // Get the first character from the file.
    char c = ifile.get();

    // Continue reading from the file until input fails.
    while(!ifile.fail())
    {
        // Logic for parsing a normal term.
        if(state == NORMALTEXT)
        {
            //if char is '[' found link
            if(c == '[')
                state = LINKTEXT;
            
        }
        // Logic for parsing a link.
        else if (state == LINKTEXT)
        {
            //update state when find end of link
            if(c == ']')
                state = ISLINK;
        }
        else if( state == ISLINK )
        {
            //clear link
            link = "";
            //update state
            if(c =='(')
                state = LINKURL;
            else if (c != '(')
                state = NORMALTEXT;
        }
        // Else we are in the LINKURL state.
        else
        {
            // ADD YOUR CODE HERE
            if(c != ')')
            {
                link += c;
            }
            else
            {
                //reached the ')'
                //links are case sensitive
                // ofile << link << " ";
                ifstream ffile(link);
                
                if(ffile.fail()) return; //file doesn't exist
                
                /*
                DFS STARTS HERE, ONCE YOU FIND THE LINK
                */
               //here im checking to see if the linked to file has already been processed, and then adding it to the set
               //should i be checking if the file im currently in is processed instead?
                //check to see if the file has already been processed
                set<string>::const_iterator pit = processed.find(link);
                if(pit == processed.end())
                {
                    //file has not alread been processed
                    string ext = extract_extension(link);
                    //search for parser type 
                    map<string, PageParser*>::const_iterator eit = parsers.find(ext);
                    if(eit != parsers.end())
                    {
                        //parser exists for this file type
                        processed.insert(link); //add to processed
                        ofile << link << '\n'; //print this link in the index file
                        eit->second->crawl(parsers, link, processed, ofile);
                    }
                    else
                    {
                        //no parser exists for this file type
                        return;
                    }
                }
                //if file has already been processed the while loop continues searching for the next link.
                state = NORMALTEXT;		
            }
        }
        // Attempt to get another character from the file.
        c = ifile.get();
    }//eo while

}//eo crawl function