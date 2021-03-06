/****************************************************************************
 *
 *   Copyright (c) 2013-2017 PX4 Development Team. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name PX4 nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/**
 * @file board_config.h
 *
 * nxphlite-v3 internal definitions
 */

#pragma once

/****************************************************************************************************
 * Included Files
 ****************************************************************************************************/
#include <px4_config.h>
#include <nuttx/compiler.h>
#include <stdint.h>

__BEGIN_DECLS

/* these headers are not C++ safe */
#include <kinetis.h>
#include <chip/kinetis_pinmux.h>
#include <arch/board/board.h>

/* NXPHLITE GPIOs ***********************************************************************************/
/* LEDs */
/* An RGB LED is connected through GPIO as shown below:
 * TBD (no makring on schematic)
 *   LED    K66
 *   ------ -------------------------------------------------------
 *   RED    FB_CS0_b/ UART2_CTS_b / ADC0_SE5b / SPI0_SCK / FTM3_CH1/ PTD1
 *   GREEN  FTM2_FLT0/ CMP0_IN3/ FB_AD6 / I2S0_RX_BCLK/ FTM3_CH5/ ADC1_SE5b/ PTC9
 *   BLUE   CMP0_IN2/ FB_AD7 / I2S0_MCLK/ FTM3_CH4/ ADC1_SE4b/ PTC8
 */

#define GPIO_LED_R             (GPIO_HIGHDRIVE | GPIO_OUTPUT_ONE | PIN_PORTD | PIN1)
#define GPIO_LED_G             (GPIO_HIGHDRIVE | GPIO_OUTPUT_ONE | PIN_PORTC | PIN9)
#define GPIO_LED_B             (GPIO_HIGHDRIVE | GPIO_OUTPUT_ONE | PIN_PORTC | PIN8)


#define GPIO_LED_1             (GPIO_HIGHDRIVE | GPIO_OUTPUT_ZER0 | PIN_PORTD | PIN13)
#define GPIO_LED_2             (GPIO_HIGHDRIVE | GPIO_OUTPUT_ZER0 | PIN_PORTD | PIN14)

#define BOARD_HAS_CONTROL_STATUS_LEDS 1 // Use D9 and D10

#define GPIO_NFC_IO            (GPIO_HIGHDRIVE | GPIO_OUTPUT_ONE  | PIN_PORTA | PIN26)
#define GPIO_SENSOR_P_EN       (GPIO_HIGHDRIVE | GPIO_OUTPUT_ONE  | PIN_PORTB | PIN8)


/* UART tty Mapping
 * Device   tty        alt           Connector Name
 * ------- ---------- -------------- --------- -------------------------
 * LPUART0 /dev/tty0  /dev/console    P16      DCD-Mini
 * UART0   /dev/tty1      ---         P7       IR transmitter & receiver
 * UART1   /dev/tty2      ---         P14,P15  SERIAL4/FrSky, RC_IN
 * UART2   /dev/tty3      ---         P3       GPS connector
 * UART4   /dev/tty4      ---         P10      UART (Bluetooth)
 */

#define GPS_DEFAULT_UART_PORT  "/dev/ttyS3"      /* UART2 */

/* High-resolution timer */
#define HRT_TIMER              1  /* TPM1 timer for the HRT */
#define HRT_TIMER_CHANNEL      0  /* Use capture/compare channel 0 */

/* PPM IN
 */

#define HRT_PPM_CHANNEL        1  /* Use TPM1 capture/compare channel 1 */
#define GPIO_PPM_IN            PIN_TPM1_CH1_1    /* PTC3 USART1 RX and PTA9 and PIN_TPM1_CH1 AKA FrSky_IN_RC_IN */


/*
 *
 * NXPhlite-v3 has separate RC_IN
 *
 * GPIO PPM_IN on PTA9 PIN_TPM1_CH1 and PCT3 USART1 RX
 * SPEKTRUM_RX (it's TX or RX in Bind) on PCT3 USART1 RX
 * Inversion is possible the UART
 * The FMU can drive GPIO PPM_IN as an output
 */

/* Spektrum controls ******************************************************/

/* Power is a p-Channel FET */

#define GPIO_SPEKTRUM_P_EN          (GPIO_HIGHDRIVE | GPIO_OUTPUT_ONE | PIN_PORTA | PIN7)

