#ifndef _inferenceenginebc_h_included_
#define _inferenceenginebc_h_included_

#include "InferenceEngine.h"

using namespace std;

class InferenceEngineBC : public InferenceEngine  {
   private:

   public:
      void triggerInferenceEngine(WorkMemory*, Fact*);
      bool findConclusionOneSentence(string content, Fact fact);
      bool isFactTrue(WorkMemory*, Fact, Fact);
};

#endif


