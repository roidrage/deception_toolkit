#include <map>
#include <string>
#include <iostream>
#include <fstream>

#include <cstdio>
#include <unistd.h>
#include <string.h>
#include <termios.h>
#include <pcre.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "sttd.h"

class FSM
{
	public:
		void changeState(unsigned int stateNum);
		void getStates(std::string file, unsigned int stateNum);
		void init(std::string file);
		void parse(std::string input);
		void respond(STTD *entry);
		void dtkSpecial(std::string command);

		~FSM(void) {};
		FSM(void) {};

	private:
		typedef std::map<std::string,STTD*> STT;
		typedef STT::iterator STTIterator;
		STT actionMatch;
		static unsigned int curLoop;
		static unsigned int curState;
		static unsigned int delay;
		static bool debug;
		STT dtkMatch;
		static unsigned int maxloops;
		STT patternMatch;
		static unsigned int timeout;
		STT wordMatch;

		static FSM *states[6];
};
