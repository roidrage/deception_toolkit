MODULES=\
	modules/dtk-port.so		\
	dtk-script				\
	modules/testmodule1.so	\
	modules/testmodule2.so	\
	$(NULL)

DTKSCRIPTOBJS=\
	moduleoptions.o				\
	modules/dtk-script.o		\
	modules/dtk-scriptfsm.o		\
	modules/dtk-scriptstatetabledata.o	\
	$(NULL)

OBJECTS=\
	moduleregistry.o			\
	moduleregistrydata.o		\
	moduleconfig.o				\
	moduleconfighandler.o		\
	modulefactorybase.o			\
	moduleoptions.o				\
	moduleloader.o				\
	inputstream.o				\
	deceptiond.o				\
	exception.o					\
	stricmp.o					\
	socket.o					\
	logging.o					\
	signals.o					\
	fw_pcap.o					\
	$(NULL)


COMMON_DEFS=-D`uname -s` -DDEBUG #-DDO_MCHECK
COMMON_LIBS=-ldl -lxerces-c -lpthread -lpcap
INCLUDES=


all: deceptiond modules

deceptiond: $(OBJECTS)
	@echo; echo 'Linking ---> $@'
	$(CC) $(LDFLAGS) $(OBJECTS) $(LIBDIRS) $(LIBS) \
	-o $@

modules: $(MODULES)

dtk-script: $(DTKSCRIPTOBJS)
	@echo; echo 'Linking ---> $@'
	$(CC) $(MODULE_CFLAGS) $(MODULE_LDFLAGS) $(LIBDIRS) $(DTKSCRIPTOBJS) \
		-o modules/$@.so -lpcre

clean:
	@echo; echo 'Cleaning...'
	rm -rf deceptiond *.o modules/*.so modules/*.o test/*.o


.SUFFIXES: .cpp .so .o
.cpp.o:
	@echo; echo 'Compiling ---> $*.o'
	$(CC) $(CFLAGS) -c $< -o $*.o
.cpp.so:
	@echo; echo 'Compiling ---> $*.o'
	$(CC) $(MODULE_CFLAGS) $(CFLAGS) -c $< -o $*.o
	@echo; echo 'Linking ---> $@'
	$(LD) $(MODULE_LDFLAGS) $*.o -o $@
