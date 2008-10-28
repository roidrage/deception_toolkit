#include <string>
class STTD
{
	public:
		unsigned int nextState;
		bool doContinue;
		std::string operation;
		std::string response;
		~STTD(void) {};
		STTD(unsigned int ns, bool dc, std::string op, std::string re)
		: nextState(ns), doContinue(dc), operation(op), response(re)
		{};

};