/* For binding the Spektrum 3-pin interfaces is used with it TX (output)
 * as an input Therefore we drive are UARTx_RX (normaly an input) as an
 * output
 */

#define GPIO_PPM_IN_AS_OUT          (GPIO_HIGHDRIVE | GPIO_OUTPUT_ONE | PIN_PORTC | PIN3)

#define SPEKTRUM_RX_AS_GPIO_OUTPUT() px4_arch_configgpio(GPIO_PPM_IN_AS_OUT)
#define SPEKTRUM_RX_AS_UART()        px4_arch_configgpio(PIN_UART1_RX)
#define SPEKTRUM_OUT(_one_true)      px4_arch_gpiowrite(GPIO_PPM_IN_AS_OUT, (_one_true))

/* RC input */

#define RC_UXART_BASE           KINETIS_UART1_BASE
#define RC_SERIAL_PORT          "/dev/ttyS2"      /* UART1 */
#define INVERT_RC_INPUT(_s)     board_rc_input(_s);

#define GPIO_RSSI_IN            PIN_ADC1_SE13

/* Ethernet Control
 *
 * Uninitialized to Reset Disabled and Inhibited
 * All pins driven low to not back feed when power is off
 */

#define nGPIO_ETHERNET_P_EN     (GPIO_LOWDRIVE | GPIO_OUTPUT_ONE  | PIN_PORTB | PIN3)
#define GPIO_ENET_RST           (GPIO_LOWDRIVE | GPIO_OUTPUT_ZER0 | PIN_PORTA | PIN28)
#define GPIO_ENET_EN            (GPIO_LOWDRIVE | GPIO_OUTPUT_ZER0 | PIN_PORTA | PIN29)
#define GPIO_ENET_INH           (GPIO_LOWDRIVE | GPIO_OUTPUT_ZER0 | PIN_PORTA | PIN8)
#define GPIO_ENET_CONFIG0       (GPIO_LOWDRIVE | GPIO_OUTPUT_ZER0 | PIN_PORTA | PIN24)
#define GPIO_ENET_CONFIG1       (GPIO_LOWDRIVE | GPIO_OUTPUT_ZER0 | PIN_PORTB | PIN2)

/* CAN Control
 * Control pin S allows two operating modes to be selected:
 * high-speed mode (Low) or silent mode (high)
 */

#define GPIO_CAN0_STB           (GPIO_LOWDRIVE | GPIO_OUTPUT_ONE  | PIN_PORTC | PIN19)
#define GPIO_CAN1_STB           (GPIO_LOWDRIVE | GPIO_OUTPUT_ONE  | PIN_PORTC | PIN18)


/* Safety Switch
 * TBD
 */
#define GPIO_LED_SAFETY         (GPIO_HIGHDRIVE | GPIO_OUTPUT_ZER0  | PIN_PORTC | PIN0)
#define GPIO_BTN_SAFETY         (GPIO_PULLUP | PIN_PORTE | PIN28)

/* NXPHlite-v3 GPIOs ****************************************************************/

/*	SDHC
 *
 * A micro Secure Digital (SD) card slot is available on the board connected to
 * the SD Host Controller (SDHC) signals of the MCU. This slot will accept micro
 * format SD memory cards. The SD card detect pin (PTE6) is an open switch that
 * shorts with VDD when card is inserted.
 *
 *   ------------ ------------- --------
 *    SD Card Slot Board Signal  K66 Pin
 *    ------------ ------------- --------
 *    DAT0         SDHC0_D0      PTE1
 *    DAT1         SDHC0_D1      PTE0
 *    DAT2         SDHC0_D2      PTE5
 *    CD/DAT3      SDHC0_D3      PTE4
 *    CMD          SDHC0_CMD     PTE3
 *    CLK          SDHC0_DCLK    PTE2
 *    SWITCH       D_CARD_DETECT PTD10
 *                 CAED_P_EN     PTD6
 *    ------------ ------------- --------
 *
 * There is no Write Protect pin available to the K66
 */
#define SD_CAED_P_EN       (GPIO_LOWDRIVE | GPIO_OUTPUT_ONE  | PIN_PORTD | PIN6)

