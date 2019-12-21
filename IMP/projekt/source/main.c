/*
Jmeno a prijmeni: Katerina Fortova
Login: xforto00
Projekt: ARM-FITkit3: Mereni srdecniho tepu
Predmet: IMP
original
Datum poslednich zmen v souboru: 21. prosince 2019
*/

#include "MK60D10.h" // for FitKIT
#include "fsl_gpio.h" // for General input/output
#include "pin_mux.h" // for configuration of ports
#include "fsl_pit.h" // for Periodic interrupt timer - refreshing display
#include "fsl_clock.h" // for work with clk
#include "fsl_lptmr.h" // for Low-power timer - beat measuring
#include "fsl_adc16.h" // for work with Analog-to-digital converter, transform analog signal from module to digital

// C libraries:
#include <stdio.h>
#include <stdbool.h>

static uint32_t clk_frequency; // frequency of clk

static char measured_average_value_str[5]; // string for show final result on display
static size_t measured_average_value_str_index = 3; // indexes of numbers on display are 0 - 3

/*
Function for display next measured value on 7-segment display
*/
void displayNextMeasuredValue()
{
	if (++measured_average_value_str_index == 4) // overflow index, back to index 0
	{
		measured_average_value_str_index = 0;
	}

	// clean every segment and all digits on display
	GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_FIRST_NUM_GPIO, BOARD_INITPINS_DISPLAY_FIRST_NUM_PIN, 1);
	GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SECOND_NUM_GPIO, BOARD_INITPINS_DISPLAY_SECOND_NUM_PIN, 1);
	GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_THIRD_NUM_GPIO, BOARD_INITPINS_DISPLAY_THIRD_NUM_PIN, 1);
	GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_FOURTH_NUM_GPIO, BOARD_INITPINS_DISPLAY_FOURTH_NUM_PIN, 1);

	GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_A_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_A_PIN, 0);
	GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_B_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_B_PIN, 0);
	GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_C_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_C_PIN, 0);
	GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_D_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_D_PIN, 0);
	GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_E_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_E_PIN, 0);
	GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_F_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_F_PIN, 0);
	GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_G_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_G_PIN, 0);

	// control each digit of display on particular index
	// first digit on display:
	if (measured_average_value_str_index == 0)
	{
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_FIRST_NUM_GPIO, BOARD_INITPINS_DISPLAY_FIRST_NUM_PIN, 0);
	}

	// second digit on display:
	else if (measured_average_value_str_index == 1)
	{
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SECOND_NUM_GPIO, BOARD_INITPINS_DISPLAY_SECOND_NUM_PIN, 0);
	}

	// third digit on display:
	else if (measured_average_value_str_index == 2)
	{
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_THIRD_NUM_GPIO, BOARD_INITPINS_DISPLAY_THIRD_NUM_PIN, 0);
	}

	// fourth digit on display:
	else if (measured_average_value_str_index == 3)
	{
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_FOURTH_NUM_GPIO, BOARD_INITPINS_DISPLAY_FOURTH_NUM_PIN, 0);
	}

	// display particular digit on index
	// show digit 0 on particular index:
	if (measured_average_value_str[measured_average_value_str_index] == '0')
	{
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_A_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_A_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_B_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_B_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_C_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_C_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_D_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_D_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_E_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_E_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_F_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_F_PIN, 1);
	}

	// show digit 1 on particular index:
	else if (measured_average_value_str[measured_average_value_str_index] == '1')
	{
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_B_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_B_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_C_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_C_PIN, 1);
	}

	// show digit 2 on particular index:
	else if (measured_average_value_str[measured_average_value_str_index] == '2')
	{
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_A_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_A_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_B_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_B_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_D_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_D_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_E_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_E_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_G_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_G_PIN, 1);
	}

	// show digit 3 on particular index:
	else if (measured_average_value_str[measured_average_value_str_index] == '3')
	{
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_A_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_A_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_B_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_B_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_C_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_C_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_D_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_D_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_G_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_G_PIN, 1);
	}

	// show digit 4 on particular index:
	else if (measured_average_value_str[measured_average_value_str_index] == '4')
	{
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_B_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_B_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_C_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_C_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_F_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_F_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_G_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_G_PIN, 1);
	}

	// show digit 5 on particular index:
	else if (measured_average_value_str[measured_average_value_str_index] == '5')
	{
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_A_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_A_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_C_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_C_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_D_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_D_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_F_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_F_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_G_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_G_PIN, 1);
	}

	// show digit 6 on particular index:
	else if (measured_average_value_str[measured_average_value_str_index] == '6')
	{
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_A_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_A_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_C_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_C_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_D_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_D_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_E_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_E_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_F_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_F_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_G_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_G_PIN, 1);
	}

	// show digit 7 on particular index:
	else if (measured_average_value_str[measured_average_value_str_index] == '7')
	{
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_A_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_A_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_B_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_B_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_C_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_C_PIN, 1);
	}

	// show digit 8 on particular index:
	else if (measured_average_value_str[measured_average_value_str_index] == '8')
	{
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_A_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_A_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_B_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_B_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_C_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_C_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_D_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_D_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_E_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_E_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_F_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_F_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_G_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_G_PIN, 1);
	}

	// show digit 9 on particular index:
	else if (measured_average_value_str[measured_average_value_str_index] == '9')
	{
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_A_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_A_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_B_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_B_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_C_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_C_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_D_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_D_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_F_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_F_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_G_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_G_PIN, 1);
	}

}

