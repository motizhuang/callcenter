#ifndef MYCALLCENTER_H
#define MYCALLCENTER_H

#include "CallCenter.h"
#include <vector> 
#include "Objects.h"
#include <string> 
#include <queue> 

class MyCallCenter: public CallCenter {
  // Member Variables
  struct Compare {
    bool operator () (const Call* a, const Call* b) const {
    if(a == nullptr || b == nullptr) {
      throw std::invalid_argument("Null pointer in result set.");
    }
    return a->importance > b->importance;
  };
  struct CompareSmall{
    bool operator () (const Call* a, const Call* b) const{
      if(a == nullptr || b == nullptr) {
      throw std::invalid_argument("Null pointer in result set.");
    }
      return a->importance > b->importance;
    }
  };
};
  //std::map<int, Call> actionpool;
  //std::priority_queue<Call,std::vector<Call> ,CompareSmall> actionpool; 
  std::vector<Employee> mEmployees;
  //std::map<int, Call>  mPool;
  int employeecount; 
  std::vector<int> action;
  std::priority_queue<Call*,std::vector<Call*> ,Compare> mPool[26]; 
  int thisminute; 
  std::vector<Call*> haveImportant; 
public:
  // Constructor
  MyCallCenter(std::vector<Employee> employees);
  // Destructor?

  // Required Member Functions
  std::vector<int> calls(int minute, const std::vector<int>& call_ids);
  void learn(int minute, const std::vector<Call>& calls);

  // Other Member Functions
  bool have_important();
  bool can_pick_up();
  bool have_null();
};

#endif
