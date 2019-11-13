#include <SPI.h>
#include "epd_display_2in13v2.h"
#include <Arduino.h>

namespace Epd
{

//
//
Display_2in13v2::Display_2in13v2( bool landscape, Gpio::Pin *cs, Gpio::Pin *dc, Gpio::Pin *reset, Gpio::Pin *busy ) :
	Display( landscape ? 250 : 122, landscape ? 122 : 250, cs, dc, reset, busy ),
	m_lanscape( landscape ),
	m_blockSize( ( ( landscape ? m_height : m_width ) + 7 ) / 8 )
{
}

//
//
void Display_2in13v2::hardReset()
{
	if( nullptr == m_reset )
		return;

	m_reset->activate();
	delay( 200 );
	m_reset->deactivate();
	delay( 200 );
}

//
//
bool Display_2in13v2::activate( bool fullUpdate )
{
	if( ( nullptr == m_cs ) || ( nullptr == m_dc ) )
		return false;

	m_fullUpdated = fullUpdate;
	waitUntilIdle();
    if( fullUpdate) 
	{
		sendCommand( SW_RESET ); 
		waitUntilIdle();
		sendCommand( SET_ANALOG_BLOCK_CONTROL );
		sendData( 0x54 );
		sendCommand( SET_DIGITAL_BLOCK_CONTROL );
		sendData( 0x3B );
		sendCommand( DRIVER_OUTPUT_CONTROL );
		sendData( 0xF9 );
		sendData( 0x00 );
		sendData( 0x00 );
		sendCommand( BORDER_WAVEFORM_CONTROL );
		sendData( 0x03 );
		sendCommand( WRITE_VCOM_REGISTER );
		sendData(0x55); 
		sendCommand( GATE_DRIVING_VOLTAGE_CONTROL );
		sendData( 0x15 );
		sendCommand( SOURCE_DRIVING_VOLTAGE_CONTROL ); 
		sendData( 0x41 );
		sendData( 0xA8 );
		sendData( 0x32 );
		sendCommand( SET_DUMMY_LINE_PERIOD );
		sendData( 0x30 );
		sendCommand( SET_GATE_TIME );
		sendData( 0x0A );
		initializeLut();
    }
	else 
	{
        sendCommand( WRITE_VCOM_REGISTER );
        sendData( 0x26 );
        waitUntilIdle();
        initializeLut();
        sendCommand( CMD_37 );
        sendData( 0x00 );
        sendData( 0x00 );
        sendData( 0x00 );
        sendData( 0x00 );
        sendData( 0x40 );
        sendData( 0x00 );
        sendData( 0x00 );
        sendCommand( DISPLAY_UPDATE_CONTROL_2 );
        sendData( 0xC0 );
        sendCommand( 0x20 );
        waitUntilIdle();
        sendCommand( BORDER_WAVEFORM_CONTROL );
        sendData(0x01);
	}

	sendCommand( DATA_ENTRY_MODE_SETTING ); 
	sendData( 0x03 );

    sendCommand( SET_RAM_AXIS1_ADDRESS_START_END_POSITION );
    sendData( 0 );
    sendData( ( m_blockSize - 1 ) & 0x3f );
    sendCommand( SET_RAM_AXIS2_ADDRESS_START_END_POSITION );
    sendData( 0 );
    sendData( 0 );
    sendData( ( ( m_lanscape ? m_width : m_height ) - 1 ) & 0xff );
    sendData( 0 );

	waitUntilIdle();

	return true;
}

//
//
void Display_2in13v2::initializeLut()
{
	static const PROGMEM uint8_t
		full[] = {	0x80,0x60,0x40,0x00,0x00,0x00,0x00,             //LUT0: BB:     VS 0 ~7
					0x10,0x60,0x20,0x00,0x00,0x00,0x00,             //LUT1: BW:     VS 0 ~7
					0x80,0x60,0x40,0x00,0x00,0x00,0x00,             //LUT2: WB:     VS 0 ~7
					0x10,0x60,0x20,0x00,0x00,0x00,0x00,             //LUT3: WW:     VS 0 ~7
					0x00,0x00,0x00,0x00,0x00,0x00,0x00,             //LUT4: VCOM:   VS 0 ~7
					0x03,0x03,0x00,0x00,0x02,                       // TP0 A~D RP0
					0x09,0x09,0x00,0x00,0x02,                       // TP1 A~D RP1
					0x03,0x03,0x00,0x00,0x02,                       // TP2 A~D RP2
					0x00,0x00,0x00,0x00,0x00,                       // TP3 A~D RP3
					0x00,0x00,0x00,0x00,0x00,                       // TP4 A~D RP4
					0x00,0x00,0x00,0x00,0x00,                       // TP5 A~D RP5
					0x00,0x00,0x00,0x00,0x00 };                     // TP6 A~D RP6
					
					
	static const PROGMEM uint8_t 
		partial[] = {	0x00,0x00,0x00,0x00,0x00,0x00,0x00,             //LUT0: BB:     VS 0 ~7
						0x80,0x00,0x00,0x00,0x00,0x00,0x00,             //LUT1: BW:     VS 0 ~7
						0x40,0x00,0x00,0x00,0x00,0x00,0x00,             //LUT2: WB:     VS 0 ~7
						0x00,0x00,0x00,0x00,0x00,0x00,0x00,             //LUT3: WW:     VS 0 ~7
						0x00,0x00,0x00,0x00,0x00,0x00,0x00,             //LUT4: VCOM:   VS 0 ~7
						0x0A,0x00,0x00,0x00,0x00,                       // TP0 A~D RP0
						0x00,0x00,0x00,0x00,0x00,                       // TP1 A~D RP1
						0x00,0x00,0x00,0x00,0x00,                       // TP2 A~D RP2
						0x00,0x00,0x00,0x00,0x00,                       // TP3 A~D RP3
						0x00,0x00,0x00,0x00,0x00,                       // TP4 A~D RP4
						0x00,0x00,0x00,0x00,0x00,                       // TP5 A~D RP5
						0x00,0x00,0x00,0x00,0x00 };                     // TP6 A~D RP6
					
	const uint8_t *ptr;
	uint8_t size;
	if( m_fullUpdated )
	{
		ptr = full;
		size = sizeof( full );
	}
	else
	{
		ptr = partial;
		size = sizeof( partial );
	}

    sendCommand( WRITE_LUT_REGISTER );
	for( uint8_t i = 0; i < size; i ++ )
	{
		sendData( pgm_read_byte( &ptr[ i ] ) );
	}
}

//
//
void Display_2in13v2::spiTransfer( uint8_t data )
{
    m_cs->activate();
    SPI.transfer( data );
    m_cs->deactivate();
}

//
//
void Display_2in13v2::sendCommand( uint8_t command )
{
    m_dc->deactivate();
    spiTransfer( command );
}

//
//
void Display_2in13v2::sendData( uint8_t data )
{
    m_dc->activate();
    spiTransfer( data );
}

//
//
void Display_2in13v2::clear() 
{
	uint16_t size = ( m_lanscape ? m_width : m_height ) * m_blockSize;
	for( uint8_t loop = 0; loop < 2; loop ++ )
	{
		if( !loop || m_fullUpdated )
		{
			setMemoryPointer( 0, 0 );
			sendCommand( !loop ? WRITE_RAM : WRITE_SHADOW_RAM );
			for( uint16_t i = 0; i < size; i++ )
			{
				sendData( 0xff );
			}
		}
	}
}

//
//
void Display_2in13v2::update() 
{
    sendCommand( DISPLAY_UPDATE_CONTROL_2 );
    sendData( m_fullUpdated ? 0xC7 : 0x0C );
    sendCommand( MASTER_ACTIVATION );
    waitUntilIdle();
}

//
//
void Display_2in13v2::setMemoryPointer( uint8_t x, uint8_t y ) 
{
    sendCommand( SET_RAM_AXIS1_ADDRESS_COUNTER );
    sendData( ( ( m_lanscape ? y : x ) >> 3 ) & 0x3F );
    sendCommand( SET_RAM_AXIS2_ADDRESS_COUNTER );
    sendData( m_lanscape ? x : y );
    sendData( 0 );
    waitUntilIdle();
}

//
//
void Display_2in13v2::copy( Canvas &canvas, uint16_t x, uint16_t y, uint16_t width, uint16_t height )
{
	uint16_t drawn_width = min( width ? width : static_cast< uint16_t >( canvas.width() ), m_width );
	uint16_t drawn_height = min( height ? height : static_cast< uint16_t >( canvas.height() ), m_height );
	uint16_t bytes = ( ( m_lanscape ? drawn_height : drawn_width ) + 7 ) / 8;
	

	for( uint8_t loop = 0; loop < 2; loop++ )
	{
		if( !loop || m_fullUpdated )
		{
			uint8_t cmd = !loop ? WRITE_RAM : WRITE_SHADOW_RAM;
			if( m_lanscape )
			{	//	landscape
				for( uint16_t idx1 = 0; idx1 < drawn_width; idx1++ )
				{
					const uint8_t *image = canvas.image( idx1, 0 );
					setMemoryPointer( m_width - 1 - idx1 - x, y );
					sendCommand( cmd );
					for( uint16_t i = 0; i < bytes; i ++ )
					{
						sendData( *image );
						image++;
					}
				}
			}
			else
			{	//	portrait
				for( uint16_t idx1 = 0; idx1 < drawn_height; idx1++ )
				{
					const uint8_t *image = canvas.image( 0, idx1 );
					//setMemoryPointer( x, m_height - 1 - idx1 - y );
					setMemoryPointer( x, idx1 + y );
					sendCommand( cmd );
					for( uint16_t i = 0; i < bytes; i ++ )
					{
						sendData( *image );
						image++;
					}
				}
			}
		}
	}
}    

}