#define GPIO_SD_CARDDETECT (GPIO_PULLUP | PIN_INT_BOTH | PIN_PORTD | PIN10)

/* SPI
 *
 *  SD Card is on SPI 0
 *	FXOS8700CQ Accelerometer & Magnetometer is on SPI 1
 *	FXAS21002CQ Gyro is on SPI 2
 */

/* SPI Bus assignments */

#define PX4_SPI_BUS_MEMORY                  PX4_BUS_NUMBER_TO_PX4(0)
#define PX4_SPI_BUS_SENSORS                 PX4_BUS_NUMBER_TO_PX4(1)
#define PX4_SPI_BUS_EXTERNAL                PX4_BUS_NUMBER_TO_PX4(2)
#define PX4_SPI_BUS_RAMTRON                 PX4_SPI_BUS_MEMORY
#define PX4_SPI_BUS_EXT                     PX4_SPI_BUS_EXTERNAL


/* SPI chip selects */

#define GPIO_SPI_CS_MEMORY                  (GPIO_LOWDRIVE | GPIO_OUTPUT_ONE  | PIN_PORTC | PIN2)
#define GPIO_SPI_CS_FXAS21002CQ_GYRO        (GPIO_LOWDRIVE | GPIO_OUTPUT_ONE  | PIN_PORTB | PIN9)
#define GPIO_SPI_CS_FXOS8700CQ_ACCEL_MAG    (GPIO_LOWDRIVE | GPIO_OUTPUT_ONE  | PIN_PORTB | PIN10)
#define GPIO_SPI2_CS                        (GPIO_LOWDRIVE | GPIO_OUTPUT_ONE  | PIN_PORTB | PIN20)
#define GPIO_SPI2_EXT                       (GPIO_LOWDRIVE | GPIO_OUTPUT_ONE  | PIN_PORTD | PIN15)

/* SPI device reset signals
 * In Active state
 */

/* Init in reset */
#define GPIO_GM_nRST                        (GPIO_LOWDRIVE | GPIO_OUTPUT_ZER0  | PIN_PORTD | PIN12)
#define GPIO_A_RST                          (GPIO_LOWDRIVE | GPIO_OUTPUT_ONE   | PIN_PORTA | PIN25)

/* Sensor interrupts */

#define GPIO_EXTI_GYRO_INT1                 (GPIO_PULLUP | PIN_INT_BOTH | PIN_PORTE | PIN7)
#define GPIO_EXTI_GYRO_INT2                 (GPIO_PULLUP | PIN_INT_BOTH | PIN_PORTE | PIN6)
#define GPIO_EXTI_ACCEL_MAG_INT1            (GPIO_PULLUP | PIN_INT_BOTH | PIN_PORTE | PIN9)
#define GPIO_EXTI_ACCEL_MAG_INT2            (GPIO_PULLUP | PIN_INT_BOTH | PIN_PORTE | PIN10)
#define GPIO_EXTI_BARO_INT1                 (GPIO_PULLUP | PIN_INT_BOTH | PIN_PORTD | PIN11)
#define GPIO_EXTI_BARO_INT2                 (GPIO_PULLUP | PIN_INT_BOTH | PIN_PORTD | PIN7)

/* Use these in place of the uint32_t enumeration to select a specific SPI device on SPI1 */

#define PX4_SPIDEV_MEMORY                   PX4_MK_SPI_SEL(PX4_SPI_BUS_MEMORY,0)
#define PX4_MEMORY_BUS_CS_GPIO              {GPIO_SPI_CS_MEMORY}
#define PX4_MEMORY_BUS_FIRST_CS             PX4_SPIDEV_MEMORY
#define PX4_MEMORY_BUS_LAST_CS              PX4_SPIDEV_MEMORY

#define PX4_SPIDEV_ACCEL_MAG                PX4_MK_SPI_SEL(PX4_SPI_BUS_SENSORS,0)
#define PX4_SPIDEV_GYRO                     PX4_MK_SPI_SEL(PX4_SPI_BUS_SENSORS,1)
#define PX4_SENSOR_BUS_CS_GPIO              {GPIO_SPI_CS_FXOS8700CQ_ACCEL_MAG, GPIO_SPI_CS_FXAS21002CQ_GYRO}
#define PX4_SENSOR_BUS_FIRST_CS             PX4_SPIDEV_ACCEL_MAG
#define PX4_SENSOR_BUS_LAST_CS              PX4_SPIDEV_GYRO

