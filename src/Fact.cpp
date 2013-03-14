#include "Fact.h"

Fact::Fact(string name, string  value) {
   this->name = name;
   this->value = value;
}

Fact::Fact(string name) {
   this->name = name;
   this->value = DEFAULT_VALUE_FACT;
}

string Fact::getName(void) {
   return name;
}

string Fact::getValue(void) {
   return value;
}
