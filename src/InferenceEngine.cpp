#include "InferenceEngine.h"

InferenceEngine::InferenceEngine(void) {
   knowledge_base = new ifstream(BASE_NAME, ifstream::out);
   if(knowledge_base->fail()) {
      cerr << endl << ERROR_BASE_DOES_NOT_EXIST;
   }
   else
      knowledge_base->close();
}

InferenceEngine::~InferenceEngine(void) {
   if(knowledge_base->is_open())
      knowledge_base->close();
}

bool InferenceEngine::findFactOneSentence(string content, Fact fact) {
   content = content.substr(0, content.find(OPERATOR_THEN));
   string substring = getFactInLine(fact);
   int find = content.find(substring);
   if(find == ERROR_SUBSTRING_NOT_FOUND)
      return false;
   return true;
}

void InferenceEngine::openKnowledgeBase(void) {
   if(!knowledge_base->is_open())
      knowledge_base = new ifstream(BASE_NAME, ifstream::out);
}

int InferenceEngine::countOccurrenceSubstring(string phrase, string substring) {
   int found = -1;
   int count = 0;
   do {
      found = phrase.find(substring, found+1);
      if (found != ERROR_SUBSTRING_NOT_FOUND) { count++; }
   } while(found != ERROR_SUBSTRING_NOT_FOUND);
   return count;
}

bool InferenceEngine::operatorIFThenChecker(string content, unsigned short int line) {
  if (content.find(OPERATOR_IF) == content.rfind(OPERATOR_IF) &&
         content.find(OPERATOR_THEN) == content.rfind(OPERATOR_THEN) &&
         content.find(OPERATOR_IF) < content.find(OPERATOR_THEN) &&
         content.find(OPERATOR_IF) < LIMIT_BUFFER &&
         content.find(OPERATOR_THEN) < LIMIT_BUFFER) {
      return true;
   }
   cerr << endl << "There is some inconsistency(Line: " << line << "). Please check if any line contains if and only if a single " << OPERATOR_IF << " and one " << OPERATOR_THEN << " and the line is less than or equal to the size of LIMIT_BUFFER.";
   return false;
}

bool InferenceEngine::operatorSentenceChecker(string content, unsigned short int line) {
   int found_begin = countOccurrenceSubstring(content, OPERATOR_BEGIN_SENTENCE);
   int found_end = countOccurrenceSubstring(content, OPERATOR_END_SENTENCE);
   if(found_begin == found_end)
      return true;
   cerr << endl << "Inconsistent. Check that for each beginning sentence there is a end sentence (Line: " << line << ").  Also make sure there is at least two expressions, a premise and a conclusion.";
   return false;
}

bool InferenceEngine::structureChecker(string content, unsigned short int line) {
   content = content.substr(content.find(OPERATOR_IF)+OPERATOR_IF.size(), content.find(OPERATOR_THEN)-OPERATOR_IF.size());
   int count_occurrence_operator_and = countOccurrenceSubstring(content, OPERATOR_AND);
   int count_occurrence_operator_or = countOccurrenceSubstring(content, OPERATOR_OR);
   int count_occurrence_variable = countOccurrenceSubstring(content, OPERATOR_BEGIN_SENTENCE);
   if ((count_occurrence_operator_and + count_occurrence_operator_or + 1) == (count_occurrence_variable))
      return true;
   cerr << endl << "Inconsistent.(Line: " << line << ") Make sure there are enough words to existing operators.";
   return false;
}

void InferenceEngine::addNewValueQueue(Fact fact) {
   this->controller_schedule.push(fact);
}

bool InferenceEngine::consistencyChecker(void) {
   openKnowledgeBase();
   if(knowledge_base->is_open()) {
      string content;
      unsigned short int line = 0;
      while(getline(*knowledge_base, content)) {
         line++;
         if(operatorIFThenChecker(content, line) && operatorSentenceChecker(content, line) && structureChecker(content, line)) continue;
         else return false;
      }
      knowledge_base->close();
   }
   return true;
}

string InferenceEngine::getFactInLine(Fact fact) {
   string substring = OPERATOR_BEGIN_SENTENCE + fact.getName() + OPERATOR_BEGIN_VALUE_IN_SENTENCE + fact.getValue() + OPERATOR_END_VALUE_IN_SENTENCE + OPERATOR_END_SENTENCE;
   return substring;
}

string InferenceEngine::checkOperator(string content, int *found) {
   vector<int> op;
   op.push_back(content.find(OPERATOR_OR, *found+1));
   op.push_back(content.find(OPERATOR_AND, *found+1));
   op.push_back(content.find(OPERATOR_BEGIN_SENTENCE, *found+1));
   op.push_back(content.find("(", *found+1));
   op.push_back(content.find(")", *found+1));
   int menor = LIMIT_BUFFER;
   int position = -1;
   for(int iterator = 0; iterator < op.size(); iterator++) {
      if(op.at(iterator) != ERROR_SUBSTRING_NOT_FOUND && op.at(iterator) < menor) {
         menor = op.at(iterator);
         position = iterator;
      }
   }
   if(menor!=LIMIT_BUFFER) *found = menor;
   else *found = ERROR_SUBSTRING_NOT_FOUND;
   switch(position) {
      case 0:
         return OPERATOR_OR;
      case 1:
         return OPERATOR_AND;
      case 2:
         return OPERATOR_BEGIN_SENTENCE;
      case 3:
         return "(";
      case 4:
         return ")";
   }
   return " ";
}

