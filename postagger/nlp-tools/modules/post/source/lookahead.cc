/*
 * $Id$
 */


#include <post/Model.hh>
#include <vector>
#include <cmath>
#include <cfloat>
#include <map>

#include "Feature.hh"
#include "StringBag.hh"

using namespace std;

bool USE_EDGE_TRIGRAMS = false;

//const static int PERCEPTRON_NITER = 7;
//const static int PERCEPTRON_NITER = 20;
const static int PERCEPTRON_NITER = 10;
//const static int PERCEPTRON_NITER = 8;

//int LOOKAHEAD_DEPTH = 1;
//int LOOKAHEAD_DEPTH = 2;
//int LOOKAHEAD_DEPTH = 3;

const double PERCEPTRON_MARGIN = 40;
//const double PERCEPTRON_MARGIN = 0;
//const double PERCEPTRON_MARGIN = 100;

const static int HV_OFFSET = 3; // Should be equals to the maximum lookahead depth possible

namespace nlptools {
namespace postagger {


void Model::lookahead_initialize_state_weights(const Sequence & seq)
{
  vector<double> powv(_num_classes);
  for (size_t i = 0; i < seq.vs.size(); i++) {
    powv.assign(_num_classes, 0.0);
    const Sample & s = seq.vs[i];
    for (vector<int>::const_iterator j = s.positive_features.begin(); j != s.positive_features.end(); j++){
      for (vector<int>::const_iterator k = _feature2mef[*j].begin(); k != _feature2mef[*j].end(); k++) {
	const double w = _vl[*k];
	powv[_fb.getFeature(*k).label()] += w;
      }
    }

    for (int j = 0; j < _num_classes; j++) {
      state_weight((int)i, j) = powv[j];
    }
  }
}

double Model::lookahead_search(const Sequence & seq,
				   vector<int> & history,
				   const int start,
				   const int max_depth,  const int depth,
				   double current_score,
				   vector<int> & best_seq,
				   const bool follow_gold,
				   const vector<int> *forbidden_seq)
{
  assert(history[HV_OFFSET + start - 1 + depth] >= 0);
  assert(history[HV_OFFSET + start - 1] >= 0);

  if (current_score > 0.001 * DBL_MAX || current_score < -0.001 * DBL_MAX) {
    cerr << "error: overflow in lookahead_search()" << endl; exit(1);
  }

  //  if (forbidden_seq && depth > 0) {
  if (forbidden_seq && depth == 1) {
    if ( (*forbidden_seq)[depth - 1] != history[HV_OFFSET + start + depth - 1])
      forbidden_seq = NULL;
  }

  // terminal (leaf) node
  if (depth >= max_depth || start + depth >= (int)seq.vs.size()) {
    best_seq.clear();
    if (forbidden_seq) return current_score;
    else               return current_score + PERCEPTRON_MARGIN;
  }

  double m = -DBL_MAX;
  for (int i = 0; i < _num_classes; i++) {
    if (follow_gold && i != seq.vs[start + depth].label) continue;

    double new_score = current_score;
    // edge unigram features (state bigrams)
    new_score += _vl[edge_feature_id(history[HV_OFFSET + start + depth -  1], i)];

    // edge bigram features (state trigrams)
    if (depth + start > 0)
      new_score += _vl[edge_feature_id2(history[HV_OFFSET + start + depth - 2], history[HV_OFFSET + start + depth - 1], i)];

    // edge trigram features (state 4-grams)
    if (USE_EDGE_TRIGRAMS) {
      if (depth + start > 1)
	new_score += _vl[edge_feature_id3(history[HV_OFFSET + start + depth - 3], history[HV_OFFSET + start + depth - 2], history[HV_OFFSET + start + depth - 1], i)];
    }

    // state + observation features
    new_score += state_weight(start + depth, i);

    history[HV_OFFSET + start + depth] = i;

    vector<int> tmp_seq;
    const double score = lookahead_search(seq, history, start, max_depth, depth + 1, new_score, tmp_seq, false, forbidden_seq);
    //    const double score = lookahead_search(seq, history, start, max_depth, depth + 1, new_score, tmp_seq, follow_gold, forbidden_seq);
    if (score > m) {
      m = score;
      best_seq.clear();
      best_seq.push_back(i);
      copy(tmp_seq.begin(), tmp_seq.end(), back_inserter(best_seq));
    }
  }

  return m;
}

void Model::calc_diff(const double val,
			  const Sequence & seq,
			  const int start,
			  const vector<int> & history,
			  const int depth, const int max_depth,
			  map<int, double> & diff)
{
  if (start + depth == (int)seq.vs.size()) return;
  //  if (depth >= LOOKAHEAD_DEPTH) return;
  if (depth >= max_depth) return;

  const int label = history[HV_OFFSET + start + depth];

  int eid = -1;
  eid = edge_feature_id(history[HV_OFFSET + start + depth -  1], label);
  assert(eid >= 0);
  diff[eid] += val;

  int eid2 = -1;
  eid2 = edge_feature_id2(history[HV_OFFSET + start + depth - 2], history[HV_OFFSET + start + depth - 1], label);
  //  assert(eid2 >= 0);
  if (eid2 >= 0) diff[eid2] += val;

  if (USE_EDGE_TRIGRAMS) {
    const int eid3 = edge_feature_id3(history[HV_OFFSET + start + depth - 3], history[HV_OFFSET + start + depth - 2], history[HV_OFFSET + start + depth - 1], label);
    if (eid3 >= 0) diff[eid3] += val;
  }

  assert(start + depth < (int)seq.vs.size());
  const Sample & s = seq.vs[start + depth];
  for (vector<int>::const_iterator j = s.positive_features.begin(); j != s.positive_features.end(); j++){
    for (vector<int>::const_iterator k = _feature2mef[*j].begin(); k != _feature2mef[*j].end(); k++) {
      if (_fb.getFeature(*k).label() == label)
	diff[*k] += val;
    }
  }

  calc_diff(val, seq, start, history, depth + 1, max_depth, diff);
}


int Model::update_weights_sub2(const Sequence & seq,
				   vector<int> & history,
				   const int x,
				   map<int, double> & diff)
{
  // gold-standard sequence
  vector<int> gold_seq;
  /*const double gold_score =*/ lookahead_search(seq, history, x, lookaheadDepth, 0, 0, gold_seq, true);

  //    cout << "gold = " << gold << " score = " << gold_score << endl;
  //        print_bestsq(gold_seq);

  vector<int> best_seq;
  /*const double score =*/ lookahead_search(seq, history, x, lookaheadDepth, 0, 0, best_seq, false, &gold_seq);

  //       print_bestsq(best_seq);

  //  if (best_seq == gold_seq) return 0;
  if (best_seq.front() == gold_seq.front()) return 0;

  map<int, double> vdiff;

  copy(gold_seq.begin(), gold_seq.end(), history.begin() + HV_OFFSET + x);
  calc_diff( 1, seq, x, history, 0, lookaheadDepth, vdiff);
  copy(best_seq.begin(), best_seq.end(), history.begin() + HV_OFFSET + x);
  calc_diff(-1, seq, x, history, 0, lookaheadDepth, vdiff);


  for (map<int, double>::const_iterator j = vdiff.begin(); j != vdiff.end(); j++) {
    diff[j->first] += j->second;
  }

  return 1;
}


int Model::lookaheadtrain_sentence(const Sequence & seq, int & t, vector<double> & wa)
{
  //    lookahead_initialize_edge_weights();  // to be removed
  lookahead_initialize_state_weights(seq);

  const int len = (int) seq.vs.size();

  vector<int> history(len + HV_OFFSET, -1);
  fill(history.begin(), history.begin() + HV_OFFSET, _num_classes); // BOS
  int error_num = 0;
  for (int x = 0; x < len; x++) {

    map<int, double> diff;

    error_num += update_weights_sub2(seq, history, x, diff);
    history[HV_OFFSET + x] = seq.vs[x].label;

    for (map<int, double>::const_iterator i = diff.begin(); i != diff.end(); i++) {
      //	    cout << "(" << i->first << ", " << i->second << ") ";
      const double v = 1.0 * i->second;
      _vl[i->first] += v;
      wa[i->first] += t * v;
    }
    t++;

  }

  return error_num;
}

double
Model::heldout_lookahead_error()
{
  size_t nerrors = 0, total_len = 0;

  //    lookahead_initialize_edge_weights();  // to be removed
  for (std::vector<Sequence>::const_iterator i = _heldout.begin(); i != _heldout.end(); i++) {
    total_len += i->vs.size();

    vector<int> vs(i->vs.size());
    decode_lookahead_sentence(*i, vs);

    for (size_t j = 0; j < vs.size(); j++) {
      if (vs[j] != i->vs[j].label) nerrors++;
    }

  }
  _heldout_error = (double)nerrors / (double)total_len;

  return 0;
}


int
Model::perform_LookaheadTraining()
{
  cerr << "lookahead depth = " << lookaheadDepth << endl;
  cerr << "perceptron margin = " << PERCEPTRON_MARGIN << endl;
  cerr << "perceptron niter = " << PERCEPTRON_NITER << endl;

  const int dim = _fb.size();

  vector<double> wa(dim, 0);

  int t = 1;

  //    vector<int> r(_vs.size());
  //    for (int i = 0; i < (int)_vs.size(); i++) r[i] = i;
  //    random_shuffle(r.begin(), r.end());

  int iter = 0;
  while (iter < PERCEPTRON_NITER) {

    iter++;

    vector<int> r(_vs.size());
    for (int i = 0; i < (int)_vs.size(); i++) r[i] = i;
    random_shuffle(r.begin(), r.end());

    int error_num = 0;
    //    int rest = _vs.size();
    for (int i = 0; i < (int)_vs.size(); i++) {
      const Sequence & seq = _vs[r[i]];

      error_num += lookaheadtrain_sentence(seq, t, wa);

      /*
      for (map<int, double>::const_iterator i = diff.begin(); i != diff.end(); i++) {
	//	    cout << "(" << i->first << ", " << i->second << ") ";
	const double v = 1.0 * i->second;
	_vl[i->first] += v;
	wa[i->first] += t * v;
      }
      */
    }
    //    cout << endl;
    cerr << "iter = " << iter << " num_errors = " << error_num;

    if (_heldout.size() > 0) {
      vector<double> tmpvl = _vl;
      for (int i = 0; i < dim; i++) _vl[i] -= wa[i] / t;
      heldout_lookahead_error();
      cerr << "\theldout_error = " << _heldout_error;
      _vl = tmpvl;
    }
    cerr << endl;

    if (error_num == 0) break;
  }

  for (int i = 0; i < dim; i++) _vl[i] -= wa[i] / t;


  return 0;
}


int Model::decode_lookahead_sentence(
    const Sequence & seq,
    vector<int> & vs )
{
  //    lookahead_initialize_edge_weights();  // to be removed
  lookahead_initialize_state_weights(seq);

  const int len = (int) seq.vs.size();

  vector<int> history(len + HV_OFFSET, -1);
  fill(history.begin(), history.begin() + HV_OFFSET, _num_classes); // BOS
  int error_num = 0;
  for (int x = 0; x < len; x++) {

    vector<int> bestsq;
    /*const double score =*/ lookahead_search(seq, history, x, lookaheadDepth, 0, 0, bestsq);

    vs[x] = bestsq.front();
    history[HV_OFFSET + x] = vs[x];
  }

  return error_num;
}


void Model::decode_lookahead(CRF_Sequence & s0)
{
  if (s0.size() >= MAX_LEN) {
    cerr << "error: sequence is too long." << endl;
    return;
  }

  Sequence seq;

  for (CRF_Sequence::const_iterator i = s0.begin(); i != s0.end(); i++) {
    Sample s;
    for (vector<string>::const_iterator j = i->features.begin(); j != i->features.end(); j++) {
      const int id = _featurename_bag.Id(*j);
      if (id >= 0) s.positive_features.push_back(id);
    }
    seq.vs.push_back(s);
  }

  vector<int> vs(seq.vs.size());
  decode_lookahead_sentence(seq, vs);

  for (size_t i = 0; i < seq.vs.size(); i++) {
    s0[i].label = _label_bag.Str(vs[i]);
  }
}


}}
