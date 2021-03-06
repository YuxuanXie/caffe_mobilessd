#include <gflags/gflags.h>
#include <glog/logging.h>
#include <string.h>

#include <cstring>
#include <map>
#include <string>
#include <vector>

#include "boost/algorithm/string.hpp"
#include "caffe/caffe.hpp"
#include "caffe/util/signal_handler.h"



using caffe::Blob;
using caffe::Caffe;
using caffe::Net;
using caffe::Layer;
using caffe::Solver;
using caffe::shared_ptr;
using caffe::string;
using caffe::Timer;
using caffe::vector;
using std::ostringstream;



DEFINE_string(solver, "",
    "The solver definition protocol buffer text file.");
DEFINE_string(weights, "",
    "the pretrained weights to initialize finetuning, "
    "separated by ','. Cannot be set simultaneously with snapshot.");
DEFINE_string(snapshot, "",
    "Optional; the snapshot solver state to resume training.");


// Load the weights from the specified caffemodel(s) into the train and
// test nets.
void CopyLayers(caffe::Solver<float>* solver, const std::string& model_list) {
  std::vector<std::string> model_names;
  boost::split(model_names, model_list, boost::is_any_of(",") );
  for (int i = 0; i < model_names.size(); ++i) {
    LOG(INFO) << "Finetuning from " << model_names[i];
    solver->net()->CopyTrainedLayersFrom(model_names[i]);
    for (int j = 0; j < solver->test_nets().size(); ++j) {
      solver->test_nets()[j]->CopyTrainedLayersFrom(model_names[i]);
    }
  }
}


void convert()
{
	CHECK_GT(FLAGS_weights.size(), 0) << "Need model weights to score.";
	CHECK_GT(FLAGS_solver.size(), 0) << "Need model weights to score.";

	caffe::SolverParameter solver_param;
	caffe::ReadSolverParamsFromTextFileOrDie(FLAGS_solver, &solver_param);


	caffe::NetParameter net_param;
	if(solver_param.has_train_net())
	{
		LOG_IF(INFO, Caffe::root_solver())
		        << "Creating training net from train_net file: " << solver_param.train_net();
		    ReadNetParamsFromTextFileOrDie(solver_param.train_net(), &net_param);
	}

	net_param.mutable_state()->set_phase(caffe::TEST);

	Net<float>* net_float32;
	net_float32 = new Net<float>(net_param);
	net_float32->CopyTrainedLayersFrom(FLAGS_weights);


	vector<shared_ptr<Layer<float> > > layers_float32 = net_float32->layers();
	for(int index = 0; index < layers_float32.size(); ++index)
	{
		if(strcmp(layers_float32[index]->type(), "Convolution") == 0)
		{

		}
	}

	LOG(INFO)<<"OK" << std::endl;

}


int main(int argc, char** argv) {
  // Print output to stderr (while still logging).
  FLAGS_alsologtostderr = 1;
  // Set version
  gflags::SetVersionString(AS_STRING(CAFFE_VERSION));
  // Run tool or show usage.
  caffe::GlobalInit(&argc, &argv);
  if (argc == 2) {
      convert();
  }
  return 0;
}
    
