#include "InferenceEngineFC.h"

void InferenceEngineFC::triggerInferenceEngine(WorkMemory *wm, Fact* fact) {
   _unused( fact );
   stack<Fact> memory = wm->getMemory();
   while(!memory.empty()) {
      openKnowledgeBase();
      Fact fact = memory.top();
      memory.pop();
      string content;
      while(getline(*knowledge_base, content)) {
         if(findFactOneSentence(content, fact)){
            vector<string> vector_to_posfix = getPremisesInSentence(content);
            vector_to_posfix = converterToPosFix(vector_to_posfix);
            if(isSentenceTrue(vector_to_posfix, *wm)) addConclusionsInMemory(content, wm);
         }
      }
      if(memory.empty()) {
         if (!controller_schedule.empty()) {
            memory.push(controller_schedule.front());
            controller_schedule.pop();
         }
      }
      knowledge_base->close();
   }
}
