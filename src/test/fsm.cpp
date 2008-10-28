#include "fsm.h"

FSM::FSM *FSM::states[6] = { NULL };
unsigned int FSM::curState = 0;
unsigned int FSM::curLoop = 0;
unsigned int FSM::maxloops = 65535;
unsigned int FSM::timeout = 0;
unsigned int FSM::delay = 0;
bool FSM::debug = false;

void FSM::changeState(unsigned int stateNum)
{
	this->curState = stateNum;

	this->states[stateNum]->dtkSpecial("START");
	return;
}

void FSM::dtkSpecial(std::string command)
{
	if (this->dtkMatch.find(command) == this->dtkMatch.end()) {
		//std::cout << command << " not found in stateobj " << this->curState << " located @" << this <<std::endl;
		return;
	}

	this->respond(this->dtkMatch[command]);

	return;
}

void FSM::respond(STTD *entry)
{
	struct termios term, oterm;
	// let the sucker wait ;)
	if (this->delay > 0)
		sleep(this->delay);

	// check the operation field
	if (entry->operation.compare("@") == 0) {
		this->init(entry->response);
	} else
	// check the operation field
	if (entry->operation.compare("-echo") == 0) {
		int input = STDIN_FILENO;
		if (tcgetattr(input, &oterm) == 0) {
			memcpy(&term, &oterm, sizeof(term));
			if (term.c_lflag & ECHO)
				term.c_lflag &= ~ECHO;
			(void)tcsetattr(input, TCSANOW|TCSAFLUSH, &term);
		} else {
			memset(&term, 0, sizeof(term));
			memset(&oterm, 0, sizeof(oterm));
		}
		//system("stty -echo");
		std::cout << entry->response ; //<< "\r\n"; //<< std::endl;
	} else 
	// check the operation field
	if (entry->operation.compare("+echo") == 0) {
		int input = STDIN_FILENO;
		if (tcgetattr(input, &oterm) == 0) {
			memcpy(&term, &oterm, sizeof(term));
			if (!(term.c_lflag & ECHO))
				term.c_lflag |= ECHO;
			(void)tcsetattr(input, TCSANOW|TCSAFLUSH, &term);
		} else {
			memset(&term, 0, sizeof(term));
			memset(&oterm, 0, sizeof(oterm));
		}
		//system("stty echo");
		std::cout << entry->response ; //<< "\r\n"; //<< std::endl;
	} else 
	// check the operation field
	if (entry->operation.compare("exec") == 0) {
		std::string program = entry->response;

		char *(args[]) = {
			const_cast<char *>(entry->response.c_str()),
			NULL,
		};
		pid_t child = fork();
		switch (child) {
			case 0:
				execve(program.c_str(), args, NULL);
				break;
			default: // parent
				int status;
				waitpid(child, &status, 0);
		}
	} else
	// check the operation field
	if (entry->operation.compare("cat") == 0) {
		std::string line;
		std::ifstream responseFile(entry->response.c_str());
		if (!responseFile)
			std::cerr << "could not open responseFile: " << entry->response << std::endl;
		// FIXME: static buffer
		char buf[1024];
		while (responseFile.getline(buf, 1023, '\n'))
			// XXX: what happens if the line is bigger than our buffer ?
			std::cout << buf << std::endl;

	} else
	if (entry->operation.compare("1") == 0) {
		// respond to the request
		// dtk README specifies 1 to add <crlf> so i assume
		// "\r\n" is more accurate that std::endl;
		std::cout << entry->response << "\r\n"; //<< std::endl;
	} else {
		std::cout << entry->response;
	}

	// check whether the fsm should terminate
	if (entry->doContinue == false)
		exit(EXIT_SUCCESS);

	// check whether we have a state transition to do
	if (this->curState != entry->nextState)
		this->changeState(entry->nextState);
	return;
}

void FSM::parse(std::string input)
{
	//std::cout << "parsing from obj @" << this << std::endl;

	// in case we are not the current object pass the call
	if (this != this->states[this->curState]) {
		this->states[this->curState]->parse(input);
		return;
	}

	// FIXME: what is NIL supposed to be acting on ?
	if (input.compare("\n") == 0) {
		this->dtkSpecial("NIL");
		return;
	}

	// bail out in case we've looped too often
	if (++this->curLoop >= this->maxloops)
		exit(EXIT_SUCCESS);

	STTD *entry = NULL;
	STTIterator transition;
	STTIterator end;

	// action Match
	transition = this->actionMatch.begin();
	end = this->actionMatch.end();
	if(this->actionMatch.find(input) != this->actionMatch.end())
		entry = this->actionMatch[input];


	// pattern Match
	transition = this->patternMatch.begin();
	end = this->patternMatch.end();
	for (; transition != end; transition++) {
		std::string dtkRegex = (*transition).first;
		pcre *re;
		const char *error;
		int errOffset;
		int rc;
		int ovector[30];

		//std::cout << "doing pattern match with " << dtkRegex << " on " << input << std::endl;
		re = pcre_compile(dtkRegex.c_str(), 0, &error, &errOffset, NULL);
		if (re == NULL) {
			std::cerr << "compile of regex '" << dtkRegex << "' failed\n";
			continue;
		}

		rc = pcre_exec(re, NULL, input.c_str(), input.length(), 0, 0, ovector, 30);
		if (rc < 0)
			continue; // do better error handling see pcre(3)

		//std::cout << "pattern match on " << input << std::endl;
		entry = (*transition).second;
	}


	// word Match
	transition = this->wordMatch.begin();
	end = this->wordMatch.end();
	for (; transition != end; transition++) {
		std::string word = (*transition).first;
		int pos = input.find(word);
		if (pos < 0)
			continue;

		//std::cout << "word match on " << pos << " " << word << std::endl;
		entry = (*transition).second;
	}


	// dtk special err out if config'd
	if (entry == NULL) {
		this->dtkSpecial("ERROR");
		return;
	}

	this->respond(entry);
	return;
}

