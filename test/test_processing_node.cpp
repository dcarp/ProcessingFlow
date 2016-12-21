#include <vector>

#include <gtest/gtest.h>

#include "processing_node.h"

using namespace tbb::flow;

using namespace ProcessingFlow;

class ProcessingNodeTest : public ::testing::Test
{
public:
    using Input = float;
    using Output = Input;
    using Outputs = std::vector<Output>;
    using Parameters = int;
    using ParameterSets = std::vector<Parameters>;
    using Errors = std::vector<Error>;

    ProcessingNodeTest()
        : processingNode(graph, [](Input f, Parameters i) { return i * f; }),
          outputCollector(
              graph, serial,
              [this](const Output &output) { outputs.push_back(output); }),
          parametersCollector(graph, serial,
                              [this](const Parameters &parameters) {
                                  parameterSets.push_back(parameters);
                              }),
          errorCollector(graph, serial, [this](const Error &error) {
              errors.push_back(error);
          })
    {
        make_edge(output_port<flowOutputPort>(processingNode), outputCollector);
        make_edge(output_port<parametersOutputPort>(processingNode),
                  parametersCollector);
        make_edge(output_port<errorOutputPort>(processingNode), errorCollector);
    }

protected:
    Outputs outputs;
    ParameterSets parameterSets;
    Errors errors;

    tbb::flow::graph graph;
    ProcessingNode<Input, Parameters> processingNode;

private:
    function_node<Output> outputCollector;
    function_node<Parameters> parametersCollector;
    function_node<Error> errorCollector;
};

TEST_F(ProcessingNodeTest, CreatesInstanceWithParameters)
{
    graph.wait_for_all();

    EXPECT_EQ(processingNode.name(), "");
    EXPECT_EQ(Outputs(), outputs);
    EXPECT_EQ(ParameterSets(), parameterSets);
    EXPECT_EQ(Errors(), errors);
}

TEST_F(ProcessingNodeTest, SetParameters)
{
    input_port<parametersInputPort>(processingNode).try_put(3);
    input_port<parametersInputPort>(processingNode).try_put(5);

    graph.wait_for_all();

    EXPECT_EQ(Outputs(), outputs);
    EXPECT_EQ(ParameterSets({3, 5}), parameterSets);
    EXPECT_EQ(Errors(), errors);
}

TEST_F(ProcessingNodeTest, SetInput)
{
    input_port<parametersInputPort>(processingNode).try_put(3);
    input_port<flowInputPort>(processingNode).try_put(2.0);
    input_port<flowInputPort>(processingNode).try_put(4.0);

    graph.wait_for_all();

    EXPECT_EQ(Outputs({6.0, 12.0}), outputs);
    EXPECT_EQ(ParameterSets({3}), parameterSets);
    EXPECT_EQ(Errors(), errors);
}

TEST_F(ProcessingNodeTest, SetInputNoParameters)
{
    input_port<flowInputPort>(processingNode).try_put(2.0);
    input_port<flowInputPort>(processingNode).try_put(4.0);

    graph.wait_for_all();

    EXPECT_EQ(Outputs(), outputs);
    EXPECT_EQ(ParameterSets(), parameterSets);
    EXPECT_EQ(Errors({MAKE_ERROR("No processing parameters"),
                      MAKE_ERROR("No processing parameters")}),
              errors);
}

TEST_F(ProcessingNodeTest, SetError)
{
    input_port<errorInputPort>(processingNode)
        .try_put(MAKE_ERROR("First error"));
    input_port<errorInputPort>(processingNode)
        .try_put(MAKE_ERROR("Second error"));

    graph.wait_for_all();

    EXPECT_EQ(Outputs(), outputs);
    EXPECT_EQ(ParameterSets(), parameterSets);
    EXPECT_EQ(Errors({MAKE_ERROR("First error"), MAKE_ERROR("Second error")}),
              errors);
}

TEST_F(ProcessingNodeTest, SetAll)
{
    input_port<flowInputPort>(processingNode).try_put(2.0);
    graph.wait_for_all();
    input_port<parametersInputPort>(processingNode).try_put(3);
    input_port<flowInputPort>(processingNode).try_put(4.0);
    graph.wait_for_all();
    input_port<parametersInputPort>(processingNode).try_put(5);
    input_port<parametersInputPort>(processingNode).try_put(3);
    input_port<flowInputPort>(processingNode).try_put(2.0);
    input_port<flowInputPort>(processingNode).try_put(4.0);
    input_port<errorInputPort>(processingNode)
        .try_put(MAKE_ERROR("First error"));
    input_port<errorInputPort>(processingNode)
        .try_put(MAKE_ERROR("Second error"));

    graph.wait_for_all();

    EXPECT_EQ(Outputs({12.0, 6.0, 12.0}), outputs);
    EXPECT_EQ(ParameterSets({3, 5, 3}), parameterSets);
    EXPECT_EQ(Errors({MAKE_ERROR("No processing parameters"),
                      MAKE_ERROR("First error"), MAKE_ERROR("Second error")}),
              errors);
}