void InferenceEngine::addVectorOP(vector<string>* division, string op, string content, int found) {
   if(op == OPERATOR_AND) division->push_back(OPERATOR_AND);
   else if(op == OPERATOR_OR) division->push_back(OPERATOR_OR);
   else if(op == "(") division->push_back("(");
   else if(op == ")") division->push_back(")");
   else if(op == OPERATOR_BEGIN_SENTENCE) {
      division->push_back(content.substr(found, content.find(OPERATOR_END_SENTENCE, found)+OPERATOR_END_SENTENCE.size()-found));
   }
}

vector<string> InferenceEngine::getPremisesInSentence(string content) {
   vector<string> division;
   int found = -1;
   content = content.substr(content.find(OPERATOR_IF)+OPERATOR_IF.size(), content.find(OPERATOR_THEN)-OPERATOR_IF.size());
   do {
      string op = checkOperator(content, &found);
      if(found != ERROR_SUBSTRING_NOT_FOUND) addVectorOP(&division, op, content, found);
   } while(found != ERROR_SUBSTRING_NOT_FOUND);
   return division;
}

bool InferenceEngine::isOperator(string ch) {
   if ((ch == OPERATOR_OR) || (ch == OPERATOR_AND)) {
      return true;
   } else
      return false;
}

bool InferenceEngine::precedence(string top, string ex) {
   if (top == ex)
      return true;
   else if ((top == OPERATOR_AND) && (ex == OPERATOR_OR)) {
      return true;
   } else {
      return false;
   }
}

vector<string> InferenceEngine::converterToPosFix(vector<string> infix) {
   stack<string> pilha;
   vector<string> posfix;
   for(int i=0; i<infix.size(); i++) {
      if(!isOperator(infix.at(i)) && infix.at(i) != "(" && infix.at(i) != ")") {
         posfix.push_back(infix.at(i));
      }
      if (isOperator(infix.at(i))) {
         while(!pilha.empty() && precedence(pilha.top(), infix.at(i))) {
            if(precedence(pilha.top(), infix.at(i))) {
               posfix.push_back(pilha.top());
               pilha.pop();
            }
         }
         pilha.push(infix.at(i));
      }
      if(infix.at(i) == "(") {
         pilha.push("(");
      }
      if(infix.at(i) == ")") {
         while(pilha.top() != "(") {
            posfix.push_back(pilha.top());
            pilha.pop();
         }
         pilha.pop();
      }
   }
   while(!pilha.empty()) {
         posfix.push_back(pilha.top());
         pilha.pop();
   }
   return posfix;
}

Fact InferenceEngine::stringToFact(string to_fact) {
   int foundb = to_fact.find(OPERATOR_BEGIN_VALUE_IN_SENTENCE) + 1;
   int founde = to_fact.find(OPERATOR_END_VALUE_IN_SENTENCE);
   string value = to_fact.substr(foundb, founde-foundb);
   to_fact = to_fact.erase(foundb-1, founde-1);
   string name_fact = to_fact.substr(1,to_fact.find(OPERATOR_END_SENTENCE)-1);
   Fact fact(name_fact, value);
   return fact;
}

bool InferenceEngine::isSentenceTrue(vector<string> in_posfix, WorkMemory wm) {
   stack<Fact> facts;
   bool okey = true;
   for(int i=0; i<in_posfix.size(); i++) {
      if(in_posfix.at(i) != "AND" && in_posfix.at(i) != "OR") {
         facts.push(stringToFact(in_posfix.at(i)));
      }
      else if(in_posfix.at(i) == "AND") {
         bool op1 = wm.isExistFactInMemory(facts.top());
         facts.pop();
         bool op2;
         if(!facts.empty()) {
            op2 = wm.isExistFactInMemory(facts.top());
            facts.pop();
         }
         else { op2 = okey; }
         if(op1 && op2)
               okey = true;
         else
            okey = false;
      }
      else if(in_posfix.at(i) == "OR") {
         bool op1 = wm.isExistFactInMemory(facts.top());
         facts.pop();
         bool op2;
         if(!facts.empty()) {
            op2 = wm.isExistFactInMemory(facts.top());
            facts.pop();
         }
         else { op2 = okey; }
         if(op1 || op2)
            okey = true;
         else
            okey = false;
      }
   }
   return okey;
}

void InferenceEngine::addConclusionsInMemory(string content, WorkMemory *wm) {
   content = content.substr(content.find(OPERATOR_THEN)+OPERATOR_THEN.size());
   int found = -1;
   do {
      found = content.find(SEPARATOR_CONCLUSIONS, found+1);
      Fact fact = stringToFact(content.substr(0, found));
      if (!wm->isExistFactInMemory(fact)) {
         controller_schedule.push(fact);
         wm->addNewFact(fact);
      }
      content = content.erase(0, found+SEPARATOR_CONCLUSIONS.size());
   } while(found != ERROR_SUBSTRING_NOT_FOUND);
}
