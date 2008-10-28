// C Headers
#ifdef DO_MCHECK
#include <mcheck.h>
#endif

#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

// Project Headers
#include "socket.h"
#include "modfactestmain.h"
#include "moduleconfig.h"
#include "logging.h"
#include "signals.h"

#include "exception.h"
#include "bindexception.h"
#include "socketexception.h"
#include "noclientexception.h"
#include "nullpointerexception.h"

// global logging object
Deception::Logging gLog;

int main (int argc, char **argv)
{
#ifdef DO_MCHECK
	mcheck(NULL);
	mtrace();
#endif
	// define & init vars
	Deception::Config mc;
	Deception::ModuleRegistry mr;
	Deception::ModuleLoader ml;
	Deception::Socket *sockobj;
	Deception::Module *mod;
	unsigned int modPortNum = 0;
	pid_t child;

	std::string configFile = "deceptiond.xml";
	gLog.setLogFile("/tmp/deceptiond.log");
	gLog.init();
	// set our nice signal handler
	// no more zombies in here
	try {
		setSigHandler(SIGCHLD, chldHandler);
	} catch (Exception &e) {
		std::cout << e.toString() << std::endl;
	}
#if defined __GNUC__
	// print gcc version info
	std::cout << "Compiled with gcc major: " << __GNUC__ << std::endl;
#endif

    // dummy code to overcome compiler warnings
    if (argc == 2)
		modPortNum = atoi(argv[1]);
        

	// read config
	mc.read(configFile);

	// load modules
	ml.loadAllModules();

	// create sockets
	Deception::ModuleRegistry::ModuleRegistryMapIterator itBegin;
	fd_set sockFdSet, selectSet;
	FD_ZERO(&sockFdSet);
	int maxfd = 0;

	// create a socket for every port and put the sockobj into the registry
	for (itBegin = mr.begin(); itBegin != mr.end(); itBegin++) {
		unsigned int port = (*itBegin).first;
		std::string ipaddr = (*itBegin).second->getIpAddr();

		/*
		 * XXX:
		 * the following line will give the following gcc error:

test/modconftestmain.cpp: In function `int main(int, char **)':
test/modconftestmain.cpp:60: assignment of read-only member `pair<const unsigned int,Deception::ModuleRegistryData *>::first'
make: *** [test/modconftestmain.o] Error 1

		* still it appears strange, that even though
		* ModuleRegistry::data is private we have read-access to it via
		* the iterators.
		* i'm not quite sure if whether that is a violation of the
		* encapsulation concepts.  it shouldn't be, since there are so
		* many great programmers out there who should've found that
		* violoation.  just wondering by myself.
		*/
		//(*itBegin).first = 31337;

		// this is okay, where not losing the pointer to the new()'ed
		// memory, since its address is stored in the static registry
		// member of ModuleRegistry::mr !
		sockobj = new Deception::Socket(ipaddr,port);
		std::cout << "creating socket for "
			<< ipaddr << ":" << port << std::endl;
		try {
			sockobj->init();
		}
		catch (NullPointerException &e) {
			std::cerr << "NullPointerException Caught " << std::endl;
			exit(EXIT_FAILURE);
		}
		catch (SocketException &e) {
			std::cerr << "SocketException Caught " << e.toString() << std::endl;
			exit(EXIT_FAILURE);
		}
		catch (BindException &e) {
			std::cerr << "BindException Caught " << e.toString() << std::endl;
			exit(EXIT_FAILURE);
		}
		catch (...) {
			std::cerr << "Unkown Exception Caught " << std::endl;
			exit(EXIT_FAILURE);
		}
		maxfd = sockobj->getFd();
		FD_SET(maxfd, &sockFdSet);
		mr.insert(port, sockobj);
	}
	sockobj = NULL;

	// nifty animation data
	//char *anim = "|/-\\";
	char *anim = "/^\\v";

	struct timeval timeout = { 0, 0 };
	// main event-loop
	for(int i = 0; ; i++) {
		// NOTE:
		// timeout and selectSet are modified by select() and therefore
		// need to be reset before another call to select()!
		timeout.tv_usec = 100000;
		selectSet = sockFdSet;

		// show nifty animation
		std::cout << "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b"
			<< "waiting for connect " << anim[i%strlen(anim)]
			<< std::flush;

hereher:
		// goto is just temporary, because select sets errno to EINTR (interrupted
		// system call), if a child exited, that's weird and has to be debugged since
		// goto sucks
		int n = select(maxfd + 1, &selectSet, NULL, NULL, &timeout);
		switch (n) {
			case -1:
				if (errno == EINTR) {
					goto hereher;
				} else {
					std::cerr << "select error!" << strerror(errno) << std::endl;
				}
				break;
			case 0:
				continue;
				break;
			default:
				std::cout << n << "fd's are ready\n";
				break;
		} // end switch (n)

		// test which fd's are ready 
		for (itBegin = mr.begin(); itBegin != mr.end(); itBegin++) {
			sockobj = (*itBegin).second->getSocket();

			if (FD_ISSET(sockobj->getFd(), &selectSet) == 0)
				continue;

			std::cout << "ready fd " << sockobj->getFd() << std::endl;
			try {
				sockobj->doAccept();
				gLog << gLog.getTime() << Deception::Info << "client " << sockobj->getClientAddress()
					<< " has connected" << std::endl;

				child = fork();
				switch (child) {
					// Error
					case -1:
						std::cerr << "Error spawning module\n";
						break;
					// Child (Module)
					case 0: { //  needed to over come compiler warning
						// TODO: child cleanup

						// define & init
						Deception::ModuleFactoryBase *fb = NULL;
						mod = NULL;

						// find by portNum
						fb = mr.find(sockobj->getPort());
						if (fb == NULL) {
							std::cerr << "module not found 'fb' -eq NULL\n";
							exit(EXIT_FAILURE);
						}
						std::cout << "main: fb @" << fb << std::endl;

						mod = fb->createModObject();
						if (mod == NULL) {
							std::cerr << "module not found 'mod' -eq NULL\n";
							exit(EXIT_FAILURE);
						}

						// test module
						mod->modMain(sockobj);

						try {
							// close connection
							if(sockobj->isConnected()) {
								std::cout << " child closing connection\n";
								sockobj->close();
							}
						}
						catch (SocketException &s) {
							std::cerr << "SocketException caught! " << s.toString()
								<< std::endl;;
						}
						catch (...) {
							std::cerr << "UnknownException caught! \n";
						}
						return 0;
							} break; // End Child
					// Parent
					default:
							// close connection
							if(sockobj->isConnected()) {
								std::cout << " parent closing connection\n";
								sockobj->close();
							}
						break; // End Parent
				}


			}
			catch (NoClientException &s) {
				std::cerr << "NoClientException caught! " << s.toString()
					<< std::endl;;
			}
		} // end iterating socket for-loop

	}// end for(;;) main-loop


#ifdef DO_MCHECK
	muntrace();
#endif
	// never reached.
	return 0;
}
