#include <stdio.h>
#include <fstream>
#include <map>
#include <list>
#include <set>
#include <iomanip>
#include <iostream>
#include <cfloat>
#include <sstream>
#include <sys/time.h>
#include <cstring>
#include "post/Predictor.hh"

using namespace std;
using namespace nlptools::postagger;

struct sresult {
  char const *start;
  int length;
  char const *tag;
  char errc;
  unsigned char metadata; // bit 0: stopword, bit 1: sentence_delimiter
  char const *pos;
  int allocd;
};

extern "C" void libpredict_tag_sequence(nlptools::postagger::Predictor* predictor, sresult* sequence, int length) {
	if (!predictor) return;
	if (!sequence) return;
	Sentence sent;
	for (int i = 0; i < length; i++) {
		std::string tok = std::string{sequence[i].start, sequence[i].length};
		sent.push_back(nlptools::postagger::Token(tok, 0, tok.size()));
	}

	vector<map<string, double>> tagp;
	predictor->predict(sent, tagp);

	for (int i = 0; i < length; i++) {
        auto &tok = sent[i];
        sequence[i].pos = strdup(tok.prd.c_str());
	}
}

extern "C" nlptools::postagger::Predictor* libpredict_predictor_from_file(char const* filename) {
	Model *crfm = new Model();
	if (!crfm->load_from_file(filename)) {
		return nullptr;
	}
	nlptools::postagger::Predictor *p = new Predictor(*crfm);
	return p;
}

extern "C" void libpredict_free_predictor(Predictor* p) {
	if (p) {
		delete &p->getModel();
		delete p;
	}
}