#ifndef CMDHANDLER_H
#define CMDHANDLER_H
#include "handler.h"
#include "searcheng.h"

/**
 * Handles the QUIT command - COMPLETED in cmdhandler.cpp
 */
class QuitHandler : public Handler
{
public:
    QuitHandler();
    QuitHandler(Handler* next);

protected:

    virtual bool canHandle(const std::string& cmd) const;
    virtual HANDLER_STATUS_T process(SearchEng* eng, std::istream& instr, std::ostream& ostr);
};

/*********** OTHER CLASSES WILL NEED TO BE COMPLETED IN cmdhandler.cpp *******/
/***********  You will also need to declare the classes for handling   *******/
/***********          the AND, OR, and DIFF command handlers           *******/

/**
 * Handles the PRINT command
 */
class PrintHandler : public Handler
{
public:
    PrintHandler();
    PrintHandler(Handler* next);

protected:

    bool canHandle(const std::string& cmd) const;
    HANDLER_STATUS_T process(SearchEng* eng, std::istream& instr, std::ostream& ostr);
};

/**
 * Handles the INCOMING command
 */
class IncomingHandler : public Handler
{
public:
    IncomingHandler();
    IncomingHandler(Handler* next);

protected:

    bool canHandle(const std::string& cmd) const;
    HANDLER_STATUS_T process(SearchEng* eng, std::istream& instr, std::ostream& ostr);
};

/**
 * Handles the OUTGOING command
 */
class OutgoingHandler : public Handler
{
public:
    OutgoingHandler();
    OutgoingHandler(Handler* next);

protected:

    bool canHandle(const std::string& cmd) const;
    HANDLER_STATUS_T process(SearchEng* eng, std::istream& instr, std::ostream& ostr);
};

/****************************************************************************/
/* Declare the handler classes for AND, OR, and DIFF here                   */
/****************************************************************************/

class ANDHandler : public Handler
{
public:
    ANDHandler();
    ~ANDHandler();
    ANDHandler(Handler* next, WebPageSetCombiner* combiner);
protected:
    bool canHandle(const std::string& cmd) const;
    HANDLER_STATUS_T process(SearchEng* eng, std::istream& instr, std::ostream& ostr);

private:
   WebPageSetCombiner* combiner_;
};

class ORHandler : public Handler
{
public:
    ORHandler();
    ~ORHandler();
    ORHandler(Handler* next, WebPageSetCombiner* combiner);

protected:
     bool canHandle(const std::string& cmd) const;
     HANDLER_STATUS_T process(SearchEng* eng, std::istream& instr, std::ostream& ostr);

private:
    WebPageSetCombiner* combiner_;

};

class DIFFHandler : public Handler
{
public:
    DIFFHandler();
    ~DIFFHandler();
    DIFFHandler(Handler* next, WebPageSetCombiner* combiner);

protected:
    bool canHandle(const std::string& cmd) const;
    HANDLER_STATUS_T process(SearchEng* eng, std::istream& istr, std::ostream& ostr);
private:
    WebPageSetCombiner* combiner_;
};

class PRORHandler : public Handler
{
public:
    PRORHandler();
    ~PRORHandler();
    PRORHandler(Handler* next, WebPageSetCombiner* combiner);

protected:
    bool canHandle(const std::string& cmd) const;
    HANDLER_STATUS_T process(SearchEng* eng, std::istream& istr, std::ostream& ostr);
private:
    WebPageSetCombiner* combiner_;
};

class PRANDHandler : public Handler
{
public:
    PRANDHandler();
    ~PRANDHandler();
    PRANDHandler(Handler* next, WebPageSetCombiner* combiner);

protected:
    bool canHandle(const std::string& cmd) const;
    HANDLER_STATUS_T process(SearchEng* eng, std::istream& istr, std::ostream& ostr);
private:
    WebPageSetCombiner* combiner_;
};
#endif





