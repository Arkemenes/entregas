/**
 * \file
 *
 * \brief lcd controller ili93xx example.
 *
 * Copyright (c) 2014-2015 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

/**
 * \mainpage lcd controller ili93xx Example
 *
 * \section Purpose
 *
 * This example demonstrates how to configure lcd controller ili9325 or
 * ili9341 to control the LCD on the board.
 *
 * \section Requirements
 *
 * This package can be used with SAM4E-EK evaluation kits.
 *
 * \section Description
 *
 * This example first configure ili93xx for access the LCD controller,
 * then initialize the LCD, finally draw some text, image, basic shapes (line,
 * rectangle, circle) on LCD.
 *
 * \section Usage
 *
 * -# Build the program and download it inside the evaluation board.
 * -# Some text, image and basic shapes should be displayed on the LCD.
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include "asf.h"
#include "stdio_serial.h"
#include "conf_board.h"
#include "conf_clock.h"
#include "smc.h"

#define PIN_PUSHBUTTON_1_MASK	PIO_PB3
#define PIN_PUSHBUTTON_1_PIO	PIOB
#define PIN_PUSHBUTTON_1_ID		ID_PIOB
#define PIN_PUSHBUTTON_1_TYPE	PIO_INPUT
#define PIN_PUSHBUTTON_1_ATTR	PIO_PULLUP | PIO_DEBOUNCE | PIO_IT_RISE_EDGE

#define PIN_PUSHBUTTON_2_MASK	PIO_PC12
#define PIN_PUSHBUTTON_2_PIO	PIOC
#define PIN_PUSHBUTTON_2_ID		ID_PIOC
#define PIN_PUSHBUTTON_2_TYPE	PIO_INPUT
#define PIN_PUSHBUTTON_2_ATTR	PIO_PULLUP | PIO_DEBOUNCE | PIO_IT_FALL_EDGE

#define PIN_LED_BLUE 19
#define PIN_LED_GREEN 20
#define PIN_LED_RED 20
#define PIN_BUTTON		3
#define PIN_BUTTON_2	12

#define PORT_LED_BLUE PIOA
#define PORT_LED_GREEN PIOA
#define PORT_LED_RED PIOC
#define PORT_BUT_1 PIOB
#define PORT_BUT_2 PIOB


#define ID_LED_BLUE ID_PIOA
#define ID_LED_GREEN ID_PIOA
#define ID_LED_RED ID_PIOC
#define ID_BUT_1 ID_PIOB
#define ID_BUT_2 ID_PIOB

#define MASK_LED_BLUE (1u << PIN_LED_BLUE)
#define MASK_LED_GREEN (1u << PIN_LED_GREEN)
#define MASK_LED_RED (1u << PIN_LED_RED)
#define MASK_BUT_1		(1u << PIN_BUTTON)
#define MASK_BUT_2		(1u << PIN_BUTTON_2)

static uint32_t counter = 0;
static uint32_t timer = 0;

/************************************************************************/
/* 
Config Modules

                                                                     */
/************************************************************************/

//Botão

static void Button1_Handler(uint32_t id, uint32_t mask)
{
	ili93xx_set_foreground_color(COLOR_WHITE);
	ili93xx_draw_filled_rectangle(115,110,170,135);
	//Increase Counter
	counter++;
	ili93xx_set_foreground_color(COLOR_BLACK);
	char buffer[10];
	snprintf(buffer, 5, "%d", counter);
	ili93xx_draw_string(120, 120, (uint8_t *)buffer);
	
	

}

/**
 *  Handle Interrupcao botao 2.
 */
	
static void Button2_Handler(uint32_t id, uint32_t mask)
{
	ili93xx_set_foreground_color(COLOR_WHITE);
	ili93xx_draw_filled_rectangle(115,110,170,135);
	//Decrease Counter
	counter--;
	ili93xx_set_foreground_color(COLOR_BLACK);
	char buffer[10];
	snprintf(buffer, 5, "%d", counter);
	ili93xx_draw_string(120, 120, (uint8_t *)buffer);
}