/*
Function for adding interrupt handler and show next measured value on display
*/
void PIT0_IRQHandler()
{
	displayNextMeasuredValue(); // show next value on 7-segment display

	PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, kPIT_TimerFlag); // clear PIT status flags
}

/*
Function for init PIT for refreshing display after particular refresh rate
*/
void PITinit()
{
	// refresh rate of display in microseconds
	// f = 60 Hz, 1 / 60 = 0,016 s = 16666,667
	// we have 4 numbers on display - 16666,667 / 4 = 4166,667
	static uint64_t display_refresh_num_rate = 4167;

	CLOCK_EnableClock(kCLOCK_Pit0); // enable clock for PIT

	// config PIT
	pit_config_t PIT_config;
	PIT_GetDefaultConfig(&PIT_config);
	PIT_Init(PIT, &PIT_config);

	// enable PIT0_IRQHandler as PIT interrupt handler
	EnableIRQ(PIT0_IRQn);
	PIT_EnableInterrupts(PIT, kPIT_Chnl_0, kPIT_TimerInterruptEnable);

	// count timer period
	uint64_t PIT_timer_period = USEC_TO_COUNT(display_refresh_num_rate, CLOCK_GetFreq(kCLOCK_BusClk));
	// refresh every 4167 microseconds with interrupt
	PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, (uint32_t) PIT_timer_period);


	PIT_StartTimer(PIT, kPIT_Chnl_0); // starting timer for PIT
}


/*
Function for init LPTMR for count time intervals and time during beat measuring
*/
void LPTMRinit()
{
	// setting MCGIRCLK as clock for LPTMR
	CLOCK_SetInternalRefClkConfig(kMCG_IrclkEnable, kMCG_IrcSlow, 0);
	// get frequency of clock
	clk_frequency = CLOCK_GetFreq(kCLOCK_McgInternalRefClk);

	// enable clock
	CLOCK_EnableClock(kCLOCK_Lptmr0);

	// config LPTMR
	lptmr_config_t LPTMR_config;
	LPTMR_GetDefaultConfig(&LPTMR_config);

	// config LPTMR clock source
	LPTMR_config.prescalerClockSource = kLPTMR_PrescalerClock_0;

	LPTMR_Init(LPTMR0, &LPTMR_config); // init LPTMR

	// count until value 0xFFFF
	LPTMR_SetTimerPeriod(LPTMR0, 0xFFFF);
}


