/**
 * This file configures the FreeRTOS Package Library
 *  In addtion to these configuration also configure
 *  FreeRTOSConfig.h.
 */

#ifndef CONFIG_R_FREERTOS_H
#define CONFIG_R_FREERTOS_H

/* This macro is parsed by the make file choices (RX600, H8S_H8SX) */
#define RTOS_PORT RX200

#define port_mS_to_TICK(time) ((portTickType) (((time) * (long)configTICK_RATE_HZ)/1000L))
#define port_TICK_to_mS(tick) ((portTickType) (((tick) * 1000L)/configTICK_RATE_HZ))

#endif
