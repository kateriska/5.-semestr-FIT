/*
Jméno a příjmení: Kateřina Fořtová
Login: xforto00
Projekt: ARM-FITkit3: Měření srdečního tepu
Předmět: IMP
originál
Datum posledních změn v souboru:
*/

#include "MK60D10.h" // for FitKIT
#include "fsl_gpio.h" // for General input/output
#include "pin_mux.h" // for configuration of ports
#include "fsl_pit.h" // for Periodic interrupt timer - refreshing display
#include "fsl_clock.h" // for work with clk
#include "fsl_lptmr.h" // for Low-power timer - beat measurement
#include "fsl_adc16.h" // for work with Analog-to-digital converter, transform analog signal from module to digital

// C libraries
#include <stdio.h>
#include <stdbool.h>

#define MEASURED_VALUES_ARR_SIZE 8 // count of measured values in array

static uint32_t clk_frequency; // // frequency of clk

static char measured_value_str[5]; // string for show final result on display


/*
Function for adding interrupt handler and show next measured value on display
*/
void PIT0_IRQHandler()
{
	displayNextMeasuredValue();
	PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, kPIT_TimerFlag);
}

/*
Function for init PIT for refreshing display after particular refresh rate
*/
void PITinit()
{
	// refresh rate of display in microseconds
	// f = 60 Hz, 1 / 60 = 0,016 s = 16666,667
	// for 1 number - 16666,667 / 4 = 4166,667
	static const uint64_t DISPLAY_REFRESH_NUM_RATE = 4167;

	CLOCK_EnableClock(kCLOCK_Pit0);

	pit_config_t PIT_config;
	PIT_GetDefaultConfig(&PIT_config);
	PIT_Init(PIT, &PIT_config);

	// setting PIT0_IRQHandler as PIT interrupt handler
	EnableIRQ(PIT0_IRQn);
	PIT_EnableInterrupts(PIT, kPIT_Chnl_0, kPIT_TimerInterruptEnable);

	const uint64_t PIT_timer_period = USEC_TO_COUNT(DISPLAY_REFRESH_NUM_RATE, CLOCK_GetFreq(kCLOCK_BusClk));
	PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, (uint32_t) PIT_timer_period);
	PIT_StartTimer(PIT, kPIT_Chnl_0);
}


/*
Function for init LPTMR for count time intervals and time during beat measurement
*/
void LPTMRinit()
{

	CLOCK_SetInternalRefClkConfig(kMCG_IrclkEnable, kMCG_IrcSlow, 0);
	clk_frequency = CLOCK_GetFreq(kCLOCK_McgInternalRefClk);
	CLOCK_EnableClock(kCLOCK_Lptmr0);

	lptmr_config_t LPTMR_config;
	LPTMR_GetDefaultConfig(&LPTMR_config);
	LPTMR_config.prescalerClockSource = kLPTMR_PrescalerClock_0;

	LPTMR_Init(LPTMR0, &LPTMR_config);
	static const uint32_t LPTMR0_period = 0xFFFF;
	LPTMR_SetTimerPeriod(LPTMR0, LPTMR0_period);
}


