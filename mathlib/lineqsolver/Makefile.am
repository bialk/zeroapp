bin_PROGRAMS = solv_lineq
solv_lineq_SOURCES = solv_lineq.cpp

if WIN32
solv_lineq_CPPFLAGS = -w -m32 -I../.. -I$(HOME)/include
solv_lineq_LDFLAGS =  -m32 -L$(HOME)/lib
solv_lineq_LDADD =  $(LIBS) -lpthreadGCE2
else
solv_lineq_CPPFLAGS = -w -m32 -I../.. -I$(HOME)/include
solv_lineq_LDFLAGS = -m32 -L$(HOME)/lib
endif
