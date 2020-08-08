#ifndef LOGGING_H
#define LOGGING_H

#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/helpers/timehelper.h>
#include <typeinfo>

/**
 *  Needed to create different variable names for the temporary scoped loggers created by LOG_SCOPE.
 *  Otherwise calling LOG_SCOPE more than once within the same scope would instantiate the same object name.
 *  Indirection via two macros is necessary due to the way that expansion is performed
 */
#define CONCAT_DIRECT(x, y) x##y
#define CONCAT(x, y) CONCAT_DIRECT(x, y)
#ifdef _MSC_VER // Necessary for edit & continue in MS Visual C++.
# define NUMBERED_NAME(str) CONCAT(str, __COUNTER__)
#else // gcc >4.3 has counter, too, but ppc is too old
# define NUMBERED_NAME(str) CONCAT(str, __LINE__)
#endif


/// @macro METHOD_SIGNATURE for different compilers
#if defined(__GNUC__)
#define METHOD_SIGNATURE __PRETTY_FUNCTION__
#elif defined(_MSC_VER)
// #define METHOD_SIGNATURE __FUNCSIG__
#define METHOD_SIGNATURE __FUNCTIONW__
#else
#define METHOD_SIGNATURE __func__   // was added to C++ in C++11
#endif


#define SET_LOGGER(X) static log4cplus::Logger g_logger(log4cplus::Logger::getInstance(LOG4CPLUS_TEXT(X)));



#define LOG_TRACE(stream) LOG4CPLUS_TRACE( g_logger, stream )
#define LOG_DEBUG(stream) LOG4CPLUS_DEBUG( g_logger, stream )
#define LOG_INFO(stream)  LOG4CPLUS_INFO ( g_logger, stream )
#define LOG_WARN(stream)  LOG4CPLUS_WARN ( g_logger, stream )
#define LOG_ERROR(stream) LOG4CPLUS_ERROR( g_logger, stream )
#define LOG_FATAL(stream) LOG4CPLUS_FATAL( g_logger, stream )

// Usage: e.g. LOG_TRACE_METHOD("Some text " << VAR_TO_STREAM(someVar) << " trailing text");
#define VAR_TO_STREAM(var)      #var << " = " << var

//#define LOG_SCOPE_METHOD(stream) LOG4CPLUS_TRACE(g_logger,  "--> [" << METHOD_SIGNATURE << "] " << stream); \

#define LOG_SCOPE_METHOD(stream) LOG4CPLUS_TRACE(g_logger,  "--> [" << METHOD_SIGNATURE << "] " << stream); \
    ScopedLogger NUMBERED_NAME(localscopedlogger) (g_logger, stream, METHOD_SIGNATURE)

//#define LOG_SCOPE_METHOD(stream) LOG4CPLUS_TRACE( g_logger, stream )


/** ScopedLogger:
 * When instantiated via LOG_SCOPE("Message") will print both enter and an automatic exit message
 * when destructor is called.
 *
 * @note By default, TRACE and DEBUG is only active in debug builds, so nothing will be logged in release mode!
 *
 *Similar to using log4cplus TraceLogger class via:
 *   log4cplus::TraceLogger tl(_SET_LOGGER_macro_logger(),"TraceLogger", __FILE__, __LINE__);
 * But our ScopedLogger output can be disabled via LOG4CPLUS_DISABLE_TRACE macro
 * Extra variable logging can be performed afterwards with LOG_SCOPE_VAR(var) for simple types.
 *
 * see "Simple debug log for C++" on CodeProject [http://www.codeproject.com/Articles/63736/Simple-debug-log-for-C] for further ideas and & extensions.
 */
class  ScopedLogger
{
public:
    ScopedLogger(const log4cplus::Logger &l, const std::wstring &msg, const std::wstring &fct)
        : _logger(l)
        , _msg(msg)
        , _fct(fct)
        , _timeStart(std::chrono::system_clock::now())
    {
        /// Logging call is directly in MACRO so calling file/line location is correct!
        // LOG4CPLUS_TRACE(_logger.getName(), "--> [" << fct << "] " << LOG4CPLUS_TEXT(_msg) );
    }

    ~ScopedLogger()
    {
        auto const millisecondsDiff = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - _timeStart);
        auto const milliseconds = millisecondsDiff.count();
        LOG4CPLUS_TRACE(_logger, "<-- [" << _fct << "] " << _msg << (_msg.size() > 0 ? " " : "") << " [after " << milliseconds << " ms] ");
    }

    // ScopedLogger is instantiated with an automatically generated name, further public functions are hence useless
private:
    ScopedLogger(ScopedLogger const&);
    ScopedLogger & operator = (ScopedLogger const&);

    log4cplus::Logger _logger;
    std::wstring _msg;
    std::wstring _fct;
    std::chrono::time_point<std::chrono::system_clock> _timeStart;
};


#endif // LOGGING_H
