#ifndef _workmemory_h_included_
#define _workmemory_h_included_

#include <iostream>
#include <stack>
#include "Fact.h"

using namespace std;

class WorkMemory {
   private:
      stack<Fact> memory;

   public:
      void addNewFact(Fact fact);
      stack<Fact> getMemory(void);

      bool isExistFactInMemory(Fact);
};

#endif