/**
 *  Interrupt handler for TC0 interrupt. 
 */
void TC0_Handler(void)
{
	volatile uint32_t ul_dummy;

	/* Clear status bit to acknowledge interrupt */
	ul_dummy = tc_get_status(TC0,0);

	/* Avoid compiler warning */
	UNUSED(ul_dummy);

	/**Atualiza Tempo */
	
	timer ++;
	ili93xx_set_foreground_color(COLOR_WHITE);
	ili93xx_draw_filled_rectangle(175,295,250, 320);
	ili93xx_set_foreground_color(COLOR_BLACK);
	int min = timer / 60;
	int seg = timer % 60;

	
	char buffer[10];
	snprintf(buffer, 10, "%02d:%02d", min, seg);
	ili93xx_draw_string(180, 300, (uint8_t *)buffer);
	
	
		//ili93xx_draw_string(80, 140, (uint8_t *)"4");




}

/**
 *  \brief Configure the Pushbuttons
 *
 *  Configure the PIO as inputs and generate corresponding interrupt when
 *  pressed or released.
 */
static void configure_buttons(int counter)
{
	pmc_enable_periph_clk(PIN_PUSHBUTTON_1_ID);
	pmc_enable_periph_clk(PIN_PUSHBUTTON_2_ID);	
	pio_set_input(PIN_PUSHBUTTON_1_PIO, PIN_PUSHBUTTON_1_MASK, PIO_PULLUP | PIO_DEBOUNCE);
	pio_set_input(PIN_PUSHBUTTON_2_PIO, PIN_PUSHBUTTON_2_MASK, PIO_PULLUP | PIO_DEBOUNCE);
	pio_set_debounce_filter(PIN_PUSHBUTTON_1_PIO, PIN_PUSHBUTTON_1_MASK, 10);
	pio_set_debounce_filter(PIN_PUSHBUTTON_2_PIO, PIN_PUSHBUTTON_2_MASK, 10);	
	pio_handler_set(PIN_PUSHBUTTON_1_PIO, PIN_PUSHBUTTON_1_ID, PIN_PUSHBUTTON_1_MASK, PIO_IT_FALL_EDGE | PIO_PULLUP, Button1_Handler);
	pio_handler_set(PIN_PUSHBUTTON_2_PIO, PIN_PUSHBUTTON_2_ID, PIN_PUSHBUTTON_2_MASK, PIO_IT_FALL_EDGE | PIO_PULLUP, Button2_Handler);	
	pio_enable_interrupt(PIN_PUSHBUTTON_1_PIO , PIN_PUSHBUTTON_1_MASK);
	pio_enable_interrupt(PIN_PUSHBUTTON_2_PIO , PIN_PUSHBUTTON_2_MASK);
	NVIC_SetPriority((IRQn_Type) PIN_PUSHBUTTON_1_ID, 0);
	NVIC_SetPriority((IRQn_Type) PIN_PUSHBUTTON_2_ID, 0);	
	NVIC_EnableIRQ((IRQn_Type) PIN_PUSHBUTTON_1_ID);
	NVIC_EnableIRQ((IRQn_Type) PIN_PUSHBUTTON_2_ID);
	}


//Timer


/**
 *  Configure Timer Counter 0 to generate an interrupt every 250ms.
 */