#define PX4_SPIDEV_EXTERNAL1                PX4_MK_SPI_SEL(PX4_SPI_BUS_EXTERNAL,0)
#define PX4_SPIDEV_EXTERNAL2                PX4_MK_SPI_SEL(PX4_SPI_BUS_EXTERNAL,1)
#define PX4_EXTERNAL_BUS_CS_GPIO            {GPIO_SPI2_CS, GPIO_SPI2_EXT}
#define PX4_EXTERNAL_BUS_FIRST_CS           PX4_SPIDEV_EXTERNAL1
#define PX4_EXTERNAL_BUS_LAST_CS            PX4_SPIDEV_EXTERNAL2

#define PX4_SPIDEV_ICM_20602                PX4_SPIDEV_EXTERNAL1
#define PX4_SPIDEV_ICM_20608                PX4_SPIDEV_EXTERNAL1
#define PX4_SPIDEV_ICM_20689                PX4_SPIDEV_EXTERNAL1
#define PX4_SPIDEV_EXT_MPU                  PX4_SPIDEV_EXTERNAL1
#define PX4_SPIDEV_MPU                      PX4_SPIDEV_EXTERNAL1

/* I2C busses */

#define PX4_I2C_BUS_EXPANSION               PX4_BUS_NUMBER_TO_PX4(0)
#define PX4_I2C_BUS_ONBOARD                 PX4_BUS_NUMBER_TO_PX4(1)

#define PX4_I2C_BUS_LED                     PX4_I2C_BUS_EXPANSION

#define PX4_I2C_OBDEV_LED                   0x55
#define PX4_I2C_OBDEV_HMC5883               0x1e

/*
 * ADC channels
 *
 * These are the channel numbers of the ADCs of the microcontroller that can be used by the Px4
 * Firmware in the adc driver. ADC1 has 32 channels, with some a/b selection overlap
 * in the AD4-AD7 range on the same ADC.
 *
 * Only ADC1 is used
 *         Bits 31:0 are ADC1 channels 31:0
 */

#define ADC1_CH(c)    (((c) & 0x1f))	/* Define ADC number Channel number */
#define ADC1_GPIO(n)  PIN_ADC1_SE##n

/* ADC defines to be used in sensors.cpp to read from a particular channel */

#define ADC_USB_VBUS_VALID          ADC1_CH(0)      /* USB_VBUS_VALID   29    -    ADC1_DP0  */
#define ADC_BATTERY_VOLTAGE_CHANNEL ADC1_CH(10)     /* BAT_VSENS        85   PTB4  ADC1_SE10 */
#define ADC_BATTERY_CURRENT_CHANNEL ADC1_CH(11)     /* BAT_ISENS        86   PTB5  ADC1_SE11 */
#define ADC_5V_RAIL_SENSE           ADC1_CH(12)     /* 5V_VSENS         87   PTB6  ADC1_SE12 */
#define ADC_RSSI_IN                 ADC1_CH(13)     /* RSSI_IN          88   PTB7  ADC1_SE13 */
#define ADC_AD1                     ADC1_CH(16)     /* AD1              35    -    ADC1_SE16 */
#define ADC_AD2                     ADC1_CH(18)     /* AD2              37    -    ADC1_SE18 */
#define ADC_AD3                     ADC1_CH(23)     /* AD3              39    -    ADC1_SE23 */

/* Mask use to initialize the ADC driver */

#define ADC_CHANNELS ((1 << ADC_USB_VBUS_VALID) | \
		      (1 << ADC_BATTERY_VOLTAGE_CHANNEL) | \
		      (1 << ADC_BATTERY_CURRENT_CHANNEL) | \
		      (1 << ADC_5V_RAIL_SENSE) | \
		      (1 << ADC_RSSI_IN) | \
		      (1 << ADC_AD1) | \
		      (1 << ADC_AD2) | \
		      (1 << ADC_AD3))


/* GPIO that require Configuration */

