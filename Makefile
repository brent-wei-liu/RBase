CXXFLAGS = -O2 -fmessage-length=0
#CXXFLAGS = -O -g -fmessage-length=0 -fno-inline -Wall

VPATH = .:rtree:common:BBS:BBSdyn

ODIR=obj


ALL_OBJS = ${RTREE_OBJS} ${COMMON_OBJS} ${BBS_OBJS} ${BBSDYN_OBJS}



_MAIN_OBJS = $(ALL_OBJS) main.o
MAIN_OBJS = $(patsubst %,$(ODIR)/%,$(_MAIN_OBJS))

CLUSTEREDGEN_OBJS = $(ODIR)/clusteredGen.o
ZIPFGEN_OBJS = $(ODIR)/zipfGen.o
CLUSTERZIPFGEN_OBJS = $(ODIR)/clusterZipfGen.o
CREATEBATCH_OBJS = $(ODIR)/createBatch.o


OBJS = $(MAIN_OBJS) $(CLUSTEREDGEN_OBJS) $(ZIPFGEN_OBJS) $(CLUSTERZIPFGEN_OBJS) $(CREATEBATCH_OBJS)

#HEADERS = ${OBJS,.o=.h}



$(ODIR)/%.o : %.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $<


COMMON_OBJS = common.o skyline.o timing.o simpleBuffer.o
RTREE_OBJS = linlist.o heap.o rand.o myhilbert.o hilbert.o blk_file.o cache.o rtree.o rtnode.o entry.o global.o
BBS_OBJS = BBS_rtree.o BBS.o
BBSDYN_OBJS = BBSdyn_rtree.o BBSdyn.o


#LIBS =  -lboost_filesystem-gcc-mt -lboost_program_options-gcc-mt    # for windows with cygwin
LIBS =  -lboost_filesystem -lboost_system -lboost_program_options     # for macosx
#LIBS =  -lboost_filesystem -lboost_program_options     # for unix/linux

TARGETS = $(MAIN) $(CLUSTEREDGEN) $(ZIPFGEN) $(CLUSTERZIPFGEN) $(CREATEBATCH)

MAIN = main
CLUSTEREDGEN = clusteredGen
ZIPFGEN = zipfGen
CLUSTERZIPFGEN = clusterZipfGen
CREATEBATCH = createBatch



$(MAIN): $(MAIN_OBJS)
	$(CXX) -o $(MAIN) $(MAIN_OBJS) $(LIBS)

$(CLUSTEREDGEN): $(CLUSTEREDGEN_OBJS)
	$(CXX) -o $(CLUSTEREDGEN) $(CLUSTEREDGEN_OBJS) $(LIBS)

$(ZIPFGEN): $(ZIPFGEN_OBJS)
	$(CXX) -o $(ZIPFGEN) $(ZIPFGEN_OBJS) $(LIBS)
	
$(CLUSTERZIPFGEN): $(CLUSTERZIPFGEN_OBJS)
	$(CXX) -o $(CLUSTERZIPFGEN) $(CLUSTERZIPFGEN_OBJS) $(LIBS)

$(CREATEBATCH): $(CREATEBATCH_OBJS)
	$(CXX) -o $(CREATEBATCH) $(CREATEBATCH_OBJS) $(LIBS)

all: $(TARGETS)

cleanup:
	rm -f $(OBJS) $(TARGETS)
