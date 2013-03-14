#include "Variable.h"

Variable::Variable(string name) {
   this->name = name;
   values.push_back(DEFAULT_VALUE_FACT);
}

string Variable::getName(void) {
   return name;
}

void Variable::addNewValue(string value) {
   values.push_back(value);
}