#define PX4_ADC_GPIO  \
	/* PTB4  ADC1_SE10 */  ADC1_GPIO(10),  \
	/* PTB5  ADC1_SE11 */  ADC1_GPIO(11),  \
	/* PTB6  ADC1_SE12 */  ADC1_GPIO(12),  \
	/* PTB7  ADC1_SE13 */  ADC1_GPIO(13)



#define BOARD_BATTERY1_V_DIV   (10.177939394f)
#define BOARD_BATTERY1_A_PER_V (15.391030303f)


/* User GPIOs
 *
 * GPIO-
 * Define as GPIO input / GPIO outputs and timers IO
 */

#define PX4_MK_GPIO(pin_ftmx, io)    ((((uint32_t)(pin_ftmx)) & ~(_PIN_MODE_MASK | _PIN_OPTIONS_MASK)) |(io))
#define PX4_MK_GPIO_INPUT(pin_ftmx)    PX4_MK_GPIO(pin_ftmx, GPIO_PULLUP)
#define PX4_MK_GPIO_OUTPUT(pin_ftmx)   PX4_MK_GPIO(pin_ftmx, GPIO_HIGHDRIVE)

#define GPIO_GPIO0_INPUT    PX4_MK_GPIO_INPUT(GPIO_FTM0_CH0OUT) /* FMU_CH1 */
#define GPIO_GPIO1_INPUT    PX4_MK_GPIO_INPUT(GPIO_FTM0_CH3OUT) /* FMU_CH2 */
#define GPIO_GPIO2_INPUT    PX4_MK_GPIO_INPUT(GPIO_FTM0_CH4OUT) /* FMU_CH3 */
#define GPIO_GPIO3_INPUT    PX4_MK_GPIO_INPUT(GPIO_FTM0_CH5OUT) /* FMU_CH4 */
#define GPIO_GPIO4_INPUT    PX4_MK_GPIO_INPUT(GPIO_FTM3_CH6OUT) /* FMU_CH5 */
#define GPIO_GPIO5_INPUT    PX4_MK_GPIO_INPUT(GPIO_FTM3_CH7OUT) /* FMU_CH6 */

#define GPIO_GPIO6_INPUT    PX4_MK_GPIO_INPUT(GPIO_FTM3_CH0OUT) /* U_TRI   */
#define GPIO_GPIO7_INPUT    PX4_MK_GPIO_INPUT(GPIO_FTM2_CH0OUT) /* U_ECHO  */

#define GPIO_GPIO0_OUTPUT   PX4_MK_GPIO_OUTPUT(GPIO_FTM0_CH0OUT) /* FMU_CH1 */
#define GPIO_GPIO1_OUTPUT   PX4_MK_GPIO_OUTPUT(GPIO_FTM0_CH3OUT) /* FMU_CH2 */
#define GPIO_GPIO2_OUTPUT   PX4_MK_GPIO_OUTPUT(GPIO_FTM0_CH4OUT) /* FMU_CH3 */
#define GPIO_GPIO3_OUTPUT   PX4_MK_GPIO_OUTPUT(GPIO_FTM0_CH5OUT) /* FMU_CH4 */
#define GPIO_GPIO4_OUTPUT   PX4_MK_GPIO_OUTPUT(GPIO_FTM3_CH6OUT) /* FMU_CH5 */
#define GPIO_GPIO5_OUTPUT   PX4_MK_GPIO_OUTPUT(GPIO_FTM3_CH7OUT) /* FMU_CH6 */

#define GPIO_GPIO6_OUTPUT   PX4_MK_GPIO_OUTPUT(GPIO_FTM3_CH0OUT) /* U_TRI   */
#define GPIO_GPIO7_OUTPUT   PX4_MK_GPIO_OUTPUT(GPIO_FTM2_CH0OUT) /* U_ECHO  */

/* P13 Ultrasonic Sensors
 * Timer Capture driver the the Trigger and sample the response on Echo
 */

#define GPIO_TRI                 GPIO_GPIO6_OUTPUT
#define GPIO_ECH                 GPIO_GPIO7_INPUT

/* Timer I/O PWM and capture
 *
 * 14 PWM outputs are configured.
 * 14 Timer inputs are configured.
 *
 * Pins:
 *      Defined in board.h
 */
// todo:Design this!

