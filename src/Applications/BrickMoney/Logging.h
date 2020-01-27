#ifndef LOGGING_H
#define LOGGING_H

#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/helpers/timehelper.h>

#define SET_LOGGER(X) static log4cplus::Logger g_logger(log4cplus::Logger::getInstance(LOG4CPLUS_TEXT(X)));

#define LOG_TRACE(stream) LOG4CPLUS_TRACE( g_logger, stream )
#define LOG_DEBUG(stream) LOG4CPLUS_DEBUG( g_logger, stream )
#define LOG_INFO(stream)  LOG4CPLUS_INFO ( g_logger, stream )
#define LOG_WARN(stream)  LOG4CPLUS_WARN ( g_logger, stream )
#define LOG_ERROR(stream) LOG4CPLUS_ERROR( g_logger, stream )
#define LOG_FATAL(stream) LOG4CPLUS_FATAL( g_logger, stream )


#endif // LOGGING_H
