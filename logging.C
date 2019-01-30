#include <logging.h>
#include <string.h>

FILE * Logger::logger = fopen("logger", "w");

DataFlowException::DataFlowException(const char *type, const char *error)
{
    sprintf(msg, "Throwing exception: (%s): %s", type, error);
    Logger::LogEvent(msg);
}

void 
Logger::LogEvent(const char *event)
{
    fwrite(event, sizeof(char), strlen(event), logger);
}

void
Logger::Finalize()
{
    fclose(logger);
}

