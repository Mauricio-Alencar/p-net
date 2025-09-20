#ifndef OSAL_H
#define OSAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdatomic.h>

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_timer.h"

// Compatibilidade de macros usadas pelo p-net
#ifndef CC_PACKED_BEGIN
#define CC_PACKED_BEGIN
#endif
#ifndef CC_PACKED_END
#define CC_PACKED_END
#endif
#ifndef CC_STATIC_ASSERT
#define CC_STATIC_ASSERT(cond) _Static_assert(cond, #cond)
#endif

// Mutex
typedef SemaphoreHandle_t os_mutex_t;

static inline os_mutex_t * os_mutex_create(void)
{
   SemaphoreHandle_t h = xSemaphoreCreateMutex();
   return (os_mutex_t *)h;
}

static inline void os_mutex_destroy(os_mutex_t * m)
{
   if (m) vSemaphoreDelete((SemaphoreHandle_t)m);
}

static inline void os_mutex_lock(os_mutex_t * m)
{
   if (m) xSemaphoreTake((SemaphoreHandle_t)m, portMAX_DELAY);
}

static inline void os_mutex_unlock(os_mutex_t * m)
{
   if (m) xSemaphoreGive((SemaphoreHandle_t)m);
}

// Eventos (bitmap simples)
typedef struct {
   EventGroupHandle_t eg;
} os_event_t;

static inline os_event_t * os_event_create(void)
{
   os_event_t *e = (os_event_t *)calloc(1, sizeof(os_event_t));
   if (!e) return NULL;
   e->eg = xEventGroupCreate();
   return e;
}

static inline void os_event_destroy(os_event_t *e)
{
   if (!e) return;
   vEventGroupDelete(e->eg);
   free(e);
}

static inline void os_event_set(os_event_t *e, uint32_t mask)
{
   xEventGroupSetBits(e->eg, (EventBits_t)mask);
}

static inline void os_event_clr(os_event_t *e, uint32_t mask)
{
   xEventGroupClearBits(e->eg, (EventBits_t)mask);
}

#define OS_WAIT_FOREVER 0xFFFFFFFFu

static inline void os_event_wait(os_event_t *e, uint32_t mask, uint32_t *flags, uint32_t timeout_ms)
{
   EventBits_t bits = xEventGroupWaitBits(e->eg, (EventBits_t)mask, pdFALSE, pdFALSE,
      (timeout_ms == OS_WAIT_FOREVER) ? portMAX_DELAY : pdMS_TO_TICKS(timeout_ms));
   if (flags) *flags = (uint32_t)bits;
}

// Tempo
static inline uint32_t os_get_current_time_us(void)
{
   return (uint32_t)esp_timer_get_time();
}

static inline void os_usleep(uint32_t usec)
{
   vTaskDelay(pdMS_TO_TICKS((usec + 999) / 1000));
}

// Memória
static inline void * os_malloc(size_t sz)
{
   return malloc(sz);
}

static inline void os_free(void *p)
{
   free(p);
}

// Funções de conversão de endianness
static inline uint16_t htons(uint16_t hostshort)
{
   return ((hostshort & 0xFF) << 8) | ((hostshort & 0xFF00) >> 8);
}

static inline uint16_t ntohs(uint16_t netshort)
{
   return htons(netshort);
}

static inline uint32_t htonl(uint32_t hostlong)
{
   return ((hostlong & 0xFF) << 24) | 
          ((hostlong & 0xFF00) << 8) | 
          ((hostlong & 0xFF0000) >> 8) | 
          ((hostlong & 0xFF000000) >> 24);
}

static inline uint32_t ntohl(uint32_t netlong)
{
   return htonl(netlong);
}

// Macro para obter o número de elementos em um array
#define NELEMENTS(arr) (sizeof(arr) / sizeof((arr)[0]))

// Macros de conversão de endianness
#define CC_TO_BE32(x) htonl(x)

// Thread functions
typedef void (*os_thread_function_t)(void * arg);

static inline void * os_thread_create(
   const char * p_name,
   uint32_t priority,
   uint32_t stack_size,
   os_thread_function_t func,
   void * arg)
{
   TaskHandle_t task_handle;
   BaseType_t result = xTaskCreate(func, p_name, stack_size / sizeof(StackType_t), arg, priority, &task_handle);
   return (result == pdPASS) ? task_handle : NULL;
}

// Asserts
#ifndef CC_ASSERT
#include <assert.h>
#define CC_ASSERT(x) assert(x)
#endif

#ifdef __cplusplus
}
#endif

#endif // OSAL_H
