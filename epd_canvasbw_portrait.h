#ifndef __epd_canvasbw_portrait_H__
#define __epd_canvasbw_portrait_H__
#include "epd_canvas.h"

namespace Epd
{
template< uint8_t _width_, uint8_t _height_ >
class CanvasBW_Portrait : public Canvas
{
	public:
		//
		//
		CanvasBW_Portrait() :
			Canvas( _width_, _height_ ),
			m_alignedWidth( _width_ + ( ( _width_ % 8 ) ? 8 - ( _width_ % 8 ) : 0 ) )
		{
		}

		//
		//
		void drawPixel( int16_t x, int16_t y, uint16_t color )
		{
			if( ( x >= width() ) || ( y >= height() ) )
				return;

			static const PROGMEM uint8_t bits[] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 }; 
			uint8_t *ptr = const_cast< uint8_t* >( image( x, y ) );
			switch( color )
			{
				case Canvas::White:
					*ptr |= pgm_read_byte( &bits[ x % 8 ] );
					break;
				case Canvas::Inverse:
					*ptr ^= pgm_read_byte( &bits[ x % 8 ] );
					break;
				default:
				case Canvas::Black:
					*ptr &= ~pgm_read_byte( &bits[ x % 8 ] );
					break;
			}
		}

		//
		//
		void fillScreen( uint16_t color )
		{
	        memset( m_buffer, ( color == Canvas::Black ) ? 0x00 : 0xff, memorySize() );
		}

		//
		//
		uint16_t memorySize() const
		{
			return sizeof( m_buffer );
		}

		//
		//
		const uint8_t* image() const
		{
			return m_buffer;
		}

		//
		//
		const uint8_t* image( uint8_t x, uint8_t y ) const
		{
			return &m_buffer[ ( ( y * m_alignedWidth ) + x ) / 8 ];
		}

		//
		//
		void setRotation(uint8_t /*r*/)
		{
			Canvas::setRotation( 0 );
		}


	protected:
	private:
		uint8_t m_alignedWidth;
		uint8_t m_buffer[ _height_ * ( ( _width_ + 7 ) / 8 ) ];
};

}

#endif