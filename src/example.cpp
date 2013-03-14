#include <iostream>
#include "InferenceEngine.h"
#include "Fact.h"
#include "WorkMemory.h"

/*
 * Knowledge Base: knowledge_base.txt
 * */

int main() {
   InferenceEngine ie;
   WorkMemory wm;
   Fact f2("A");
   wm.addNewFact(f2);
   bool c = ie.consistencyChecker();
   if(c) cout << endl << "[consistent]";
   ie.triggerInferenceEngine(&wm);
   stack<Fact> fa = wm.getMemory();
   int tam_total = fa.size();
   while(!fa.empty()) {
      cout << endl << fa.top().getName();
      fa.pop();
   }
   cout << endl << tam_total << endl;
   system("pause");
   return 0;
}
