#ifndef __MINU_LOG_H_
#define __MINU_LOG_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef NO_ESP_IDF
#define MINU_LOGI(tag, format, ...)
#else
#include "esp_log.h"
#define MINU_LOGI(tag, format, ...) ESP_LOGI(tag, format, __VA_ARGS__)
#endif

#ifdef __cplusplus
}
#endif

#endif /* __MINU_OPS_H_ */
