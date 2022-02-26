#include "searcheng.h"
#include "combiners.h"
// Place your function implementations for your derived
// WebPageSetCombiner classes here

ANDWebPageSetCombiner::~ANDWebPageSetCombiner()
{

}
WebPageSet ANDWebPageSetCombiner::combine(const WebPageSet& setA, const WebPageSet& setB)
{
    //combining all elements that are in both setA and setB and placing into a new setC
    WebPageSet setC;
    WebPageSet::const_iterator iA; //= setA.begin();
    WebPageSet::const_iterator iB;// = setB.begin();

    //iterate through the sets and find the terms that are in both
    for(iA = setA.begin(); iA != setA.end(); ++iA)
    {
    	//std::string filename = iA->filename();
    	
    	//check if webpage in A is in B
    	iB = setB.find(*iA); 
    	
    	if(iB != setB.end())
    	{
    		//webpage is in setB
    		setC.insert(*iA);
    	}

    }

    return setC; 
}

WebPageSet ORWebPageSetCombiner::combine(const WebPageSet& setA, const WebPageSet& setB)
{
	//combining all elements that are in both setA and setB and returning. 
	//IE just return all the elements in setA and setB
	WebPageSet setC = setA;
	
	//WebPageSet::const_iterator iC;
    WebPageSet::const_iterator iB; 

   //iterate through setB and insert into setC
    for(iB = setB.begin(); iB != setB.end(); ++iB)
    {
    	setC.insert(*iB);
    }
    
    return setC;
}

ORWebPageSetCombiner::~ORWebPageSetCombiner()
{
	
}


 WebPageSet DiffWebPageSetCombiner::combine(const WebPageSet& setA, const WebPageSet& setB)
 {
 	//assume setA is the set of pages which have the term user asked for
 	//so, copy setA into setC and delete elements in setC which are in setB
 	WebPageSet setC = setA;

 	WebPageSet::const_iterator iC;
 	WebPageSet::const_iterator iB;

 	//iterate through setB and remove the elements in setC that are in setB
 	for(iB = setB.begin(); iB != setB.end(); ++iB)
 	{
 		//search setC for fileB
 		iC = setC.find((*iB));
 		
 		if(iC != setC.end())
 		{
 			//the file in setB exists in setC - erase it
 			setC.erase(iC);
 		}
 	}

 	return setC;
 }

DiffWebPageSetCombiner::~DiffWebPageSetCombiner()
{
	
}



