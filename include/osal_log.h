#ifndef OSAL_LOG_H
#define OSAL_LOG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

// Estados de log
#define LOG_STATE_ON  1
#define LOG_STATE_OFF 0

// Níveis de log
#define LOG_LEVEL_DEBUG 0
#define LOG_LEVEL_INFO  1
#define LOG_LEVEL_WARN  2
#define LOG_LEVEL_ERROR 3

// Ativação condicional (simplificada)
#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_LEVEL_INFO
#endif

#define LOG_DEBUG_ENABLED(X) (X)
#define LOG_INFO_ENABLED(X) (X)

#define LOG_DEBUG(X, fmt, ...) do { if (LOG_DEBUG_ENABLED(X)) printf(fmt, ##__VA_ARGS__); } while (0)
#define LOG_INFO(X, fmt, ...)  do { if (LOG_DEBUG_ENABLED(X)) printf(fmt, ##__VA_ARGS__); } while (0)
#define LOG_WARNING(X, fmt, ...) do { if (LOG_DEBUG_ENABLED(X)) printf(fmt, ##__VA_ARGS__); } while (0)
#define LOG_FATAL(X, fmt, ...) do { if (LOG_DEBUG_ENABLED(X)) printf(fmt, ##__VA_ARGS__); } while (0)
#define LOG_ERROR(X, fmt, ...) do { if (LOG_DEBUG_ENABLED(X)) printf(fmt, ##__VA_ARGS__); } while (0)

#ifdef __cplusplus
}
#endif

#endif // OSAL_LOG_H
