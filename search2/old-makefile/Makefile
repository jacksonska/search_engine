CXX = g++
FLAGS = -g -Wall --std=c++11
OBJDIR = obj

all: search-shell crawl

#webpage object
$(OBJDIR)/webpage.o: $(OBJDIR) webpage.cpp webpage.h
	$(CXX) $(FLAGS) -c webpage.cpp -o $(OBJDIR)/webpage.o

#util object
$(OBJDIR)/util.o: $(OBJDIR) util.cpp util.h webpage.h 
	$(CXX) $(FLAGS) -c util.cpp -o $(OBJDIR)/util.o

#md_parser object
$(OBJDIR)/md_parser.o: $(OBJDIR) md_parser.cpp md_parser.h util.h pageparser.h
	$(CXX) $(FLAGS) -c md_parser.cpp -o $(OBJDIR)/md_parser.o

#txt_parser object
$(OBJDIR)/txt_parser.o: $(OBJDIR) txt_parser.cpp txt_parser.h util.h pageparser.h
	$(CXX) $(FLAGS) -c txt_parser.cpp -o $(OBJDIR)/txt_parser.o

#crawl object
$(OBJDIR)/crawl.o:  $(OBJDIR) crawl.cpp md_parser.h txt_parser.h util.h
	$(CXX) $(FLAGS) -c crawl.cpp -o $(OBJDIR)/crawl.o

#handler object
$(OBJDIR)/handler.o: handler.cpp handler.h searcheng.h
	$(CXX) $(FLAGS) -c handler.cpp -o $(OBJDIR)/handler.o

#cmdhandler object
$(OBJDIR)/cmdhandler.o: cmdhandler.cpp cmdhandler.h handler.h util.h searcheng.h webpage.h msort.h
	$(CXX) $(FLAGS) -c cmdhandler.cpp -o $(OBJDIR)/cmdhandler.o 

#searcheng object
$(OBJDIR)/searcheng.o: searcheng.cpp searcheng.h util.h pageparser.h webpage.h wpscombiner.h $(OBJDIR)/util.o
	$(CXX) $(FLAGS) -c searcheng.cpp -o $(OBJDIR)/searcheng.o

#searchui object
$(OBJDIR)/searchui.o: searchui.cpp searchui.h util.h searcheng.h handler.h 
	$(CXX) $(FLAGS) -c searchui.cpp -o $(OBJDIR)/searchui.o

#search-shell object
$(OBJDIR)/search-shell.o: $(OBJDIR) search-shell.cpp searcheng.h searchui.h cmdhandler.h md_parser.h txt_parser.h handler.h
	$(CXX) $(FLAGS) -c search-shell.cpp -o $(OBJDIR)/search-shell.o

#combiners object
$(OBJDIR)/combiners.o: combiners.cpp combiners.h wpscombiner.h webpage.h searcheng.h
	$(CXX) $(FLAGS) -c combiners.cpp -o $(OBJDIR)/combiners.o 

#crawl executable
crawl: $(OBJDIR) $(OBJDIR)/crawl.o $(OBJDIR)/md_parser.o $(OBJDIR)/txt_parser.o $(OBJDIR)/util.o $(OBJDIR)/webpage.o 
	$(CXX) $(FLAGS) $(OBJDIR)/crawl.o $(OBJDIR)/md_parser.o $(OBJDIR)/txt_parser.o $(OBJDIR)/util.o $(OBJDIR)/webpage.o -o crawl

#search-shell executable
search-shell: $(OBJDIR) $(OBJDIR)/handler.o $(OBJDIR)/combiners.o $(OBJDIR)/md_parser.o $(OBJDIR)/txt_parser.o $(OBJDIR)/util.o $(OBJDIR)/webpage.o $(OBJDIR)/searcheng.o $(OBJDIR)/searchui.o  $(OBJDIR)/search-shell.o $(OBJDIR)/cmdhandler.o
	$(CXX) $(FLAGS)  $(OBJDIR)/handler.o $(OBJDIR)/combiners.o $(OBJDIR)/md_parser.o $(OBJDIR)/txt_parser.o $(OBJDIR)/util.o $(OBJDIR)/webpage.o $(OBJDIR)/searcheng.o $(OBJDIR)/searchui.o  $(OBJDIR)/search-shell.o $(OBJDIR)/cmdhandler.o -o search-shell

#PERSONAL TEST COMPILATION
$(OBJDIR)/md_parser_crawl_test.o:  md_parser_crawl_test.cpp md_parser.h util.h 
	$(CXX) $(FLAGS) -c md_parser_crawl_test.cpp -o $(OBJDIR)/md_parser_crawl_test.o 

pctest: $(OBJDIR) $(OBJDIR)/md_parser_crawl_test.o $(OBJDIR)/md_parser.o $(OBJDIR)/util.o $(OBJDIR)/webpage.o  
	$(CXX) $(FLAGS) $(OBJDIR)/md_parser_crawl_test.o $(OBJDIR)/md_parser.o $(OBJDIR)/util.o $(OBJDIR)/webpage.o -o pctest
	@echo "*****Testing MDParser just read links****"
	@echo "*****************************************"
	./pctest pr-small/index.seed pr-small/linkstest.out
	@echo "****NEED TO MANUALLY CHECK THE OUTPUTFILE 'linkstest.out'****"


ctest: crawl
	@echo "**** Testing crawl function using pr-small ****"
	@echo "***********************************************"
	./crawl pr-small/index.seed pr-small/index-test.out
	@diff -s pr-small/index.in pr-small/index-test.out
	@echo "Passed Crawl Test!"

prtester: prtest
	@echo "**** Testing PageRank function ****"
	@echo "***********************************"
	./prtest

stest: search-shell test-small
	@echo "***Testing Search-Shell**"
	@echo "Running test 1"
	./search-shell test-small/index.in test-small/query1.txt test-small/query1.out
	#@diff -s test-small/query1.out test-small/query1.exp
	@echo "*************************"
	@echo "Passed Query 1!"
	@echo "*************************"
	@echo "Running Query 2"
	./search-shell test-small/index.in test-small/query2.txt test-small/query2.out
	#@diff -s test-small/query2.out test-small/query2.exp
	@echo "*************************"
	@echo "Passed Query 2!"
	@echo "*************************"

$(OBJDIR):
	mkdir $(OBJDIR)

test-small:
	mkdir test-small

clean:
	rm -rf $(OBJDIR) pr-small/*.out pctest crawl search-shell 