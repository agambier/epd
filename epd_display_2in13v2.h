#ifndef __edp_display_2in13v2_H__
#define __edp_display_2in13v2_H__

#include <gpiopin.h>
#include "epd_display.h"

namespace Epd
{

class Display_2in13v2 : public Display
{
	public:
		Display_2in13v2( bool landscape, Gpio::Pin *cs, Gpio::Pin *dc, Gpio::Pin *reset, Gpio::Pin *busy );

	    void hardReset();

		void clear();
		void update();
		void copy( Canvas &canvas, uint16_t x = 0, uint16_t y = 0, uint16_t width = 0, uint16_t height = 0 );

	protected:
		bool m_lanscape;
		uint16_t m_blockSize;

		bool activate( bool fullUpdate );

		// e-paper 2.13 inch commands
		enum Command
		{
			DRIVER_OUTPUT_CONTROL = 0x01,
			GATE_DRIVING_VOLTAGE_CONTROL = 0x03,
			SOURCE_DRIVING_VOLTAGE_CONTROL = 0x04,
			DATA_ENTRY_MODE_SETTING = 0x11,
			SW_RESET = 0x12,
			MASTER_ACTIVATION = 0x20,
			DISPLAY_UPDATE_CONTROL_2 = 0x22,
			WRITE_RAM = 0x24,
			WRITE_SHADOW_RAM = 0x26,
			WRITE_VCOM_REGISTER = 0x2C,
			WRITE_LUT_REGISTER = 0x32,
			CMD_37 = 0x37,
			SET_DUMMY_LINE_PERIOD = 0x3A,
			SET_GATE_TIME = 0x3B,
			BORDER_WAVEFORM_CONTROL = 0x3C,
			SET_RAM_AXIS1_ADDRESS_START_END_POSITION = 0x44,
			SET_RAM_AXIS2_ADDRESS_START_END_POSITION = 0x45,
			SET_RAM_AXIS1_ADDRESS_COUNTER = 0x4E,
			SET_RAM_AXIS2_ADDRESS_COUNTER = 0x4F,
			SET_ANALOG_BLOCK_CONTROL = 0x74,
			SET_DIGITAL_BLOCK_CONTROL = 0x7E,
		};

		void initializeLut();
		void spiTransfer( uint8_t data );
		void sendCommand( uint8_t command );
		void sendData( uint8_t data );
		void setMemoryPointer( uint8_t x, uint8_t y );

};

//	----- inline functions -----


}

#endif 