#ifndef COMBINERS_H
#define COMBINERS_H

#include "wpscombiner.h"

/*** Define Derived WebPageSetCombiners (for AND, OR, DIFF) classes below ***/

class ANDWebPageSetCombiner : public WebPageSetCombiner
{
public:
    virtual
    WebPageSet combine(const WebPageSet& setA, const WebPageSet& setB);
    ~ANDWebPageSetCombiner();
};

class ORWebPageSetCombiner : public WebPageSetCombiner
{
public:
    virtual
    WebPageSet combine(const WebPageSet& setA, const WebPageSet& setB);
    ~ORWebPageSetCombiner();
};

class DiffWebPageSetCombiner : public WebPageSetCombiner
{
public:
    virtual
    WebPageSet combine(const WebPageSet& setA, const WebPageSet& setB);
    ~DiffWebPageSetCombiner();
};


// MY IMPLEMENTATIONS
/*** Define Derived WebPageSetCombiners (for AND, OR, DIFF) classes below ***/
//     //combiners must run in O(nlogn) not O(n^2)

// class AndWebPageSetCombiner : public WebPageSetCombiner
// {

// public:
//     ~AndWebPageSetCombiner();

//     WebPageSet combine(const WebPageSet& setA, const WebPageSet& setB);

// };


// class OrWebPageSetCombiner : public WebPageSetCombiner
// {
// public:

//     ~OrWebPageSetCombiner();

//     WebPageSet combine(const WebPageSet& setA, const WebPageSet& setB);

// };

// class DiffWebPageSetCombiner : public WebPageSetCombiner
// {
// public:
    
//     ~DiffWebPageSetCombiner();

//     WebPageSet combine(const WebPageSet& setA, const WebPageSet& setB);

// };


#endif
