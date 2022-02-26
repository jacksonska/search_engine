CXX = g++
FLAGS = -g -Wall --std=c++11
OBJDIR = obj

all: search-shell crawl

#webpage object
webpage.o:  webpage.cpp webpage.h
	$(CXX) $(FLAGS) -c webpage.cpp -o webpage.o

#util object
util.o:  util.cpp util.h webpage.h 
	$(CXX) $(FLAGS) -c util.cpp -o util.o

#md_parser object
md_parser.o:  md_parser.cpp md_parser.h util.h pageparser.h
	$(CXX) $(FLAGS) -c md_parser.cpp -o md_parser.o

#txt_parser object
txt_parser.o:  txt_parser.cpp txt_parser.h util.h pageparser.h
	$(CXX) $(FLAGS) -c txt_parser.cpp -o txt_parser.o

#crawl object
crawl.o:   crawl.cpp md_parser.h txt_parser.h util.h
	$(CXX) $(FLAGS) -c crawl.cpp -o crawl.o

#handler object
handler.o: handler.cpp handler.h searcheng.h
	$(CXX) $(FLAGS) -c handler.cpp -o handler.o

#cmdhandler object
cmdhandler.o: cmdhandler.cpp cmdhandler.h handler.h util.h searcheng.h webpage.h msort.h
	$(CXX) $(FLAGS) -c cmdhandler.cpp -o cmdhandler.o 

#searcheng object
searcheng.o: searcheng.cpp searcheng.h util.h pageparser.h webpage.h wpscombiner.h util.o
	$(CXX) $(FLAGS) -c searcheng.cpp -o searcheng.o

#searchui object
searchui.o: searchui.cpp searchui.h util.h searcheng.h handler.h 
	$(CXX) $(FLAGS) -c searchui.cpp -o searchui.o

#search-shell object
search-shell.o:  search-shell.cpp searcheng.h searchui.h cmdhandler.h md_parser.h txt_parser.h handler.h
	$(CXX) $(FLAGS) -c search-shell.cpp -o search-shell.o

#combiners object
combiners.o: combiners.cpp combiners.h wpscombiner.h webpage.h searcheng.h
	$(CXX) $(FLAGS) -c combiners.cpp -o combiners.o 

#crawl executable
crawl:  crawl.o md_parser.o txt_parser.o util.o webpage.o 
	$(CXX) $(FLAGS) crawl.o md_parser.o txt_parser.o util.o webpage.o -o crawl

#search-shell executable
search-shell:  handler.o combiners.o md_parser.o txt_parser.o util.o webpage.o searcheng.o searchui.o  search-shell.o cmdhandler.o
	$(CXX) $(FLAGS)  handler.o combiners.o md_parser.o txt_parser.o util.o webpage.o searcheng.o searchui.o  search-shell.o cmdhandler.o -o search-shell

#PERSONAL TEST COMPILATION
md_parser_crawl_test.o:  md_parser_crawl_test.cpp md_parser.h util.h 
	$(CXX) $(FLAGS) -c md_parser_crawl_test.cpp -o md_parser_crawl_test.o 

pctest:  md_parser_crawl_test.o md_parser.o util.o webpage.o  
	$(CXX) $(FLAGS) md_parser_crawl_test.o md_parser.o util.o webpage.o -o pctest
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

 
test-small:
	mkdir test-small

clean:
	rm -rf  pr-small/*.out pctest crawl search-shell *.o 