// [main_tc_configure]
static void configure_tc(void)
{
	/*
	* Aqui atualizamos o clock da cpu que foi configurado em sysclk init
	*
	* O valor atual est'a em : 120_000_000 Hz (120Mhz)
	*/
	uint32_t ul_sysclk = sysclk_get_cpu_hz();
	
	/*
	*	Ativa o clock do periférico TC 0
	* 
	*/
	pmc_enable_periph_clk(ID_TC0);

	/*
	* Configura TC para operar no modo de comparação e trigger RC
	* devemos nos preocupar com o clock em que o TC irá operar !
	*
	* Cada TC possui 3 canais, escolher um para utilizar.
	*
	* Configurações de modo de operação :
	*	#define TC_CMR_ABETRG (0x1u << 10) : TIOA or TIOB External Trigger Selection 
	*	#define TC_CMR_CPCTRG (0x1u << 14) : RC Compare Trigger Enable 
	*	#define TC_CMR_WAVE   (0x1u << 15) : Waveform Mode 
	*
	* Configurações de clock :
	*   #define  TC_CMR_TCCLKS_TIMER_CLOCK1 : Clock selected: internal MCK/2 clock signal 
	*	#define  TC_CMR_TCCLKS_TIMER_CLOCK2 : Clock selected: internal MCK/8 clock signal 
	*	#define  TC_CMR_TCCLKS_TIMER_CLOCK3 : Clock selected: internal MCK/32 clock signal 
	*	#define  TC_CMR_TCCLKS_TIMER_CLOCK4 : Clock selected: internal MCK/128 clock signal
	*	#define  TC_CMR_TCCLKS_TIMER_CLOCK5 : Clock selected: internal SLCK clock signal 
	*
	*	MCK		= 120_000_000
	*	SLCK	= 32_768		(rtc)
	*
	* Uma opção para achar o valor do divisor é utilizar a funcao
	* tc_find_mck_divisor()
	*/
	tc_init(TC0,0,TC_CMR_CPCTRG | TC_CMR_TCCLKS_TIMER_CLOCK5);
	
	/*
	* Aqui devemos configurar o valor do RC que vai trigar o reinicio da contagem
	* devemos levar em conta a frequência que queremos que o TC gere as interrupções
	* e tambem a frequencia com que o TC está operando.
	*
	* Devemos configurar o RC para o mesmo canal escolhido anteriormente.
	*	
	*   ^ 
	*	|	Contador (incrementado na frequencia escolhida do clock)
	*   |
	*	|	 	Interrupcao	
	*	|------#----------- RC
	*	|	  /
	*	|   /
	*	| /
	*	|-----------------> t
	*
	*
	*/
	tc_write_rc(TC0,0,32768);
	
	/*
	* Devemos configurar o NVIC para receber interrupções do TC 
	*/
	NVIC_EnableIRQ((IRQn_Type) ID_TC0);
	
	/*
	* Opções possíveis geradoras de interrupção :
	* 
	* Essas configurações estão definidas no head : tc.h 
	*
	*	#define TC_IER_COVFS (0x1u << 0)	Counter Overflow 
	*	#define TC_IER_LOVRS (0x1u << 1)	Load Overrun 
	*	#define TC_IER_CPAS  (0x1u << 2)	RA Compare 
	*	#define TC_IER_CPBS  (0x1u << 3)	RB Compare 
	*	#define TC_IER_CPCS  (0x1u << 4)	RC Compare 
	*	#define TC_IER_LDRAS (0x1u << 5)	RA Loading 
	*	#define TC_IER_LDRBS (0x1u << 6)	RB Loading 
	*	#define TC_IER_ETRGS (0x1u << 7)	External Trigger 
	*/
	tc_enable_interrupt(TC0,0,TC_IER_CPCS);
	
	tc_start(TC0,0);
 }












/** Chip select number to be set */
#define ILI93XX_LCD_CS      1

struct ili93xx_opt_t g_ili93xx_display_opt;

/**
 *  Configure UART console.
 */
static void configure_console(void)
{
	const usart_serial_options_t uart_serial_options = {
		.baudrate = CONF_UART_BAUDRATE,
		.paritytype = CONF_UART_PARITY
	};

	/** Configure console UART. */
	sysclk_enable_peripheral_clock(CONSOLE_UART_ID);
	stdio_serial_init(CONF_UART, &uart_serial_options);
}

