#ifndef _PROCESSING_FLOW_PROCESSING_NODE_H_
#define _PROCESSING_FLOW_PROCESSING_NODE_H_

#include <tbb/flow_graph.h>

#include "error.h"

namespace ProcessingFlow
{

#define FlowDataType typename
#define ParameterType typename
#define BodyType typename

class TwoTrackNode
{
public:
};

template <FlowDataType Input, ParameterType Parameters,
          FlowDataType Output = Input>
class ProcessingNode
    : public tbb::flow::composite_node<std::tuple<Input, Parameters, ErrorT>,
                                       std::tuple<Output, Parameters, ErrorT>>,
      public TwoTrackNode
{
    using Base
        = tbb::flow::composite_node<std::tuple<Input, Parameters, ErrorT>,
                                    std::tuple<Output, Parameters, ErrorT>>;
    using Multiplexer = tbb::flow::indexer_node<Input, ErrorT>;
    enum {
        MULTIPLEXER_INPUT_PORT = 0,
        MULTIPLEXER_ERROR_PORT = 1,
    };

    using ProcessingFunction
        = tbb::flow::multifunction_node<typename Multiplexer::output_type,
                                        std::tuple<Output, ErrorT>>;
    enum {
        PROCESSING_FUNCTION_OUTPUT_PORT = 0,
        PROCESSING_FUNCTION_ERROR_PORT = 1,
    };

    tbb::flow::overwrite_node<Parameters> parameters;
    Multiplexer multiplexer;
    ProcessingFunction processingFunction;

    const std::string name_;

    template <BodyType Body> struct ProcessingBody {
        ProcessingBody(Body &body, ProcessingNode &outer)
            : body_(body), outer_(outer)
        {
        }
        ProcessingBody() = delete;

        void operator()(const typename Multiplexer::output_type &input,
                        typename ProcessingFunction::output_ports_type &output)
        {
            switch (input.tag()) {
            case MULTIPLEXER_INPUT_PORT:
                if (!outer_.parameters.is_valid()) {
                    std::get<PROCESSING_FUNCTION_ERROR_PORT>(output).try_put(
                        MAKE_ERROR("No processing parameters"));
                    return;
                }

                try {
                    Parameters parameters;
                    outer_.parameters.try_get(parameters);

                    std::get<PROCESSING_FUNCTION_OUTPUT_PORT>(output).try_put(
                        body_(tbb::flow::cast_to<Input>(input), parameters));
                } catch (const std::exception &exception) {
                    std::get<PROCESSING_FUNCTION_ERROR_PORT>(output).try_put(
                        MAKE_ERROR(exception.what()));
                }

                break;
            case MULTIPLEXER_ERROR_PORT:
                std::get<PROCESSING_FUNCTION_ERROR_PORT>(output).try_put(
                    tbb::flow::cast_to<ErrorT>(input));
                break;
            }
        }

    private:
        Body &body_;
        ProcessingNode &outer_;
    };

public:
    template <BodyType Body>
    ProcessingNode(tbb::flow::graph &graph, Body body,
                   const std::string &name = "")
        : Base(graph), parameters(graph), multiplexer(graph),
          processingFunction(graph, tbb::flow::serial,
                             ProcessingBody<Body>(body, *this)),
          name_(name)
    {
        static_assert(
            std::is_same<typename std::result_of<Body(Input, Parameters)>::type,
                         Output>::value,
            "Processing node type and body definition missmatch");

        tbb::flow::make_edge(multiplexer, processingFunction);

        typename Base::input_ports_type input_tuple(
            tbb::flow::input_port<MULTIPLEXER_INPUT_PORT>(multiplexer),
            parameters,
            tbb::flow::input_port<MULTIPLEXER_ERROR_PORT>(multiplexer));
        typename Base::output_ports_type output_tuple(
            tbb::flow::output_port<PROCESSING_FUNCTION_OUTPUT_PORT>(
                processingFunction),
            parameters, tbb::flow::output_port<PROCESSING_FUNCTION_ERROR_PORT>(
                            processingFunction));
        Base::set_external_ports(input_tuple, output_tuple);
    }
};

} // namespace ProcessingFlow

#endif // _PROCESSING_FLOW_PROCESSING_NODE_H_
