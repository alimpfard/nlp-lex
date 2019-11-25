/*
 * $Id$
 */

#include <post/Model.hh>
#include <cmath>
#include <cstdio>
#include <cfloat>
#include <set>
#include "Feature.hh"
#include "StringBag.hh"
//#include "lbfgs.h"

extern bool USE_EDGE_TRIGRAMS;


namespace nlptools {
namespace postagger {


using namespace std;



const string BOS_LABEL = "!BOS!";
const string EOS_LABEL = "!EOS!";
//const bool USE_BOS_EOS = true;  // need to fix load_from_file(). BOE and EOS should be put first.
const bool USE_BOS_EOS = false;

const bool OUTPUT_MARGINAL_PROB = true;

//const double MIN_FEATURE_WEIGHT = 0.001; // prune features when saving


#define CALLED_THIS      //std::cout << __FUNCTION__ << std::endl;


CRF_State::CRF_State() : label("") {};
CRF_State::CRF_State(const std::string & l) : label(l) {};
void CRF_State::set_label(const std::string & l) { label = l; }

// to add a binary feature
void CRF_State::add_feature(const std::string & f)
{
    if (f.find_first_of('\t') != std::string::npos)
    {
        std::cerr << "error: illegal characters in a feature string" << std::endl;
        exit(1);
    }
    features.push_back(f);
}


void CRF_Sequence::add_state(
    const CRF_State & state )
{
    content.push_back(state);
}


CRF_State &CRF_Sequence::operator [](
    size_t index )
{
    return content[index];
}

const CRF_State &CRF_Sequence::operator [](
    size_t index ) const
{
    return content[index];
}

size_t CRF_Sequence::size() const
{
    return content.size();
}

void CRF_Sequence::push_back(
    const CRF_State &item )
{
    content.push_back(item);
}

void CRF_Sequence::pop_back()
{
    content.pop_back();
}

CRF_Sequence::iterator CRF_Sequence::begin()
{
    return content.begin();
}

CRF_Sequence::iterator CRF_Sequence::end()
{
    return content.end();
}

CRF_Sequence::const_iterator CRF_Sequence::begin() const
{
    return content.begin();
}

CRF_Sequence::const_iterator CRF_Sequence::end() const
{
    return content.end();
}


Model::Model() :  _label_bag(*new StringBag()), _fb(*new FeatureBag())
{
    CALLED_THIS;
    _nheldout = 0;
    _early_stopping_n = 0;
    //_line_counter = 0;
    lookaheadDepth = 2;

    if (USE_EDGE_TRIGRAMS) {
      p_edge_feature_id3  = (int*)malloc(sizeof(int) * MAX_LABEL_TYPES * MAX_LABEL_TYPES * MAX_LABEL_TYPES * MAX_LABEL_TYPES);
      p_edge_weight3     = (double*)malloc(sizeof(double) * MAX_LABEL_TYPES * MAX_LABEL_TYPES * MAX_LABEL_TYPES * MAX_LABEL_TYPES);
    }
    p_edge_feature_id2  = (int*)malloc(sizeof(int) * MAX_LABEL_TYPES * MAX_LABEL_TYPES * MAX_LABEL_TYPES);
    p_edge_feature_id  = (int*)malloc(sizeof(int) * MAX_LABEL_TYPES * MAX_LABEL_TYPES);
    p_state_weight     = (double*)malloc(sizeof(double) * MAX_LEN * MAX_LABEL_TYPES);
    p_edge_weight      = (double*)malloc(sizeof(double) * MAX_LABEL_TYPES * MAX_LABEL_TYPES);
    p_edge_weight2     = (double*)malloc(sizeof(double) * MAX_LABEL_TYPES * MAX_LABEL_TYPES * MAX_LABEL_TYPES);
    p_forward_cache    = (double*)malloc(sizeof(double) * MAX_LEN * MAX_LABEL_TYPES);
    p_backward_cache   = (double*)malloc(sizeof(double) * MAX_LEN * MAX_LABEL_TYPES);
    p_backward_pointer = (int*)malloc(sizeof(int) * MAX_LEN * MAX_LABEL_TYPES);

}

Model::~Model()
{
  CALLED_THIS;
    if (USE_EDGE_TRIGRAMS) {
      free(p_edge_feature_id3);
      free(p_edge_weight3);
    }
    free(p_edge_feature_id2);
    free(p_edge_feature_id);
    free(p_state_weight);
    free(p_edge_weight2);
    free(p_edge_weight);
    free(p_forward_cache);
    free(p_backward_cache);
    free(p_backward_pointer);
}


void Model::initialize_edge_weights()
{
  CALLED_THIS;
  for (int i = 0; i < _label_bag.size(); i++) {
    for (int j = 0; j < _label_bag.size(); j++) {
      const int id = edge_feature_id(i, j);
      //      if (id < 0) { edge_weight[i][j] = 1; continue; }
      assert(id >= 0);
      const double ew = _vl[id];
      edge_weight(i, j) = exp(ew);
    }
  }

}



int
Model::make_feature_bag(const int cutoff)
{
  CALLED_THIS;
#ifdef USE_HASH_MAP
  //  typedef __gnu_cxx::hash_map<mefeature_type, int> map_type;
  typedef std::tr1::unordered_map<mefeature_type, int> map_type;
#else
  typedef std::map<mefeature_type, int> map_type;
#endif

  map_type count;
  if (cutoff > 0) {
    for (vector<Sequence>::const_iterator k = _vs.begin(); k != _vs.end(); k++) {
      for (vector<Sample>::const_iterator i = k->vs.begin(); i != k->vs.end(); i++) {
	for (vector<int>::const_iterator j = i->positive_features.begin(); j != i->positive_features.end(); j++) {
	  count[Feature(i->label, *j).body()]++;
	}
      }
    }
  }

  for (vector<Sequence>::const_iterator k = _vs.begin(); k != _vs.end(); k++) {
    for (vector<Sample>::const_iterator i = k->vs.begin(); i != k->vs.end(); i++) {
      for (vector<int>::const_iterator j = i->positive_features.begin(); j != i->positive_features.end(); j++) {
	const Feature feature(i->label, *j);
	if (cutoff > 0 && count[feature.body()] <= cutoff) continue;
	_fb.put(feature);
      }
    }
  }

  init_feature2mef();

  return 0;
}


inline bool contain_space(const string & s)
{
  for (size_t i = 0; i < s.size(); i++) {
    if (isspace(s[i])) return true;
  }
  return false;
}

void
Model::add_training_sample(const CRF_Sequence & seq)
{
  CALLED_THIS;
  if (seq.size() >= MAX_LEN) {
    cerr << "error: sequence is too long.";
    exit(1);
  }
  if (seq.size() == 0) {
    cerr << "warning: empty sentence" << endl;
    return;
  }
  assert(seq.size() > 0);

  Sequence s1;
  for (CRF_Sequence::const_iterator i = seq.begin(); i != seq.end(); i++) {
    if (i->label == BOS_LABEL || i->label == EOS_LABEL) {
      cerr << "error: the label name \"" << i->label << "\" is reserved. Use a different name.";
      exit(1);
    }
    if (contain_space(i->label)) {
      cerr << "error: the name of a label must not contain any space." << endl; exit(1);
    }
    Sample s;
    s.label = _label_bag.Put(i->label);
    if (s.label >= MAX_LABEL_TYPES - 2) {
      cerr << "error: too many types of labels." << endl;
      exit(1);
    }
    assert(s.label >= 0 && s.label < MAX_LABEL_TYPES);
    for (vector<string>::const_iterator j = i->features.begin(); j != i->features.end(); j++) {
      if (contain_space(*j)) {
	cerr << "error: the name of a feature must not contain any space." << endl; exit(1);
      }
      s.positive_features.push_back(_featurename_bag.Put(*j));
    }
    s1.vs.push_back(s);
  }
  _vs.push_back(s1);
}

int
Model::train(
    const int cutoff,
		const double sigma,
    const double widthfactor )
		 //		 const double Nsigma2, const double widthfactor)
{
  CALLED_THIS;
  if (sigma > 0 && widthfactor > 0) {
  //  if (Nsigma2 > 0 && widthfactor > 0) {
    cerr << "error: Gausian prior and inequality modeling cannot be used together." << endl;
    return 0;
  }
  if (_vs.size() == 0) {
    cerr << "error: no training data." << endl;
    return 0;
  }
  if (_nheldout >= (int)_vs.size()) {
    cerr << "error: too much heldout data. no training data is available." << endl;
    return 0;
  }
  //if (_nheldout > 0) random_shuffle(_vs.begin(), _vs.end());

  _label_bag.Put(BOS_LABEL);
  _label_bag.Put(EOS_LABEL);
  _num_classes = _label_bag.size() - 2;

  for (int i = 0; i < _nheldout; i++) {
    _heldout.push_back(_vs.back());
    _vs.pop_back();
  }

  size_t total_len = 0;
  for (size_t i = 0; i < _vs.size(); i++) total_len += _vs[i].vs.size();

  //  _sigma = sqrt((double)total_len / Nsigma2);
  //  if (Nsigma2 == 0) _sigma = 0;
  _sigma = sigma;
  _inequality_width = widthfactor / (double) _vs.size();

  if (cutoff > 0) cerr << "cutoff threshold = " << cutoff << endl;
  //  if (_sigma > 0) cerr << "Gaussian prior sigma = " << _sigma << endl;
    //    cerr << "N*sigma^2 = " << Nsigma2 << " sigma = " << _sigma << endl;
  if (widthfactor > 0)
    cerr << "widthfactor = " << widthfactor << endl;
  cerr << "preparing for estimation...";
  make_feature_bag(cutoff);
  //  _vs.clear();
  cerr << "done" << endl;
  cerr << "number of state types = " << _num_classes << endl;
  cerr << "number of samples = " << _vs.size() << endl;
  cerr << "number of features = " << _fb.size() << endl;

  cerr << "done" << endl;

  _vl.resize(_fb.size());
  _vl.assign(_vl.size(), 0.0);

  perform_LookaheadTraining();


  if (_inequality_width > 0) {
    int sum = 0;
    for (int i = 0; i < _fb.size(); i++) {
      if (_vl[i] != 0) sum++;
    }
    cerr << "number of active features = " << sum << endl;
  }

  return 0;
}





bool
Model::load_from_file(const string & filename, bool verbose)
{
  CALLED_THIS;
  FILE * fp = fopen(filename.c_str(), "r");
  if (!fp) {
    cerr << "error: cannot open " << filename << "!" << endl;
    return false;
  }
  if (verbose) {
    cerr << "loading " << filename;
  }

  _vl.clear();
  _label_bag.clear();
  _featurename_bag.clear();
  _fb.clear();
  char buf[1024];
  while(fgets(buf, 1024, fp)) {
    string line(buf);
    string::size_type t1 = line.find_first_of('\t');
    string::size_type t2 = line.find_last_of('\t');
    string classname = line.substr(0, t1);
    string featurename = line.substr(t1 + 1, t2 - (t1 + 1) );
    float lambda;
    string w = line.substr(t2+1);
    sscanf(w.c_str(), "%f", &lambda);

    const int label = _label_bag.Put(classname);
    const int feature = _featurename_bag.Put(featurename);
    _fb.put(Feature(label, feature));
    _vl.push_back(lambda);
  }

  // for zero-wight edges
  _label_bag.Put(BOS_LABEL);
  _label_bag.Put(EOS_LABEL);
  for (int i = 0; i < _label_bag.size(); i++) {
    for (int j = 0; j < _label_bag.size(); j++) {
      const string & label1 = _label_bag.Str(j);
      const int l1 = _featurename_bag.Put("->\t" + label1);
      const int id = _fb.getId(Feature(i, l1));
      if (id < 0) {
	_fb.put(Feature(i, l1));
	_vl.push_back(0);
      }
    }
  }
  for (int i = 0; i < _label_bag.size(); i++) {
    for (int j = 0; j < _label_bag.size(); j++) {
      for (int k = 0; k < _label_bag.size(); k++) {
	const string & label1 = _label_bag.Str(j);
	const string & label2 = _label_bag.Str(k);
	const int l1 = _featurename_bag.Put("->\t" + label1 + "\t->\t" + label2);
	const int id = _fb.getId(Feature(i, l1));
	if (id < 0) {
	  _fb.put(Feature(i, l1));
	  _vl.push_back(0);
	}
      }
    }
  }
  if (USE_EDGE_TRIGRAMS) {
    for (int i = 0; i < _label_bag.size(); i++) {
      for (int j = 0; j < _label_bag.size(); j++) {
        for (int k = 0; k < _label_bag.size(); k++) {
	  for (int l = 0; l < _label_bag.size(); l++) {
	    const string & label1 = _label_bag.Str(j);
	    const string & label2 = _label_bag.Str(k);
	    const string & label3 = _label_bag.Str(l);
	    const int l1 = _featurename_bag.Put("->\t" + label1 + "\t->\t" + label2 + "\t->\t" + label3);
	    const int id = _fb.getId(Feature(i, l1));
	    if (id < 0) {
	      _fb.put(Feature(i, l1));
	      _vl.push_back(0);
	    }
	  }
        }
      }
    }
  }


  _num_classes = _label_bag.size() - 2;

  init_feature2mef();
  initialize_edge_weights();

  fclose(fp);

  if (verbose) {
    cerr << "...done" << endl;
  }

  return true;
}

void
Model::init_feature2mef()
{
  CALLED_THIS;
  // fills '_feature2mef' with the ID for all existing features
  // in the bag
  _feature2mef.clear();
  for (size_t i = 0; i < _featurename_bag.size(); i++) {
    vector<int> vi;
    for (size_t k = 0; k < (size_t) _num_classes; k++) {
      int id = _fb.getId(Feature((int)k, (int)i));
      if (id >= 0) vi.push_back(id);
    }
    _feature2mef.push_back(vi);
  }
#if 1
    for (int i = 0; i < _label_bag.size(); i++)
    {
        for (int j = 0; j < _label_bag.size(); j++)
        {
            const string & label1 = _label_bag.Str(j);
            const int l1 = _featurename_bag.Put("->\t" + label1);
            const int id = _fb.put(Feature(i, l1));
            edge_feature_id(i, j) = id;

            for (int k = 0; k < _label_bag.size(); k++)
            {
                const string & label2 = _label_bag.Str(k);
                const int l2 = _featurename_bag.Put("->\t" + label1 + "\t->\t" + label2);
                const int id = _fb.put(Feature(i, l2));
                edge_feature_id2(i, j, k) = id;
            }

        }
    }
#else
  for (int i = 0; i < _label_bag.Size(); i++) {
    for (int j = 0; j < _label_bag.Size(); j++) {
      const string & label1 = _label_bag.Str(j);
      const int l1 = _featurename_bag.Put("->\t" + label1);
      const int id = _fb.Put(Feature(i, l1));
      edge_feature_id(i, j) = id;
    }
  }

  for (int i = 0; i < _label_bag.Size(); i++) {
    for (int j = 0; j < _label_bag.Size(); j++) {
      for (int k = 0; k < _label_bag.Size(); k++) {
	const string & label1 = _label_bag.Str(j);
	const string & label2 = _label_bag.Str(k);
	const int l1 = _featurename_bag.Put("->\t" + label1 + "\t->\t" + label2);
	const int id = _fb.Put(Feature(i, l1));
	edge_feature_id2(i, j, k) = id;
      }
    }
  }
#endif
  if (USE_EDGE_TRIGRAMS) {
    for (int i = 0; i < _label_bag.size(); i++) {
      for (int j = 0; j < _label_bag.size(); j++) {
	for (int k = 0; k < _label_bag.size(); k++) {
	  for (int l = 0; l < _label_bag.size(); l++) {
	    const string & label1 = _label_bag.Str(j);
	    const string & label2 = _label_bag.Str(k);
	    const string & label3 = _label_bag.Str(l);
	    const int l1 = _featurename_bag.Put("->\t" + label1 + "\t->\t" + label2 + "\t->\t" + label3);
	    const int id = _fb.put(Feature(i, l1));
	    edge_feature_id3(i, j, k, l) = id;
	  }
	}
      }
    }
  }


}

bool
Model::save_to_file(const string & filename, double th) const
{
    CALLED_THIS;
    FILE * fp = fopen(filename.c_str(), "wt");
    if (!fp) return false;

    th = abs(th);

    /*NameTable::const_iterator i = _featurename_bag.begin();
    for (; i != _featurename_bag.end(); i++)
    {
        for (int j = 0; j < _label_bag.size(); j++)
        {
            string label = _label_bag.Str(j);
            string history = i->first;
            int id = _fb.getId(Feature(j, i->second));
            if (id < 0) continue;
            if (abs(_vl[id]) < th) continue; // cut off low-weight features
            fprintf(fp, "%s\t%s\t%f\n", label.c_str(), history.c_str(), _vl[id]);
        }
    }*/


    //  for (MiniStringBag::map_type::const_iterator i = _featurename_bag.begin();
    for (StrDic::const_Iterator i = _featurename_bag.begin();
    i != _featurename_bag.end(); i++) {
    for (int j = 0; j < _label_bag.size(); j++) {
    string label = _label_bag.Str(j);
    string history = i.getStr();
    int id = _fb.getId(Feature(j, i.getId()));
    if (id < 0) continue;
    if (abs(_vl[id]) < th) continue; // cut off low-weight features
    fprintf(fp, "%s\t%s\t%f\n", label.c_str(), history.c_str(), _vl[id]);
    }
    }

    fclose(fp);

    return true;
}


std::string Model::get_class_label(int i) const { return _label_bag.Str(i); }
int Model::get_class_id(const std::string & s) const { return _label_bag.Id(s); }

}}