/*
Function for init ADC - convert analog signal to digital from module
*/
void ADCinit()
{
	CLOCK_EnableClock(kCLOCK_Adc0);

	adc16_config_t ADC_config;
	ADC16_GetDefaultConfig(&ADC_config);
	ADC_config.enableContinuousConversion = true;

	// setting sample resolution - 16-bit
#if (defined(FSL_FEATURE_ADC16_MAX_RESOLUTION) && FSL_FEATURE_ADC16_MAX_RESOLUTION >= 16)
	ADC_config.resolution = kADC16_Resolution16Bit;
#endif

	ADC16_Init(ADC0, &ADC_config);
	ADC16_EnableHardwareTrigger(ADC0, false);
	ADC16_SetHardwareAverage(ADC0, kADC16_HardwareAverageCount32);

	// calibration of ADC
#if (defined(FSL_FEATURE_ADC16_HAS_CALIBRATION) && FSL_FEATURE_ADC16_HAS_CALIBRATION)
	ADC16_DoAutoCalibration(ADC0);
#endif

	// setting channel of ADC
	adc16_channel_config_t ADC_channel_config = {
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
Function for display next measured value on 7-segment display
*/
void displayNextMeasuredValue()
{
	static size_t display_num_index = 3; // indexes are 0 - 3

	if (++display_num_index == 4) // overflow index, back to index 0
	{
		display_num_index = 0;
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
	if (display_num_index == 0)
	{
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_FIRST_NUM_GPIO, BOARD_INITPINS_DISPLAY_FIRST_NUM_PIN, 0);
	}
	else if (display_num_index == 1)
	{
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SECOND_NUM_GPIO, BOARD_INITPINS_DISPLAY_SECOND_NUM_PIN, 0);
	}
	else if (display_num_index == 2)
	{
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_THIRD_NUM_GPIO, BOARD_INITPINS_DISPLAY_THIRD_NUM_PIN, 0);
	}
	else if (display_num_index == 3)
	{
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_FOURTH_NUM_GPIO, BOARD_INITPINS_DISPLAY_FOURTH_NUM_PIN, 0);
	}

	// display particular digit at index
	// show digit 0 on particular index:
	if (measured_value_str[display_num_index] == '0')
	{
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_A_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_A_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_B_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_B_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_C_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_C_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_D_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_D_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_E_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_E_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_F_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_F_PIN, 1);
	}

	// show digit 1 on particular index:
	else if (measured_value_str[display_num_index] == '1')
	{
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_B_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_B_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_C_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_C_PIN, 1);
	}

	// show digit 2 on particular index:
	else if (measured_value_str[display_num_index] == '2')
	{
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_A_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_A_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_B_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_B_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_D_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_D_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_E_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_E_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_G_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_G_PIN, 1);
	}

	// show digit 3 on particular index:
	else if (measured_value_str[display_num_index] == '3')
	{
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_A_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_A_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_B_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_B_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_C_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_C_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_D_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_D_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_G_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_G_PIN, 1);
	}

	// show digit 4 on particular index:
	else if (measured_value_str[display_num_index] == '4')
	{
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_B_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_B_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_C_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_C_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_F_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_F_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_G_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_G_PIN, 1);
	}

	// show digit 5 on particular index:
	else if (measured_value_str[display_num_index] == '5')
	{
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_A_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_A_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_C_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_C_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_D_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_D_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_F_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_F_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_G_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_G_PIN, 1);
	}

	// show digit 6 on particular index:
	else if (measured_value_str[display_num_index] == '6')
	{
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_A_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_A_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_C_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_C_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_D_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_D_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_E_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_E_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_F_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_F_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_G_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_G_PIN, 1);
	}

	// show digit 7 on particular index:
	else if (measured_value_str[display_num_index] == '7')
	{
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_A_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_A_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_B_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_B_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_C_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_C_PIN, 1);
	}

	// show digit 8 on particular index:
	else if (measured_value_str[display_num_index] == '8')
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
	else if (measured_value_str[display_num_index] == '9')
	{
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_A_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_A_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_B_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_B_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_C_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_C_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_D_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_D_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_F_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_F_PIN, 1);
		GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_G_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_G_PIN, 1);
	}

}

float signalFiltering(float dt, float x_i_lowpass)
{
	// fc = 1 / 2 * pi * tau
	// tau = 1 / fc * 2 * pi
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

	// variable for control x_0 index
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

	y_i_lowpass = alpha_lowpass * x_i_lowpass + (1.0 - alpha_lowpass) * y_i_preceding_lowpass;

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


	y_i_highpass = alpha_highpass * y_i_preceding_highpass + alpha_highpass * (x_i_highpass - x_i_preceding_highpass);


	x_i_preceding_highpass = x_i_highpass;
	y_i_preceding_highpass = y_i_highpass;

	return y_i_highpass;

}


