/*
    PowerMail versatile mail receiver
    Copyright (C) 2002  PowerDNS.COM BV

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
#ifndef DELIVERY_HH
#define DELIVERY_HH
#include <string>
#include <vector>
using namespace std;

class DeliveryException
{
public:
  DeliveryException(const string &reason) : d_reason(reason) {}
  const string &getReason() const
  {
    return d_reason;
  }
private:
  const string d_reason;

};


class Delivery 
{
public:
  Delivery();
  ~Delivery();
  int addFile(const string &address, const string &index, string *response);
  int makeRealMessage(const string &address, const string &index, string *filename, string *response);
  int makeLinkMessage(const string &address, const string &index, const string &first, string *filename, string *response);
  void listMbox(const string &mbox, string &response);
  int giveLine(const string &line);
  void close(string &response);
  int numRecipients();
  int getMessageFD(const string &mbox, const string &index, string &response);
  int delMessage(const string &mbox, const string &index, string &response);
  void commit();
  void rollBack(string &response);
  void reset();
  string calcMaildir(const string &mbox);
  void startList();
  bool getListNext(string &response);
  void nuke(const string &mbox, string &response);
private:
  void nukeDir(const string &dir);
  void unlinkAll();
  int moveAll();
  int setupMaildir(const char *dir, string *response);
  string dohash(const string &mbox, int *x=0, int *y=0);
  int makeHashDirs(const string &mbox, string *response);

  int makeMessageLoc(const string &address, const string& index, string *filename, string *response);
  vector<string>d_destinations;
  vector<string>d_filenames;
  string d_filename;
  int d_fd;
  string d_mailroot;
  vector<int> d_listXvect;
  vector<int> d_listYvect;
  vector<int>::const_iterator d_listX;
  vector<int>::const_iterator d_listY;
};

#endif /* DELIVERY_HH */
