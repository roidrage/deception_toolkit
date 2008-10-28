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
 * \file signals.cpp
 * 
 * \author Mathias Meyer <pom@beatsteaks.de>
 *
 * Contains function implementation for signal handling
 */
#include <signals.h>
#include <logging.h>
#include <fw_pcap.h>

extern Deception::Logging globLog;
extern std::string logName;

// {{{1 DXG DOC
/**
 * Handler for SIGCHLD, just calls waitpidto fetch all exited child
 * processes.
 *
 * \param signalNo Signal number
 */
void chldHandler(int signalNo)
{
	pid_t pid;
	int stat;
	std::string logMsg;
	// wait for any child process
	// if no child has exited, we'll just return to
	// the caller, hence WNOHANG
	while ((pid = waitpid(-1, &stat, WNOHANG)) > 0)
	{
#ifdef DEBUG
		logMsg.append("child ").append(intToString(pid)).append(" exited with return code ").append(intToString(stat));
		globLog.toLog(logName, Deception::Debug, logMsg);
#endif
	}
	return;
}

// {{{1 DXG DOC
/**
 * Handler to catch up with timeouts.
 */
// }}}1 DXG DOC
void alrmHandler(int signalNo)
{
	return;
}

// {{{1 DXG DOC
/**
 * Function to set signal handlers, brought to you by W.R.Stevens
 *
 * \param signalNo	Number of signal
 * \param *function	Pointer to signal handler function
 *
 * \return Pointer to old signal handler function
 */
// }}}1 DXG DOC
sigFunc * setSigHandler(int signalNo, sigFunc *function)
{
	struct sigaction action, oldAction;
	action.sa_handler = function;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;
	if (signalNo == SIGALRM) {
#ifdef SA_INTERRUPT
		action.sa_flags |= SA_INTERRUPT;
#endif
	} else {
#ifdef SA_RESTART
		action.sa_flags |= SA_RESTART;
#endif
	}
	if (sigaction(signalNo, &action, &oldAction) < 0) {
		throw Deception::Exception(errno);
	}
	return(oldAction.sa_handler);
}

