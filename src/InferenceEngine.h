#ifndef _inferenceengine_h_included_
#define _inferenceengine_h_included_

#include <iostream>
#include <fstream>
#include <queue>
#include <iterator>
#include <stack>
#include "Fact.h"
#include "WorkMemory.h"

#define ERROR_BASE_DOES_NOT_EXIST "Base does not exist."
#define ERROR_SUBSTRING_NOT_FOUND -1
#define _unused(x) ((void)x)

using namespace std;

const int LIMIT_BUFFER = 1024;
const string OPERATOR_IF = "IF";
const string OPERATOR_THEN = "THEN";
const string OPERATOR_AND = "AND";
const string OPERATOR_OR = "OR";
const string OPERATOR_BEGIN_SENTENCE = "<";
const string OPERATOR_END_SENTENCE = ">";
const string OPERATOR_BEGIN_VALUE_IN_SENTENCE = "[";
const string OPERATOR_END_VALUE_IN_SENTENCE = "]";
const char BASE_NAME[] = "knowledge_base.txt";
const string DOES_NOT_EXIST_OPERATOR = "null";
const string SEPARATOR_CONCLUSIONS = ",";

// IF <[A(NAO)]> THEN <B>

class InferenceEngine {
   protected:
      ifstream *knowledge_base;
      queue<Fact> controller_schedule;

      void openKnowledgeBase(void);
      bool operatorIFThenChecker(string, unsigned short int);
      bool operatorSentenceChecker(string, unsigned short int);
      bool structureChecker(string, unsigned short int);
      int countOccurrenceSubstring(string, string);
      bool findFactOneSentence(string, Fact);
      string getFactInLine(Fact);
      vector<string> getPremisesInSentence(string);
      string checkOperator(string, int*);
      void addVectorOP(vector<string>*, string, string, int);

      bool isOperator(string ch);
      bool precedence(string top, string ex);
      vector<string> converterToPosFix(vector<string> infix);
      bool isSentenceTrue(vector<string>, WorkMemory);
      Fact stringToFact(string);
      void addConclusionsInMemory(string, WorkMemory*);
   public:
      InferenceEngine(void);
      ~InferenceEngine(void);

      void addNewValueQueue(Fact);
      bool consistencyChecker(void);

      virtual void triggerInferenceEngine(WorkMemory*, Fact*) = 0;

};

#endif