/*
Function for init ADC - convert analog signal to digital from module
*/
void ADCinit()
{
	// enable clock
	CLOCK_EnableClock(kCLOCK_Adc0);

	// config ADC
	adc16_config_t ADC_config;
	ADC16_GetDefaultConfig(&ADC_config);
	ADC_config.enableContinuousConversion = true;

	// setting sample resolution - 16-bit
	#if ((defined(FSL_FEATURE_ADC16_MAX_RESOLUTION)) && (FSL_FEATURE_ADC16_MAX_RESOLUTION >= 16))
		ADC_config.resolution = kADC16_Resolution16Bit;
	#endif

	ADC16_Init(ADC0, &ADC_config); // init ADC

	// make hardware trigger feature not enabled
	ADC16_EnableHardwareTrigger(ADC0, false);

	// set hardware average mode, conversion result is doing automatically by using hardware
	ADC16_SetHardwareAverage(ADC0, kADC16_HardwareAverageCount32);

	// calibration of ADC
	#if ((defined(FSL_FEATURE_ADC16_HAS_CALIBRATION)) && (FSL_FEATURE_ADC16_HAS_CALIBRATION))
		ADC16_DoAutoCalibration(ADC0);
	#endif

	// setting channel of ADC
	adc16_channel_config_t ADC_channel_config =
	{
		.channelNumber = 0,
		.enableInterruptOnConversionCompleted = false,
	};

	// use differential sample mode
	#if (defined(FSL_FEATURE_ADC16_HAS_DIFF_MODE) && FSL_FEATURE_ADC16_HAS_DIFF_MODE)
		ADC_channel_config.enableDifferentialConversion = false;
	#endif

	ADC16_SetChannelConfig(ADC0, 0, &ADC_channel_config);
}

/*
Function for filtering signal from module with low-pass filter and high-pass filter
*/
float signalFiltering(float dt, float x_i_lowpass)
{
	// fc = 1 / (2 * pi * tau)
	// tau = 1 / (fc * 2 * pi)
	// fc = 2 Hz - average fc of heartbeat
	// 1 / (2 * pi * 2) = 0.0795774155 seconds = 79577.47155 microseconds
	float tau = 79577.47155;

	// lowpass filtering:
	float y_i_lowpass;
	static float y_i_preceding_lowpass = 0.0;

	// highpass filtering:
	static float x_i_highpass = 0.0;
	float y_i_highpass;
	static float x_i_preceding_highpass = 0.0;
	static float y_i_preceding_highpass = 0.0;

	// variable for control x_0 index:
	static int actual_x_index = 0;

	float alpha_lowpass = dt / (tau + dt);

	// control when index of x is 0 - no preceding index, this is first index of signal
	if (actual_x_index == 0)
	{
		y_i_lowpass = x_i_lowpass * alpha_lowpass;
		y_i_preceding_lowpass = y_i_lowpass;

		y_i_highpass = x_i_highpass;
		x_i_preceding_highpass = x_i_highpass;
		y_i_preceding_highpass = y_i_highpass;

		actual_x_index++;
	}


	/*
	 // Return RC low-pass filter output samples, given input samples,
 	 // time interval dt, and time constant RC
 	 function lowpass(real[0..n] x, real dt, real RC)
   	   var real[0..n] y
   	   var real α := dt / (RC + dt)
   	   y[0] := α * x[0]
   	   for i from 1 to n
       	   y[i] := α * x[i] + (1-α) * y[i-1]
   	   return y

   	   Source: https://en.wikipedia.org/wiki/Low-pass_filter
	 */

	y_i_lowpass = (alpha_lowpass * x_i_lowpass) + ((1.0 - alpha_lowpass) * y_i_preceding_lowpass);

	y_i_preceding_lowpass = y_i_lowpass;

	x_i_highpass = y_i_lowpass;

	/*
	 // Return RC high-pass filter output samples, given input samples,
 	 // time interval dt, and time constant RC
 	 function highpass(real[0..n] x, real dt, real RC)
   	   var real[0..n] y
   	   var real α := RC / (RC + dt)
   	   y[0] := x[0]
   	   for i from 1 to n
     	 y[i] := α * y[i-1] + α * (x[i] - x[i-1])
   	   return y

   	   Source: https://en.wikipedia.org/wiki/High-pass_filter
	 */

	float alpha_highpass = tau / (tau + dt);


	y_i_highpass = (alpha_highpass * y_i_preceding_highpass) + (alpha_highpass * (x_i_highpass - x_i_preceding_highpass));


	x_i_preceding_highpass = x_i_highpass;
	y_i_preceding_highpass = y_i_highpass;

	return y_i_highpass;

}

