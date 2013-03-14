#ifndef _variable_h_included_
#define _variable_h_included_

#include <iostream>
#include <vector>

using namespace std;

const string DEFAULT_VALUE_FACT = "normal";

class Variable {
   private:
      string name;
      vector<string> values;

   public:
      Variable(string);

      string getName(void);
      void addNewValue(string);
};

#endif

