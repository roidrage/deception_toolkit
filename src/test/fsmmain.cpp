#include <iostream>
#include <string>

#include <cstdio>

#include "fsm.h"

int main (int argc, char **argv)
{
	std::string input;
	std::string file = "./10021.response";
	char buf[1024];

	if (argc > 1)
		file = argv[1];

	FSM fsm;
	fsm.init(file);

	// XXX: integrating into deceptiond framework
	// timeouts can be set via the InputStream obj
	// '>>' tokenizes using \s ;( this should be different using
	// the InputStream
	while(1) {
		std::cin.getline(buf, 1023, '\n');
		if (buf[0] == 0)
			break;
		input = buf;
		fsm.parse(input);
	}

	return 0;
}
