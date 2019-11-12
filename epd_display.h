#ifndef __edp_display_H__
#define __edp_display_H__

#include <gpiopin.h>
#include "epd_canvas.h"

namespace Epd
{

class Display
{
	public:
		Display( uint16_t width, uint16_t height, Gpio::Pin *cs, Gpio::Pin *dc, Gpio::Pin *reset, Gpio::Pin *busy );

	    virtual void hardReset() = 0;
		virtual bool initializePins();

		inline bool activateFullUpdate();
		inline bool activatePartialUpdate();
		inline bool isFullUpdated() const;

		inline uint16_t width() const;
		inline uint16_t height() const;
		
		virtual void clear() = 0;
		virtual void update() = 0;
		virtual void copy( Canvas &canvas, uint16_t x = 0, uint16_t y = 0, uint16_t width = 0, uint16_t height = 0 ) = 0;

	protected:
		uint16_t m_width, m_height;
		Gpio::Pin *m_cs, *m_dc, *m_reset, *m_busy;
		bool m_fullUpdated;

		virtual bool activate( bool fullUpdate ) = 0;
		void waitUntilIdle();
		void spiTransfer( uint8_t data );
		void sendCommand( uint8_t command );
		void sendData( uint8_t data );

};

//	----- inline functions -----
bool Display::activateFullUpdate()
{
	return activate( true );
}
bool Display::activatePartialUpdate()
{
	return activate( false );
}
bool Display::isFullUpdated() const
{
	return m_fullUpdated;
}
uint16_t Display::width() const
{
	return m_width;
}
uint16_t Display::height() const
{
	return m_height;
}


}

#endif 