#ifndef _PROCESSING_FLOW_ERROR_H_
#define _PROCESSING_FLOW_ERROR_H_

#include "error_generated.h"

namespace ProcessingFlow
{

#define MAKE_ERROR(message) makeError(__FILE__, __LINE__, __FUNCTION__, message)

ErrorT makeError(const std::string &file, unsigned line,
                 const std::string &function, const std::string &message)
{
    ErrorT error;
    error.file = file;
    error.line = line;
    error.function = function;
    error.message = message;

    return error;
}

bool operator==(const ErrorT &firstError, const ErrorT &secondError)
{
    return firstError.message == secondError.message;
}

} // namespace ProcessingFlow

#endif // #_PROCESSING_FLOW_ERROR_H_