#define DIRECT_PWM_OUTPUT_CHANNELS  8
#define DIRECT_INPUT_TIMER_CHANNELS 8

/* Power supply control and monitoring GPIOs */
// None

#define GPIO_PERIPH_3V3_EN  0


/* Tone alarm output PTA11 - TMP 2_CH1 is On +P12-4, -P12-5
 * It is driving a NPN
 */
#define TONE_ALARM_TIMER     2   /* timer */
#define TONE_ALARM_CHANNEL   1   /* channel  */
#define GPIO_TONE_ALARM_IDLE (GPIO_LOWDRIVE | GPIO_OUTPUT_ZER0 | PIN_PORTA | PIN11)
#define GPIO_TONE_ALARM      (PIN_TPM2_CH1_1)

/* USB
 *
 *  VBUS detection is on 29  ADC_DPM0 and PTE8
 */
#define GPIO_USB_VBUS_VALID         /* PTE8 */ (GPIO_PULLUP | PIN_PORTE | PIN8)

/* PWM input driver. Use FMU PWM14 pin
 * todo:desing this
 */
#define PWMIN_TIMER		0
#define PWMIN_TIMER_CHANNEL	2
#define GPIO_PWM_IN		GPIO_FTM0_CH2IN

#define BOARD_NAME "NXPHLITE_V3"


/* Define True logic Power Control in arch agnostic form */

#define VDD_ETH_EN(on_true)                px4_arch_gpiowrite(nGPIO_ETHERNET_P_EN, !(on_true))
// Do not have #define VDD_5V_PERIPH_EN(on_true)          px4_arch_gpiowrite(GPIO_nVDD_5V_PERIPH_EN, !(on_true))
// Do not have #define VDD_5V_HIPOWER_EN(on_true)         px4_arch_gpiowrite(GPIO_nVDD_5V_HIPOWER_EN, !(on_true))
#define VDD_3V3_SENSORS_EN(on_true)        px4_arch_gpiowrite(GPIO_SENSOR_P_EN, !(on_true))
#define VDD_3V3_SPEKTRUM_POWER_EN(on_true) px4_arch_gpiowrite(GPIO_SPEKTRUM_P_EN, !(on_true))
#define READ_VDD_3V3_SPEKTRUM_POWER_EN()   px4_arch_gpioread(GPIO_SPEKTRUM_P_EN)
// Do not have #define VDD_5V_RC_EN(on_true)              px4_arch_gpiowrite(GPIO_VDD_5V_RC_EN, (on_true))
// Do not have #define VDD_5V_WIFI_EN(on_true)            px4_arch_gpiowrite(GPIO_VDD_5V_WIFI_EN, (on_true))
#define VDD_3V3_SD_CARD_EN(on_true)        px4_arch_gpiowrite(SD_CAED_P_EN, !(on_true))

/* Map to control term used in RC lib */
#define SPEKTRUM_POWER(on_true) VDD_3V3_SPEKTRUM_POWER_EN((on_true))


/*
 * By Providing BOARD_ADC_USB_CONNECTED (using the px4_arch abstraction)
 * this board support the ADC system_power interface, and therefore
 * provides the true logic GPIO BOARD_ADC_xxxx macros.
 */

#define BOARD_ADC_USB_CONNECTED (px4_arch_gpioread(GPIO_USB_VBUS_VALID))
#define BOARD_ADC_BRICK_VALID   (1)
#define BOARD_ADC_SERVO_VALID   (1)
#define BOARD_ADC_PERIPH_5V_OC  (0)
#define BOARD_ADC_HIPOWER_5V_OC (0)

#define BOARD_HAS_PWM	DIRECT_PWM_OUTPUT_CHANNELS

#define BOARD_FMU_GPIO_TAB { \
		{GPIO_GPIO0_INPUT,       GPIO_GPIO0_OUTPUT,       0}, \
		{GPIO_GPIO1_INPUT,       GPIO_GPIO1_OUTPUT,       0}, \
		{GPIO_GPIO2_INPUT,       GPIO_GPIO2_OUTPUT,       0}, \
		{GPIO_GPIO3_INPUT,       GPIO_GPIO3_OUTPUT,       0}, \
		{GPIO_GPIO4_INPUT,       GPIO_GPIO4_OUTPUT,       0}, \
		{GPIO_GPIO5_INPUT,       GPIO_GPIO5_OUTPUT,       0}, \
		{0,                      GPIO_TRI,                0}, \
		{GPIO_ECH,               0,                       0}, \
	}

