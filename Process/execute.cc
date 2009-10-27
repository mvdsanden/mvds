#include "process.ih"

#include <cstdlib>
#include <fcntl.h>
#include <iostream>

#include "../FDStream/fdstream.hh"
#include "../IOError/ioerror.hh"

int Process::execute(std::string const &command)
{

  d_inPipe[0] = 0;
  d_inPipe[1] = 0;

  // Create the pipes
  if (pipe(d_inPipe) < 0)
	throw IOError(errno,"Process: error creating in pipe");

  if (pipe(d_outPipe) < 0)
	throw IOError(errno,"Process: error creating out pipe");

  if (pipe(d_errPipe) < 0)
	throw IOError(errno,"Process: error creating err pipe");

  // Fork!
  d_pid = fork();

  if (d_pid < 0)
	throw runtime_error("Process: error forking");
  else if (!d_pid) { // We are the child process!
	try {
	  close(d_inPipe[1]);
	  close(d_outPipe[0]);
	  close(d_errPipe[0]);

	  // Redirect stdin to the in pipe.
	  if (dup2(d_inPipe[0],0) < 0)
		throw IOError(errno,"Process: redirecting stdin to in pipe");
	  close(d_inPipe[0]);

	  // Redirect the stdout to the out pipe.
	  if (dup2(d_outPipe[1],1) < 0)
		throw IOError(errno,"Process: redirecting stdout to out pipe");
	  close(d_outPipe[1]);

	  // Redirect the stderr to the err pipe.
	  if (dup2(d_errPipe[1],2) < 0)
		throw IOError(errno,"Process: redirecting stderr to err pipe");
	  close(d_errPipe[1]);

	  // Run the command.
	  if (execl("/bin/sh","/bin/sh","-c",command.c_str(),static_cast<char*>(0)) < 0)
		throw IOError(errno,"Process: executing process");

	} catch (runtime_error &e) {
	  cerr << "Process error in fork: " << e.what() << endl;

	  // If it returns an error has occured...
	  exit(1);
	}
  } else {
	// Close the end of the pipes we do not use.
	close(d_inPipe[0]);
	close(d_outPipe[1]);
	close(d_errPipe[1]);

	// Create stl streams from them.
	d_in  = new OFDStream(d_inPipe[1]);
	d_out = new IFDStream(d_outPipe[0]);
	d_err = new IFDStream(d_errPipe[0]);
  }

  return d_pid;
}
