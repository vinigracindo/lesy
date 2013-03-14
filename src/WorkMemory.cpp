#include "WorkMemory.h"

void WorkMemory::addNewFact(Fact fact) {
   memory.push(fact);
}

stack<Fact> WorkMemory::getMemory(void) {
   return memory;
}

bool WorkMemory::isExistFactInMemory(Fact fact) {
   stack<Fact> aux = memory;
   while(!aux.empty()) {
      if(aux.top().getName() == fact.getName() && memory.top().getValue() == fact.getValue())
         return true;
      aux.pop();
   }
   return false;
}
