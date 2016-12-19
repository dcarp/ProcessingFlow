#ifndef _PROCESSING_FLOW_ERROR_H_
#define _PROCESSING_FLOW_ERROR_H_

#include "error_generated.h"

namespace ProcessingFlow
{

using Error = FlatBuffers::ErrorT;

#define MAKE_ERROR(message) makeError(__FILE__, __LINE__, __FUNCTION__, message)

Error makeError(const std::string &file, unsigned line,
                const std::string &function, const std::string &message)
{
    Error error = {};
    error.file = file;
    error.line = line;
    error.function = function;
    error.message = message;

    return error;
}

namespace FlatBuffers
{

bool operator==(const ErrorT &firstError, const ErrorT &secondError)
{
    return firstError.message == secondError.message;
}

} // namespace FlatBuffers

} // namespace ProcessingFlow

#endif // #_PROCESSING_FLOW_ERROR_H_
