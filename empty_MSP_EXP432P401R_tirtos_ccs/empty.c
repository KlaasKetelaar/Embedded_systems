/*
 * Copyright (c) 2015-2019, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== empty.c ========
 */

/* For usleep() */
#include <unistd.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <pthread.h>
//  int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void*), void *arg);
    int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void*), void *arg);

/* Driver Header files */
#include <ti/drivers/GPIO.h>
// #include <ti/drivers/I2C.h>
// #include <ti/drivers/SPI.h>
// #include <ti/drivers/UART.h>
// #include <ti/drivers/Watchdog.h>

/* Driver configuration */
#include "ti_drivers_config.h"

/*
 *  ======== subThreads ========
 */
void *threadFxn1(void *arg0){
    /* 1 second delay */
    uint32_t time = 1;
    printf("start Thread 1\n");
    int i = 0;
    for(i = 0; i < 20; i++){
        printf("Toggle LED_1\n");
        GPIO_toggle(CONFIG_GPIO_LED_1);
        sleep(time);
    }
    return NULL;
}

void *threadFxn0(void *arg0){
    /* 1 second delay */
    uint32_t time = 1;
    printf("Start Thread 0\n");
    int i = 0;
    for(i = 0; i < 20; i++){
        printf("Toggle LED_0\n");
        GPIO_toggle(CONFIG_GPIO_LED_0);
        sleep(time);
    }
    return NULL;
}
/*
 *  ======== mainThread ========
 */
void *mainThread(void *arg0)
{
    /* Call driver init functions */
    GPIO_init();
    // I2C_init();
    // SPI_init();
    // UART_init();
    // Watchdog_init();

    //init thread vars
    pthread_t thread1;
    pthread_t thread0;
    pthread_attr_t attrs;
    pthread_attr_init (&attrs);
    pthread_attr_setstacksize(&attrs, 1024);

    //Create threads
    pthread_create(&thread0, &attrs, &threadFxn0, NULL);
    pthread_create(&thread1, &attrs, &threadFxn1, NULL);
    pthread_join(thread1,NULL); // why join thread 1 first?
    pthread_join(thread0,NULL);
    pthread_attr_destroy(&attrs);

    /* Configure the LED pin */
    GPIO_setConfig(CONFIG_GPIO_LED_0, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(CONFIG_GPIO_LED_1, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);

    /* Turn on user LED */
    GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_ON);
    GPIO_write(CONFIG_GPIO_LED_1, CONFIG_GPIO_LED_ON);
//
//    while (1) {
//        GPIO_toggle(CONFIG_GPIO_LED_1);
//        sleep(time);
//        GPIO_toggle(CONFIG_GPIO_LED_0);
//        sleep(time);
//    }

    return NULL;
}

