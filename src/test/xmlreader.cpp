#include "deceptionhandler.h"

int main (int argc, char* argv[]) {

	try {
		XMLPlatformUtils::Initialize();
	} catch (const XMLException& e) {
		std::cout << "Error during initialization: " << XMLString::transcode(e.getMessage()) << std::endl;
		return 1;
	}

	std::string configFile = "deceptiond.xml";
	SAX2XMLReader* parser = XMLReaderFactory::createXMLReader();
	parser->setFeature(XMLUni::fgSAX2CoreValidation, true);

	DeceptionHandler* defaultHandler = new DeceptionHandler();
	parser->setContentHandler(defaultHandler);
	parser->setErrorHandler(defaultHandler);

	try {
		parser->parse(configFile.c_str());
		defaultHandler->dumpList();
	} catch (XMLException& e) {
		std::cout << "Caught XML Exception: " << XMLString::transcode(e.getMessage()) << std::endl;
		return -1;
	} catch (const SAXParseException& e) {
		std::cout << "Caught Parsing Exception: " << XMLString::transcode(e.getMessage()) << std::endl;
		return -1;
	} catch (...) {
		std::cout << "Unknown Exception" << std::endl;
		return -1;
	}

	delete parser;
	delete defaultHandler;
	return 0;
}