void FSM::init(std::string file)
{
	//std::cout << "using scriptfile: " << file << std::endl;
	if (access(file.c_str(), F_OK) < 0) {
		std::cerr << "could not open file " << file << std::endl;
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < 6; i++) {
		this->states[i] = new FSM;
		this->states[i]->getStates(file, i);
	}
	this->changeState(0);
	//this->states[this->curState]->parse("/foobar/");
}

void FSM::getStates(std::string file, unsigned int stateNum)
{
std::string line;
// FIXME: static buffer!
    char buf[1024];
    char *row;
    std::string data[6];
    enum sttdFields { STATE, INPUT, NEXTSTATE, CONTINUE, OPERATION, RESPONSE };
    enum sttdDtkFields { DTKSPECIAL = 3, VARIABLE, VALUE };
	typedef enum matchTableIdent{DTK,PATTERN,ACTION,WORD} TableIdent;
    const std::string regExStr1 = "/";
    const std::string regExStr2 = "M!";

    // open script file
    std::ifstream confFile(file.c_str());
    if (!confFile) {
        std::cerr << "Error: " << __FILE__ << ":" << __LINE__
            << " could not open file " << file << std::endl;
        return;
    }


	STT *whatTable = &this->actionMatch;
    // read the whole line
    while (confFile.getline(buf, 1023, '\n')) {
        // skip lines that are not interesting to us.
        if ((strlen(buf) == 0)			// skip blanks
				|| (buf[0] == '#')		// skip comments
				|| ((buf[0] != '!')		// skip non-fsm-config lines
					// skip lines that are not for this obj, statewise
					&& (unsigned int)atoi(&buf[0]) != stateNum))
            continue;

        // strip fields from line and save them into temp data.
        int i;
        for (i = 5; i > 0; i--) {
			data[i] = "";
            row = strrchr(buf,'\t');
			if (row == NULL)
				break;
            data[i] = row + 1;
            *row = '\0';
        }
        // get status
        data[i] = buf[0];

		// do fsm config variables
		if (buf[0] == '!') {
			if (data[VARIABLE].compare("maxloops") == 0) {
				this->maxloops = (unsigned int) atoi(data[VALUE].c_str());
			} else
			if (data[VARIABLE].compare("timeout") == 0) {
				this->timeout = (unsigned int) atoi(data[VALUE].c_str());
			} else
			if (data[VARIABLE].compare("delay") == 0) {
				this->delay = (unsigned int) atoi(data[VALUE].c_str());
			} else
			if (data[VARIABLE].compare("debug") == 0) {
				this->debug = (atoi(data[VALUE].c_str()) == 1) ? true : false;
			}
			continue;
		}

		unsigned int nextState = atoi(data[NEXTSTATE].c_str());
		char c[6];
		std::sprintf(c, "%s", data[CONTINUE].c_str());
		bool doContinue = (atoi(c) == 1) ? true : false;

		//std::cout << "STATE " << stateNum << " ";
        // switch INPUT for appropiate script line entry
        if ((data[INPUT].compare("START") == 0)
            || (data[INPUT].compare("NIL") == 0)
            || (data[INPUT].compare("ERROR") == 0)
            || (data[INPUT].compare("NOTICE") == 0)) {
            whatTable = &this->dtkMatch;
			this->dtkMatch[data[INPUT]] = new STTD(nextState, doContinue, data[OPERATION], data[RESPONSE]);
            //std::cout << "DTK SPECIAL FOUND\n";
        } else

// non-gcc compiler compat hook
#ifndef __GNUC__
#define __GNUC__ 0
#endif

#if __GNUC__ == 2
        if (data[INPUT].compare("!", 0, 1) == 0) {
#else
        if (data[INPUT].compare(0, 1, "!") == 0) {
#endif
            whatTable = &this->actionMatch;
        } else
#if __GNUC__ == 2
        if ((data[INPUT].compare(regExStr1, 0, regExStr1.length()) == 0)
            || (data[INPUT].compare(regExStr2, 0, regExStr2.length()) == 0)) {
#else
        if ((data[INPUT].compare(0, regExStr1.length(), regExStr1) == 0)
            || (data[INPUT].compare(0, regExStr2.length(), regExStr2) == 0)) {
#endif
			// pcre won't allow for '/' or 'M!' perl compat regex markup!
			// so strip these.
			char *perlPattern = const_cast<char *>(data[INPUT].c_str());
			if (perlPattern[0] == '/')
				perlPattern += 1;
			else
				perlPattern += 2;
			int len = strlen(perlPattern) - 1;
			perlPattern[len] = '\0';
			data[INPUT] = perlPattern;

            whatTable = &this->patternMatch;
        } else {
            whatTable = &this->wordMatch;
        }

		std::string idx = data[INPUT];
		(*whatTable)[idx] = new STTD(nextState, doContinue, data[OPERATION], data[RESPONSE]);
		//whatTable[idx] = new STTD(nextState, doContinue, data[OPERATION], data[RESPONSE]);
		//std::cout << "added " << data[INPUT] << " on state " << this->curState << " which is obj " << whatTable << std::endl;
    } // end while
}
