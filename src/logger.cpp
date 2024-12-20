#include "logger.h"


namespace TTT::Sensor {

std::unique_ptr<Ports::Logger> Logger::m_instance;

const Ports::Logger &Logger::instance()
{
    if(m_instance)
        return *m_instance;

    // In case no logger instance was set (yet), than return a dummy logger
    class NullLogger: public Ports::Logger {
        virtual void log(const std::string& msg) const {
        }
    };
    static NullLogger dummy;
    return dummy;
}

void Logger::log(const std::string& msg) const
{
    instance().log(msg);
}
    
namespace details {

LoggerWriter::LoggerWriter(LogLevel level)
{
    m_stream  << "[" << toString(level) << "]: ";
}

std::string LoggerWriter::toString(LogLevel level) {
    switch (level) {
        case LogLevel::ERROR:
            return "ERROR";
        case LogLevel::WARNING:
            return "WARNING";
        case LogLevel::INFO:
            return "INFO";
        case LogLevel::DEBUG:
            return "DEBUG";
    }

    // This should never happen, only here to get rid of compiler warning.
    return "";
}

LoggerWriter::~LoggerWriter() {
    Logger::instance().log(m_stream.str());
}


} // End namespace details

} // End namespace TTT::Sensor
