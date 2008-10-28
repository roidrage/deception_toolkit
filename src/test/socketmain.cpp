#include "exception.h"
#include "inputstream.h"
#include "outputstream.h"
#include "socket.h"

using namespace Deception;

int main() {
	try {
		Socket p(2000);
		p.init();
		while(1) {
			try {
				p.accept();
				std::cout << "connected with: " << p.getClientAddress() << std::endl;
				char c;
				InputStream &in = p.getInputStream();
				OutputStream &out = p.getOutputStream();
				out << "hallo\n" << std::endl;
				while (in.get(c)) {
					std::cout << c << std::endl;
				}
			} catch (SocketException &s) {
				continue;
			}
			p.shutDown(SHUT_RDWR);
		}
	} catch (Exception &e) {
		std::cout << e.getType() << ": " << e.toString() << std::endl;
	}

	return 0;
}
