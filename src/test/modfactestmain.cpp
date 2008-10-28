#include "modfactestmain.h"

int main (int argc, char **argv)
{
	// define & init vars
	ModuleRegistry mr;
	ModuleLoader ml;

	ModuleFactoryBase *foo = NULL;
	Module *bar = NULL;
	std::string configFile = "config.xml";
	// seems to be needed on BSD systems
	// LD_LIBRARAY_PATH behaves weird
	//char *sharedobject = "/export/home/lxs/share/diploma/src/modules/testmodule1.so";
	char *sharedobject = "testmodule1.so";

#if defined __GNUC__
	std::cout << "Compiled with gcc major: " << __GNUC__ << std::endl;
#endif

    // dummy code to overcome compiler warnings
    if (argc == 0)
        (void) strlen(argv[0]);

	//  open shared object
	try {
		ml.open(sharedobject);
	}
	catch (Deception::NullPointerException &e) {
		// XXX: icc warning:
		// remark #981: operands are evaluated in unspecified order
		std::cerr << "NillPointerException Caught " << e.toString() << std::endl;
		exit(EXIT_FAILURE);
	}

	// find a factory
	foo = mr.find(1);
	if (foo == NULL) {
		std::cerr << "module not found 'foo' -eq NULL\n";
		exit(EXIT_FAILURE);
	}
	//std::cout << "main: foo @" << foo << std::endl;

	// use the factory
	bar = foo->createModObject();
	if (bar == NULL) {
		std::cerr << "module not found 'bar' -eq NULL\n";
		exit(EXIT_FAILURE);
	}

	// use the created object
	bar->modMain();	

	// close the shared object
try {
		ml.close(sharedobject);
	}
	catch (Deception::NullPointerException &e) {
		std::cerr << "NillPointerException Caught " << e.toString() << std::endl;
		exit(EXIT_FAILURE);
	}

	return 0;
}
