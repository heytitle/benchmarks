/**
 * @file mlp_forward.cpp
 * @author Marcus Edel
 *
 * Code to benchmark the mlpack ann code.
 */

#include <mlpack/prereqs.hpp>
#include <mlpack/core/util/cli.hpp>

#include <mlpack/methods/ann/layer/layer.hpp>
#include <mlpack/methods/ann/ffn.hpp>

#include <mlpack/core/util/timers.hpp>

// Information about the program itself.
PROGRAM_INFO("MLP Forward", "Simple program to benchmark the mlpack ann code.");
PARAM_INT_IN("input_size", "The input size.", "i", 10000);
PARAM_INT_IN("hidden_size", "The hidden size.", "d", 100);
PARAM_INT_IN("output_size", "The output size.", "o", 10);

using namespace mlpack;
using namespace mlpack::ann;

int main(int argc, char **argv)
{
  // Parse command line options.
  CLI::ParseCommandLine(argc, argv);

  const size_t inputSize = CLI::GetParam<int>("input_size");
  const size_t hiddenSize = CLI::GetParam<int>("hidden_size");
  const size_t outSize = CLI::GetParam<int>("output_size");

  Timer::Start("model_building");
  FFN<NegativeLogLikelihood<> > model;
  model.Add<Linear<> >(inputSize, hiddenSize);
  model.Add<SigmoidLayer<> >();
  model.Add<Linear<> >(hiddenSize, hiddenSize);
  model.Add<SigmoidLayer<> >();
  model.Add<Linear<> >(hiddenSize, hiddenSize);
  model.Add<SigmoidLayer<> >();
  model.Add<Linear<> >(hiddenSize, outSize);
  model.Add<LogSoftMax<> >();
  Timer::Stop("model_building");

  arma::mat input = arma::ones(inputSize, 1);
  arma::mat output;

  // Initilize the model.
  model.Predict(input, output);

  Timer::Start("forward");
  model.Predict(input, output);
  Timer::Stop("forward");

  return 0;
}
