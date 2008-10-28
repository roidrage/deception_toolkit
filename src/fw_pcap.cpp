// Copyright (c) 2003, Alexis Hildebrandt, Mathias Meyer
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// 
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
// * Neither the name of the authors nor the names of its contributors may
//   be used to endorse or promote products derived from this software without
//   specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

/**
 * \file fw_pcap.cpp
 *
 * Contains a simple packet parsing engine based on libpcap to look for incoming connection request.
 * Compile with $CC -lpcap
 * 
 * \note libpcap before v0.7.2 has a bug in /usr/include/net/bpf.h which makes it unusable for use with C++.
 * To fix that, use patch bpf.h.patch in current directory.
 *
 * \author Mathias Meyer <pom@beatsteaks.de>
 */

// system includes
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <netinet/if_ether.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <stdlib.h>
#include <pcap.h>
#include <string.h>

// c++ stuff
#include <string>

// framework includes
#include "logging.h"
#include "fw_pcap.h"
#include "exception.h"
#include "captureexception.h"

std::string name = "pcap_engine";
std::string logMsg = "";
extern Deception::Logging globLog;

// {{{1 DXG DOC
/**
 * Convert an unsigned int value to a string
 *
 * \param toConvert Integer to be converted
 *
 * \retval "const char*" String containing converted integer
 **/
// }}}1 DXG DOC
const char* intToString(unsigned int toConvert)
{
	// enough space for unsigned int and \0
	char *buf = static_cast<char*>(calloc(sizeof(char), 11));
	
	if (snprintf(buf, 11, "%d", toConvert) < 0) {
		throw Deception::Exception("error while converting int to string");
	}
	return strdup(buf);
}
	
// {{{1 DXG DOC
/**
 * Analyze a single packet, if it's of any use for us (i.e. we only look for connections being opened
 * from somewhere else, so only packets with syn = 1 and ack = 0 are interesting here. It's being called
 * from pcap_loop() or pcap_dispatch(), because of that the parameters are fixed
 *
 * \param useless ???
 * \param pkthdr The header of the fetched packet
 * \param packet Pointer to the actual package
 */
// }}}1 DXG DOC
void analyzePacket(u_char* useless, const struct pcap_pkthdr* pkthdr, const u_char* packet)
{
	const struct ip *sniffip;
	const struct tcphdr *snifftcp;
	const struct ether_header *sniffether;
	// grab ethernet header
	sniffether = reinterpret_cast<const struct ether_header*>(packet);
	// if we got a bad pointer, get the hell out of here
	if (!sniffether) {
		return;
	}
	// check if it's an ip packet, since we're only interested in that
	if (ntohs(sniffether->ether_type) == ETHERTYPE_IP) {
		// now that we're at it, only tcp is interesting
		// fetch ip structure
		sniffip = reinterpret_cast<const struct ip*>(packet + sizeof(struct ether_header));
		if (sniffip->ip_p == IPPROTO_TCP) {
			// fetch tcp structure
			snifftcp = reinterpret_cast<const struct tcphdr*>(packet + sizeof(struct ether_header)
					+ sizeof(struct ip));
			// we are only interested in connection request, i.e. syn-flag must be set and ack flag is not set
#if defined(__sun__) || defined(__sun) || defined(__FreeBSD__) || defined(Darwin)
			if ((snifftcp->th_flags & TH_SYN) & ~(snifftcp->th_flags & TH_ACK)) {
#else
			if ((snifftcp->syn == 1) && (snifftcp->ack == 0)) {
#endif
				const char *inIpBuf = inet_ntoa(sniffip->ip_src);
				const char *outIpBuf = inet_ntoa(sniffip->ip_dst);
				// fetch source and destination ip
				// print out connection request
				logMsg.append("connection request from ").append(inIpBuf).append(":")
#if defined(__sun__) || defined(__sun) || defined(__FreeBSD__) || defined(Darwin)
					.append(intToString(ntohs(snifftcp->th_sport)))
#else
					.append(intToString(ntohs(snifftcp->source)))
#endif
					.append(" to ").append(outIpBuf).append(":")
#if defined(__sun__) || defined(__sun) || defined(__FreeBSD__) || defined(Darwin)
					.append(intToString(ntohs(snifftcp->th_dport)));
#else
					.append(intToString(ntohs(snifftcp->dest)));
#endif
				globLog.toLog(name, Deception::Info, logMsg);
				logMsg.erase();
			}
		}
	}
}

// {{{1 DXG DOC
/**
 * Initialize capturing engine
 *
 * \param _modReg ModuleRegistry containing all used IPs and ports
 * \param _dev Device to sniff on
 **/
// }}}1 DXG DOC
void capture(Deception::ModuleRegistry &_modReg, std::string &_dev)
{
	pcap_t *handle;
	char errbuf[PCAP_ERRBUF_SIZE];
	struct bpf_program filter;
	std::string filterRule, ipAddr;
	unsigned int port;
	Deception::ModuleRegistry::ModuleRegistryMapIterator mPtr = _modReg.begin();
	Deception::ModuleRegistry::ModuleRegistryMapIterator mEnd = _modReg.end();
	for (; mPtr != mEnd; mPtr++) {
		// FIXME: do something useful in case of 0.0.0.0
		ipAddr = _modReg.getIpAddr((*mPtr).first);
		port = _modReg.getPort((*mPtr).first);
		filterRule.append("(dst host " + _modReg.getIpAddr((*mPtr).first) + " and not dst port " + intToString(port) + ")");
		if ((++mPtr) != mEnd) {
			filterRule.append(" or ");
		}
		--mPtr;
	}
	// keyword tcp has to be escaped for pcap filter
	filterRule.append(" and ip proto \\tcp");
	bpf_u_int32 mask;
	bpf_u_int32 net;
	// get a nice and useful device, but only if none was configured
	if (_dev.length() == 0) {
		if ((_dev = pcap_lookupdev(errbuf)).length() == 0) {
			throw Deception::CaptureException(errbuf);
		}
	}
	// open handle
	if ((handle = pcap_open_live(const_cast<char*>(_dev.c_str()), BUFSIZ, 1, 0, errbuf)) == NULL) {
		throw Deception::CaptureException(errbuf);
	}
	// compile filter (for now from command line)
	if (pcap_compile(handle, &filter, const_cast<char*>(filterRule.c_str()), 0, net) == -1) {
		throw Deception::CaptureException(errbuf);
	}
	// apply filter
	if (pcap_setfilter(handle, &filter) == -1) {
		throw Deception::CaptureException(pcap_geterr(handle));
	}
	// now let's loop around and snoop around
	if (pcap_loop(handle, -1, analyzePacket, NULL) == -1) {
		throw Deception::CaptureException(pcap_geterr(handle));
	}
	pcap_close(handle);
	return;
}
