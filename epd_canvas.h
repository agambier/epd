#ifndef __epd_canvas_H__
#define __epd_canvas_H__

#include <Adafruit_GFX.h>

namespace Epd
{

class Canvas : public Adafruit_GFX
{
	public:
		enum Color
		{
			Black = 0,
			White,
			Inverse
		} __attribute__( ( __packed__ ) ) ;

		Canvas( uint8_t w, uint8_t h ) : 
			Adafruit_GFX( w, h )
		{
		}
		virtual uint16_t memorySize() const = 0;
		virtual const uint8_t* image() const = 0;
		virtual const uint8_t* image( uint8_t x, uint8_t y ) const;
	private:
};
//	----- inline function -----
}

#endif