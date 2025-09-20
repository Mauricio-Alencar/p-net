#ifndef PNAL_SYS_H
#define PNAL_SYS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

// Configuração dependente de SO/hardware que a pilha p-net passa para PNAL
// No ESP-IDF, podemos não precisar de nada específico por enquanto.
typedef struct pnal_cfg
{
   uint32_t dummy; // placeholder
   struct {
      uint32_t prio;
      uint32_t stack_size;
   } bg_worker_thread;
} pnal_cfg_t;

#ifdef __cplusplus
}
#endif

#endif // PNAL_SYS_H 