const unsigned int measureRate(const uint32_t clk_frequency)
{
	static uint64_t measurement_time = 0; // time of measurement
	static bool began_measurement = false; // control whether measurement began

	static float measured_values_arr[MEASURED_VALUES_ARR_SIZE] = {0.0};
	static size_t measured_values_arr_index = 0; // index to array of measured values

	static float max_signal_value = 0.0; // maximum of signal
	static bool rising_signal = false; // control whether signal is rising or not
	float measured_value = 0.0; // measured value in float
	static unsigned int measured_average_value = 0; // measured average value as unsigned int

	float max_human_rate = 220.0; // max bpm of human

	bool conversion_done = false; // bool for control whether conversion of signal is done

	while (conversion_done == false) // wait for finish conversion of signal
	{
		// get signal from module
		uint32_t status_flags = ADC16_GetChannelStatusFlags(ADC0, 0);
		conversion_done = (status_flags & kADC16_ChannelConversionDoneFlag);
	}

	uint32_t module_signal = ADC16_GetChannelConversionValue(ADC0, 0);

	// time interval dt between measurements in microseconds
	uint64_t dt = COUNT_TO_USEC((uint64_t) LPTMR_GetCurrentTimerCount(LPTMR0), clk_frequency);
	measurement_time = measurement_time + dt; // counting time if measure

	// reset of LPTMR timer
	LPTMR_StopTimer(LPTMR0);
	LPTMR_StartTimer(LPTMR0);

	float filtered_signal = signalFiltering((float) dt, (float) module_signal); // filter signal with low-pass and high-pass filter

	// signal is rising
	if (rising_signal == true)
	{
		// finding maximum of signal
		if (filtered_signal > max_signal_value)
		{
			max_signal_value = filtered_signal;
		}
		else // found maximum of signal
		{
			if (max_signal_value != 0.0)
			{
				rising_signal = false;

				if (measurement_time != 0 && began_measurement == true)
				{
					measured_value = (60.0 / ((float) measurement_time / 1000000.0)); // compute result in bpm
					//measured_value = measured_value - 60.0; // control because of better accuracy
					if (measured_value <= max_human_rate) // ignore rates larger than max human bpm
					{
						// add value to array
						measured_values_arr[measured_values_arr_index] = measured_value;
						measured_values_arr_index++;
					}
				}

				began_measurement = true;
				measurement_time = 0;

			}
		}
	}
	else
	{
		if (filtered_signal <= 0.0) // control negative values
		{
			rising_signal = true;
			max_signal_value = 0.0;
		}

	}

	if (measured_values_arr_index == MEASURED_VALUES_ARR_SIZE) // array of measured values is full
	{
		measured_values_arr_index = 0;

		int sum = 0;

		for (int i = 0; i < MEASURED_VALUES_ARR_SIZE; i++)
		{
			sum = sum + measured_values_arr[i];
		}

		// compute average measured value of MEASURED_VALUES_ARR_SIZE values
		measured_average_value = sum / MEASURED_VALUES_ARR_SIZE;

		// clean whole array
		for (size_t i = 0; i < MEASURED_VALUES_ARR_SIZE; i++)
		{
			measured_values_arr[i] = 0.0;
		}
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

	// display 0 in the beginning of measurement:
	GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_A_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_A_PIN, 1);
	GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_B_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_B_PIN, 1);
	GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_C_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_C_PIN, 1);
	GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_D_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_D_PIN, 1);
	GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_E_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_E_PIN, 1);
	GPIO_WritePinOutput(BOARD_INITPINS_DISPLAY_SEGMENT_F_GPIO, BOARD_INITPINS_DISPLAY_SEGMENT_F_PIN, 1);

	while (true)
	{
		unsigned int measured_value = measureRate(clk_frequency); // measure rate
		sprintf(measured_value_str, "% *u", 4, measured_value); // display rate
	}
}
