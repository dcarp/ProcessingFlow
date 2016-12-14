#ifndef _PROCESSING_FLOW_RESULT_H_
#define _PROCESSING_FLOW_RESULT_H_

#include <cassert>
#include <exception>
#include <utility>

template <typename T> class Result
{
    typedef T ValueType;
    Result(const T &value) : value_(value), exception_(nullptr) {}
    Result(T &&value) : value_(std::move(value)), exception_(nullptr) {}
    Result(const std::exception *exception)
    {
        assert(exception != nullptr);
        exception_ = exception;
    }

    const T &value() const { return value_; }
    T &value() { return value_; }
    const std::exception *exception() { return exception_; }

private:
    T value_;
    const std::exception *exception_;
};

#endif // _PROCESSING_FLOW_RESULT_H_