/*
Function for measuring bpm from module
*/
unsigned int measureRate(uint32_t clk_frequency)
{
	static uint64_t measuring_time = 0; // time of measuring
	static bool began_measuring = false; // control whether measuring began

	static float measured_values_arr[3];
	static size_t measured_values_arr_index = 0; // index to array of measured values

	static float max_signal_value = 0.0; // maximum of signal
	static bool ascending_signal = false; // control whether signal is ascending or not
	float measured_value = 0.0; // measured value in float
	static unsigned int measured_average_value = 0; // measured average value as unsigned int

	float max_human_rate = 220.0; // max bpm of human
	float min_human_rate = 60.0; // min bpm of human

	bool conversion_undone = true; // bool for control whether conversion of signal is done or not

	while (conversion_undone == true) // wait for finish conversion of signal
	{
		// get signal from module
		uint32_t ADC_status_flags = ADC16_GetChannelStatusFlags(ADC0, 0);
		conversion_undone = !((ADC_status_flags) & (kADC16_ChannelConversionDoneFlag));
	}

	uint32_t module_signal = ADC16_GetChannelConversionValue(ADC0, 0);

	// time interval dt between measuring in microseconds
	uint64_t dt = COUNT_TO_USEC((uint64_t) LPTMR_GetCurrentTimerCount(LPTMR0), clk_frequency);

	measuring_time = measuring_time + dt; // counting time of measuring

	// restart of LPTMR timer
	LPTMR_StopTimer(LPTMR0);
	LPTMR_StartTimer(LPTMR0);

	// filter signal with low-pass and high-pass filter
	float filtered_signal = signalFiltering((float) dt, (float) module_signal);

	// signal is ascending
	if (ascending_signal == true)
	{
		// finding maximum of signal
		if ((filtered_signal > max_signal_value) && (filtered_signal > 0.0))
		{
			max_signal_value = filtered_signal;
		}
		else // found maximum of signal
		{
			if (max_signal_value != 0.0)
			{
				if ((measuring_time != 0 ) && (began_measuring == true))
				{
					measured_value = (60.0 / ((float) measuring_time / 1000000.0)); // compute result in bpm
					//measured_value = measured_value - 70.0; // control because of better accuracy
					// ignore rates larger than max human bpm and below min human bpm
					if ((measured_value <= max_human_rate) && (measured_value >= min_human_rate))
					{
						// add value to array
						measured_values_arr[measured_values_arr_index] = measured_value;
						measured_values_arr_index++;
					}
				}

				ascending_signal = false; // now signal is descending
				began_measuring = true;
				measuring_time = 0; // measure new value

			}
		}
	}
	else
	{
		if (filtered_signal <= 0.0) // control negative values
		{
			ascending_signal = true;
			max_signal_value = 0.0; // maximum of signal is 0
		}

	}

	if (measured_values_arr_index == 3) // array of measured values is full
	{

		int sum = 0;

		for (int i = 0; i < 3; i++)
		{
			sum = sum + measured_values_arr[i];
		}

		// compute average measured value of 3 values in array
		measured_average_value = (sum / 3);

		// clean whole array
		for (size_t i = 0; i < 3; i++)
		{
			measured_values_arr[i] = 0.0;
		}

		measured_values_arr_index = 0; // set index to first value
	}

	return measured_average_value;

}


/*
*** MAIN ***
*/
int main()
{
	// init everything necessary for project
	BOARD_InitBootPins();
	PITinit();
	LPTMRinit();
	ADCinit();

	// display 0 in the beginning of measuring:
	GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_A_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_A_PIN, 1);
	GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_B_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_B_PIN, 1);
	GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_C_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_C_PIN, 1);
	GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_D_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_D_PIN, 1);
	GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_E_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_E_PIN, 1);
	GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_F_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_F_PIN, 1);

	while (true)
	{
		unsigned int measured_average_value = measureRate(clk_frequency); // measure average rate
		sprintf(measured_average_value_str, "% *u", 4, measured_average_value); // display average rate
	}
}
