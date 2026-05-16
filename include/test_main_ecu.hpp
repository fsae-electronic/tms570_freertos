#ifndef TEST_MAIN_ECU_HPP
#define TEST_MAIN_ECU_HPP

#include "can.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void test_main_ecu_init(void);
void test_main_ecu_periodic(void);

#ifdef __cplusplus
}
#endif

#endif