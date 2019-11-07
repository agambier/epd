#ifndef __edp_display_2in13v2_H__
#define __edp_display_2in13v2_H__

#include <gpiopin.h>
#include "epd_display.h"

namespace Epd
{

class Display_2in13v2
{
	public:
		Display_2in13v2( uint16_t width, uint16_t height, Gpio::Pin *cs, Gpio::Pin *dc, Gpio::Pin *reset, Gpio::Pin *busy );

	    void reset();
		bool initializePins();

		inline bool activateFullUpdate();
		inline bool activatePartialUpdate();
		inline bool isFullUpdated() const;

		inline uint16_t width() const;
		inline uint16_t height() const;
		
		void clear();
		void update();
		void copy( Canvas &canvas, uint8_t x = 0, uint8_t y = 0, uint8_t width = 0, uint8_t height = 0 );

	private:
		uint16_t m_width, m_height;
		uint8_t m_blockSize;
		Gpio::Pin *m_cs, *m_dc, *m_reset, *m_busy;
		bool m_fullUpdate;
	
	protected:
		bool begin( bool fullUpdate = true );
		void waitUntilIdle();

		// e-paper 2.13 inch commands
		enum Command
		{
			DRIVER_OUTPUT_CONTROL = 0x01,
			GATE_DRIVING_VOLTAGE_CONTROL = 0x03,
			SOURCE_DRIVING_VOLTAGE_CONTROL = 0x04,
			BOOSTER_SOFT_START_CONTROL = 0x0C,
			GATE_SCAN_START_POSITION = 0x0F,
			DEEP_SLEEP_MODE = 0x10,
			DATA_ENTRY_MODE_SETTING = 0x11,
			SW_RESET = 0x12,
			TEMPERATURE_SENSOR_CONTROL = 0x1A,
			MASTER_ACTIVATION = 0x20,
			DISPLAY_UPDATE_CONTROL_1 = 0x21,
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
			TERMINATE_FRAME_READ_WRITE = 0xFF,
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
uint16_t Display_2in13v2::width() const
{
	return m_width;
}
uint16_t Display_2in13v2::height() const
{
	return m_height;
}
bool Display_2in13v2::activateFullUpdate()
{
	return begin( true );
}
bool Display_2in13v2::activatePartialUpdate()
{
	return begin( false );
}
bool Display_2in13v2::isFullUpdated() const
{
	return m_fullUpdate;
}


}

#endif 