#ifndef SESSION_HH
#define SESSION_HH

#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <cerrno>
#include <arpa/inet.h>  

#include <sys/stat.h>
#include <unistd.h>
#include <netdb.h>
#include <iostream>
#include "ahuexception.hh"

class SessionException: public AhuException
{
public:
  SessionException(const string &reason) : AhuException(reason){}
};

class SessionTimeoutException: public SessionException
{
public:
  SessionTimeoutException(const string &reason) : SessionException(reason){}
};

//! The Session class represents a TCP/IP session, which can either be created or run on an existing socket
class Session
{
  class SessionBuffer
  {
  public:
    explicit SessionBuffer(unsigned int size=16000) : d_size(size), d_buffer(new char[d_size])
    {
    }
    SessionBuffer(const SessionBuffer &orig) : d_size(orig.d_size), d_buffer(new char[orig.d_size])
    {
      memcpy(d_buffer,orig.d_buffer,d_size);
    }
    ~SessionBuffer(){ 
      delete[] d_buffer; 
    }
    SessionBuffer& operator=(const SessionBuffer &rhs)
    {
      if(&rhs!=this) {
	delete[] d_buffer;
	d_size=rhs.d_size;

	d_buffer=new char[d_size];
	memcpy(d_buffer,rhs.d_buffer,d_size);
      }
      return *this;
    }
    operator char*() 
    {
      return d_buffer;
    }
  private:
    unsigned int d_size;
    char *d_buffer;
  };
public:
  void getLine(string &); //!< Read a line from the remote. Throws an exception in case of error or end of file
  bool haveLine(); //!< returns true if a line is available
  bool putLine(const string &s); //!< Write a line to the remote
  bool sendFile(int fd); //!< Send a file out
  int timeoutRead(int s,char *buf, size_t len);

  Session(int s, struct sockaddr_in r); //!< Start a session on an existing socket, and inform this class of the remotes name

  /** Create a session to a remote host and port. This function reads a timeout value from the ArgvMap class 
      and does a nonblocking connect to support this timeout. It should be noted that nonblocking connects 
      suffer from bad portability problems, so look here if you see weird problems on new platforms */
  Session(const string &remote, int port, int timeout=0); 
  Session(u_int32_t ip, int port, int timeout=0);
  Session(const Session &s); 
  
  ~Session();
  int getSocket(); //!< return the filedescriptor for layering violations
  string getRemote();
  u_int32_t getRemoteAddr();
  string getRemoteIP();
  void beVerbose();
  int close(); //!< close and disconnect the connection
  void setTimeout(unsigned int seconds);
private:
  void doConnect(u_int32_t ip, int port);
  bool d_verbose;
  SessionBuffer rdbuf;
  int d_bufsize;
  int rdoffset;
  int wroffset;
  int clisock;
  struct sockaddr_in remote;
  void init();
  int d_timeout;

};

//! The server class can be used to create listening servers
class Server
{
public:
  Server(int p, const string &localaddress=""); //!< port on which to listen
  Session* accept(); //!< Call accept() in an endless loop to accept new connections
private:
  int s;
  int port;
  int backlog;

  string d_localaddress;
};

class Exception
{
public:
  Exception(){reason="Unspecified";};
  Exception(string r){reason=r;};
  string reason;
};

#endif /* SESSION_HH */