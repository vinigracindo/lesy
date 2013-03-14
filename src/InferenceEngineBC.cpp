#include "InferenceEngineBC.h"

bool InferenceEngineBC::findConclusionOneSentence(string content, Fact fact) {
   content = content.substr(content.find(OPERATOR_THEN)+OPERATOR_THEN.size(), content.size()-(content.find(OPERATOR_THEN)+OPERATOR_THEN.size()));
   string substring = getFactInLine(fact);
   int find = content.find(substring);
   if(find == ERROR_SUBSTRING_NOT_FOUND)
      return false;
   return true;
}

bool InferenceEngineBC::isFactTrue(WorkMemory* wm, Fact fact, Fact goal) {
   if(wm->isExistFactInMemory(fact)) {
      return true;
   }
   else {
      if(knowledge_base->is_open()) knowledge_base->close();
      openKnowledgeBase();
      string content;
      bool find = false;
      while(getline(*knowledge_base, content)) {
         if(findConclusionOneSentence(content, fact)){
            find = true;
            vector<string> vector_to_posfix = getPremisesInSentence(content);
            vector_to_posfix = converterToPosFix(vector_to_posfix);
            for(int i=0; i<vector_to_posfix.size(); i++) {
               if(vector_to_posfix.at(i) != OPERATOR_AND && vector_to_posfix.at(i) != OPERATOR_OR)
                  return isFactTrue(wm,stringToFact(vector_to_posfix.at(i)), goal);
            }
         }
      }
      if(!find) {
         char resp;
         cout << endl << fact.getName() << " is True? (y/n) ";
         cin >> resp;
         if(resp == 'y') wm->addNewFact(fact);
         else return false;
      }
      knowledge_base->close();
   }
}

void InferenceEngineBC::triggerInferenceEngine(WorkMemory *wm, Fact* goal) {
   stack<Fact> memory = wm->getMemory();
   openKnowledgeBase();
   string content;
   while(getline(*knowledge_base, content)) {
      if(findConclusionOneSentence(content, *goal)){
         knowledge_base->close();
         bool is_true = isFactTrue(wm, *goal, *goal);
         if(is_true) {
            cout << endl << goal->getName() << " is True" << endl;
            return;
         }
         else {
            cerr << endl << goal->getName() << " is False" << endl;
            return;
         }
      }
   }
   cout << endl << "Objective lacking in the knowledge base." << endl;
}

