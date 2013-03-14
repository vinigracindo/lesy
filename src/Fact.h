#ifndef _fact_h_included_
#define _fact_h_included_

#include <iostream>
#include "Variable.h"

using namespace std;

class Fact {
   private:
      string name;
      string value;

   public:
      Fact(string, string);
      Fact(string);

      string getName(void);
      string getValue(void);
};

#endif