#define BOARD_HAS_LED_PWM              1

#define LED_TIM3_CH1OUT   /* PTD1  RGB_R */ PIN_FTM3_CH1_1
#define LED_TIM3_CH5OUT   /* PTC9  RGB_G */ PIN_FTM3_CH5_1
#define LED_TIM3_CH4OUT   /* PTC8  RGB_B */ PIN_FTM3_CH4_1


/*
 * GPIO numbers.
 *
 * There are no alternate functions on this board.
 */
#define GPIO_SERVO_1          (1<<0)  /**< servo 1 output */
#define GPIO_SERVO_2          (1<<1)  /**< servo 2 output */
#define GPIO_SERVO_3          (1<<2)  /**< servo 3 output */
#define GPIO_SERVO_4          (1<<3)  /**< servo 4 output */
#define GPIO_SERVO_5          (1<<4)  /**< servo 5 output */
#define GPIO_SERVO_6          (1<<5)  /**< servo 6 output */
#define GPIO_SERVO_7          (1<<6)  /**< servo 7 output U_TRI */
#define GPIO_SERVO_8          (1<<6)  /**< servo 8 output U_ECH */

/* This board provides a DMA pool and APIs */

#define BOARD_DMA_ALLOC_POOL_SIZE 5120

/* This board provides the board_on_reset interface */

#define BOARD_HAS_ON_RESET 1

#define PX4_GPIO_PWM_INIT_LIST { \
		GPIO_GPIO5_INPUT, \
		GPIO_GPIO4_INPUT, \
		GPIO_GPIO3_INPUT, \
		GPIO_GPIO2_INPUT, \
		GPIO_GPIO1_INPUT, \
		GPIO_GPIO0_INPUT, \
		GPIO_TRI,         \
	}

#define PX4_GPIO_INIT_LIST {  \
		GPIO_LED_R,           \
		GPIO_LED_G,           \
		GPIO_LED_B,           \
		GPIO_SENSOR_P_EN,     \
		SD_CAED_P_EN,         \
		nGPIO_ETHERNET_P_EN,  \
		GPIO_SPEKTRUM_P_EN,   \
		PX4_ADC_GPIO,         \
		GPIO_GM_nRST,         \
		GPIO_A_RST,           \
		GPIO_USB_VBUS_VALID,  \
		GPIO_ECH,             \
		GPIO_ENET_RST,        \
		GPIO_ENET_EN,         \
		GPIO_ENET_INH,        \
		GPIO_ENET_CONFIG0,    \
		GPIO_ENET_CONFIG1,    \
		GPIO_CAN0_STB,        \
		GPIO_CAN1_STB,        \
		GPIO_BTN_SAFETY,      \
		GPIO_TONE_ALARM_IDLE, \
		GPIO_NFC_IO,          \
		GPIO_LED_1,           \
		GPIO_LED_2            \
	}

/* Automounter */

#define HAVE_MMCSD      1
#define HAVE_AUTOMOUNTER 1
#if !defined(CONFIG_FS_AUTOMOUNTER) || !defined(HAVE_MMCSD)
#  undef HAVE_AUTOMOUNTER
#  undef CONFIG_NXPHLITE_SDHC_AUTOMOUNT
#endif

#ifndef CONFIG_NXPHLITE_SDHC_AUTOMOUNT
#  undef HAVE_AUTOMOUNTER
#endif

/* Automounter defaults */

#ifdef HAVE_AUTOMOUNTER

#  ifndef CONFIG_NXPHLITE_SDHC_AUTOMOUNT_FSTYPE
#    define CONFIG_NXPHLITE_SDHC_AUTOMOUNT_FSTYPE "vfat"
#  endif

#  ifndef CONFIG_NXPHLITE_SDHC_AUTOMOUNT_BLKDEV
#    define CONFIG_NXPHLITE_SDHC_AUTOMOUNT_BLKDEV "/dev/mmcds0"
#  endif

