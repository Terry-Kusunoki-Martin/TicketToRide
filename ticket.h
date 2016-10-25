#ifndef TICKET_H_
#define TICKET_H_
#include <string>
#include <iostream>

#include "library/pair.h"
#include "library/circularArrayList.h"
using std::string;

/*A class for representing a destination ticket in Ticket to Ride.  Each
 * ticket has two cities (order does not matter) and a point value.
 */
class Ticket{

  private:
    string cityA;
    string cityB;
    int points;

  public:
    Ticket();
    Ticket(string a, string b, int pts);
    Pair<string, string> getCities();
    int getPoints();

    void setPoints(int pts);
    void setCities(string a, string b);
    // to print ticket data
    friend std::ostream& operator<<(std::ostream& os, const Ticket& ticket);
};

inline bool operator==(Ticket t1,Ticket t2){
  if(t1.getCities() == t2.getCities() && t1.getPoints() == t2.getPoints()){
    return true;
  }
  return false;
}
#endif
