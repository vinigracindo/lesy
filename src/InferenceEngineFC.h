#ifndef _inferenceenginefc_h_included_
#define _inferenceenginefc_h_included_

#include "InferenceEngine.h"

using namespace std;

class InferenceEngineFC : public InferenceEngine  {
   private:

   public:
      void triggerInferenceEngine(WorkMemory*, Fact* = NULL);
};

#endif

