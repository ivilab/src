/*!
 * @file Tokenizer.cc
 *
 * @author Mihai Surdeanu
 * $Id$ 
 */

#include "spear/Tokenizer.h"

using namespace std;
using namespace spear;

bool Tokenizer::initialized = false;

spear::StringMap<bool> Tokenizer::multiWords;

bool Tokenizer::initialize(const std::string & dataPath)
{
  if(initialized == true){
    return true;
  }

  string name = dataPath + "/multiwords.txt";
  IFStream is(name.c_str());
  if(! is){
    cerr << "Can not open tokenizer multiword file: " << name << endl;
    return false;
  }

  // load multiwords from this stream
  string mw;
  int count = 0;
  while(is >> mw){
    multiWords.set(mw.c_str(), true);
    count ++;
  }

  // cerr << "Loaded " << count << " multiwords." << endl;

  initialized = true;
  return true;
}
 
Tokenizer::Tokenizer(const std::string & dataPath)
{
  if(initialize(dataPath) == false){
    throw bool(false);
  }
}

String Tokenizer::normalize(const String & s) 
{
  if(s == W("(")){
    return W("-LRB-");
  }

  if(s == W(")")){
    return W("-RRB-");
  }

  if(s == W("[") || s == W("{")){
    return W("-LCB-");
  }

  if(s == W("]") || s == W("}")){
    return W("-RCB-");
  }

  if(s == W("\"")){
    return W("''");
  }

  return s;
}

// generated by lex
extern void unitSplit(const Char * buffer, 
		      unsigned int size,
		      std::vector<String> & tokens);

// The maximum size of a multiword in atomic tokens
#define MAX_MULTI_WORD_SIZE 20

void Tokenizer::split(const String & buffer, std::vector<spear::Word> & tokens)
{
  vector<String> splits;

  // 
  // As a first stage, we tokenize without detecting multiwords
  // 
#ifdef USE_UNICODE
#error "Unicode tokenization not implemented yet!"
#else
  unitSplit(buffer.c_str(), buffer.size(), splits);
#endif

  if(splits.size() == 0){
    return;
  }

  // the code below is no longer used because now splitter.l separates only the last dot
  /*
  vector<String> mw;
  
  //
  // Detect multiwords
  //
  for(unsigned int i = 0; i < splits.size(); i ++){

    // where the mw ends
    unsigned int end = splits.size();
    if(end > i + MAX_MULTI_WORD_SIZE) end = i + MAX_MULTI_WORD_SIZE;

    bool found = false;

    // must have at least two tokens per multiword
    for(; end > i + 1; end --){
      String conc = concatenate(splits, i, end);
      found = false;

      // found a multiword
      if(multiWords.get(conc.c_str(), found) == true){
	mw.push_back(conc);
	i = end - 1;
	break;
      }
    }

    // no multiword starting at this position found
    if(! found){
      mw.push_back(splits[i]);
    }
  }

  for(unsigned int i = 0; i < mw.size(); i ++){
    tokens.push_back(Word(normalize(mw[i]), W("")));
  }
  */

  //
  // check for abbreviations
  //

  vector<String> merged;

  for(unsigned int i = 0; i < splits.size(); i ++){
    bool found = false;

    // found a known abbreviation
    if(i < splits.size() - 1 && splits[i + 1] == ".") {
      String m = splits[i] + splits[i + 1];
      if(multiWords.get(m.c_str(), found) == true){
	merged.push_back(m);
	i ++;
      }
    } 

    if(found == false){
      merged.push_back(splits[i]);
    }
  }
  
  // save the tokens
  for(unsigned int i = 0; i < merged.size(); i ++){
    tokens.push_back(Word(normalize(merged[i]), W("")));
  }
}

String Tokenizer::concatenate(const std::vector<String> & tokens,
			      unsigned int start,
			      unsigned int end) const
{
  OStringStream os;

  for(; start < end; start ++){
    os << tokens[start];
  }

  return os.str();
}