#  ifndef CONFIG_NXPHLITE_SDHC_AUTOMOUNT_MOUNTPOINT
#    define CONFIG_NXPHLITE_SDHC_AUTOMOUNT_MOUNTPOINT "/mnt/sdcard"
#  endif

#  ifndef CONFIG_NXPHLITE_SDHC_AUTOMOUNT_DDELAY
#    define CONFIG_NXPHLITE_SDHC_AUTOMOUNT_DDELAY 1000
#  endif

#  ifndef CONFIG_NXPHLITE_SDHC_AUTOMOUNT_UDELAY
#    define CONFIG_NXPHLITE_SDHC_AUTOMOUNT_UDELAY 2000
#  endif
#endif /* HAVE_AUTOMOUNTER */


/************************************************************************************
 * Public data
 ************************************************************************************/

#ifndef __ASSEMBLY__

/************************************************************************************
 * Public Functions
 ************************************************************************************/

/************************************************************************************
 * Name: nxphlite_spidev_initialize
 *
 * Description:
 *   Called to configure SPI chip select GPIO pins for the NXPHlite-v3 board.
 *
 ************************************************************************************/

void nxphlite_spidev_initialize(void);

/************************************************************************************
 * Name: nxphlite_spi_bus_initialize
 *
 * Description:
 *   Called to configure SPI Buses.
 *
 ************************************************************************************/

int  nxphlite_spi_bus_initialize(void);

/****************************************************************************************************
 * Name: board_spi_reset board_peripheral_reset
 *
 * Description:
 *   Called to reset SPI and the perferal bus
 *
 ****************************************************************************************************/
void board_spi_reset(int ms);
void board_peripheral_reset(int ms);

/************************************************************************************
 * Name: nxphlite_bringup
 *
 * Description:
 *   Bring up board features
 *
 ************************************************************************************/

#if defined(CONFIG_LIB_BOARDCTL) || defined(CONFIG_BOARD_INITIALIZE)
int nxphlite_bringup(void);
#endif

/****************************************************************************
 * Name: nxphlite_sdhc_initialize
 *
 * Description:
 *   Inititialize the SDHC SD card slot
 *
 ****************************************************************************/

int nxphlite_sdhc_initialize(void);

/************************************************************************************
 * Name: nxphlite_cardinserted
 *
 * Description:
 *   Check if a card is inserted into the SDHC slot
 *
 ************************************************************************************/

#ifdef HAVE_AUTOMOUNTER
bool nxphlite_cardinserted(void);
#else
#  define nxphlite_cardinserted() (false)
#endif

/************************************************************************************
 * Name: nxphlite_writeprotected
 *
 * Description:
 *   Check if the card in the MMC/SD slot is write protected
 *
 ************************************************************************************/

#ifdef HAVE_AUTOMOUNTER
bool nxphlite_writeprotected(void);
#else
#  define nxphlite_writeprotected() (false)
#endif

/************************************************************************************
 * Name:  nxphlite_automount_initialize
 *
 * Description:
 *   Configure auto-mounter for the configured SDHC slot
 *
 * Input Parameters:
 *   None
 *
 *  Returned Value:
 *    None
 *
 ************************************************************************************/

#ifdef HAVE_AUTOMOUNTER
void nxphlite_automount_initialize(void);
#endif

/************************************************************************************
 * Name:  nxphlite_automount_event
 *
 * Description:
 *   The SDHC card detection logic has detected an insertion or removal event.  It
 *   has already scheduled the MMC/SD block driver operations.  Now we need to
 *   schedule the auto-mount event which will occur with a substantial delay to make
 *   sure that everything has settle down.
 *
 * Input Parameters:
 *   inserted - True if the card is inserted in the slot.  False otherwise.
 *
 *  Returned Value:
 *    None
 *
 *  Assumptions:
 *    Interrupts are disabled.
 *
 ************************************************************************************/

#ifdef HAVE_AUTOMOUNTER
void nxphlite_automount_event(bool inserted);
#endif

/************************************************************************************
 * Name: nxphlite_timer_initialize
 *
 * Description:
 *   Called to configure the FTM to provide 1 Mhz
 *
 ************************************************************************************/

void nxphlite_timer_initialize(void);

#include "../common/board_common.h"

#endif /* __ASSEMBLY__ */

__END_DECLS