/**
 * \brief Application entry point for smc_lcd example.
 *
 * \return Unused (ANSI-C compatibility).
 */
int main(void)
{
	sysclk_init();
	board_init();

	/** Initialize debug console */
	configure_console();

	/** Enable peripheral clock */
	pmc_enable_periph_clk(ID_SMC);

	/** Configure SMC interface for Lcd */
	smc_set_setup_timing(SMC, ILI93XX_LCD_CS, SMC_SETUP_NWE_SETUP(2)
			| SMC_SETUP_NCS_WR_SETUP(2)
			| SMC_SETUP_NRD_SETUP(2)
			| SMC_SETUP_NCS_RD_SETUP(2));
	smc_set_pulse_timing(SMC, ILI93XX_LCD_CS, SMC_PULSE_NWE_PULSE(4)
			| SMC_PULSE_NCS_WR_PULSE(4)
			| SMC_PULSE_NRD_PULSE(10)
			| SMC_PULSE_NCS_RD_PULSE(10));
	smc_set_cycle_timing(SMC, ILI93XX_LCD_CS, SMC_CYCLE_NWE_CYCLE(10)
			| SMC_CYCLE_NRD_CYCLE(22));
#if ((!defined(SAM4S)) && (!defined(SAM4E)))
	smc_set_mode(SMC, ILI93XX_LCD_CS, SMC_MODE_READ_MODE
			| SMC_MODE_WRITE_MODE
			| SMC_MODE_DBW_8_BIT);
#else
	smc_set_mode(SMC, ILI93XX_LCD_CS, SMC_MODE_READ_MODE
			| SMC_MODE_WRITE_MODE);
#endif
	/** Initialize display parameter */
	g_ili93xx_display_opt.ul_width = ILI93XX_LCD_WIDTH;
	g_ili93xx_display_opt.ul_height = ILI93XX_LCD_HEIGHT;
	g_ili93xx_display_opt.foreground_color = COLOR_BLACK;
	g_ili93xx_display_opt.background_color = COLOR_WHITE;

	/** Switch off backlight */
	aat31xx_disable_backlight();

	/** Initialize LCD */
	ili93xx_init(&g_ili93xx_display_opt);

	/** Set backlight level */
	aat31xx_set_backlight(AAT31XX_AVG_BACKLIGHT_LEVEL);

	ili93xx_set_foreground_color(COLOR_WHITE);
	ili93xx_draw_filled_rectangle(0, 0, ILI93XX_LCD_WIDTH,
			ILI93XX_LCD_HEIGHT);
	/** Turn on LCD */
	ili93xx_display_on();
	ili93xx_set_cursor_position(0, 0);

	/*
	*
	*Cabeçalho
	*
	*/
	ili93xx_set_foreground_color(COLOR_BLACK);
	ili93xx_draw_string(10, 20, (uint8_t *)"Rodrigo 13.04537-7");
	
	ili93xx_draw_string(10, 40, (uint8_t *)"Eric    13.01703-9");
	
	ili93xx_draw_string(10, 60, (uint8_t *)"Thiago  12.03287-5");

	ili93xx_draw_line(0, 90, 400, 90);

	ili93xx_draw_line(0, 100, 400, 100);
	
	//Fim do cabeçalho
	
	//Contador
	
	ili93xx_draw_string(10, 120, (uint8_t *)"Contador:0");
	
		//Elapsed_Time
	ili93xx_draw_string(108, 300, (uint8_t *)"Timer:00:00");


	
	

	/* Disable the watchdog */
	WDT->WDT_MR = WDT_MR_WDDIS;

	/** Configura o timer */
	configure_tc();
	
	/* Configura os botões */
	configure_buttons(counter);

	while (1) {
				pmc_sleep(SAM_PM_SMODE_SLEEP_WFI);

	}
}

