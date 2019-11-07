#include <SPI.h>
#include "epd_display.h"
#include <Arduino.h>

namespace Epd
{

//
//
Display::Display( uint16_t width, uint16_t height, Gpio::Pin *cs, Gpio::Pin *dc, Gpio::Pin *reset, Gpio::Pin *busy ) :
	m_width( width ),
	m_height( height ),
	m_heightBytes( ( height + 7 ) / 8 ),
	m_cs( cs ), 
	m_dc( dc ),
	m_reset( reset ),
	m_busy( busy ),
	m_fullUpdate( true )
{
}

//
//
bool Display::initializePins()
{
	if( ( nullptr == m_cs ) || ( nullptr == m_dc ) )
		return false;

	//	CS
	m_cs->setActiveLow();
	m_cs->setMode( OUTPUT );

	//	DC
	m_dc->setActiveHigh();
	m_dc->setMode( OUTPUT );

	//	Reset
	if( nullptr != m_reset )
	{
		m_reset->setActiveLow();
		m_reset->setMode( OUTPUT );
	}

	//	busy
	if( nullptr != m_busy )
	{
		m_busy->setActiveHigh();
		m_busy->setMode( INPUT );
	}

	return true;
}

//
//
bool Display::begin( bool fullUpdate )
{
	if( ( nullptr == m_cs ) || ( nullptr == m_dc ) )
		return false;

	m_fullUpdate = fullUpdate;
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
    sendData( ( m_heightBytes - 1 ) & 0x3f );
    sendCommand( SET_RAM_AXIS2_ADDRESS_START_END_POSITION );
    sendData( 0 );
    sendData( 0 );
    sendData( ( m_width - 1 ) & 0xff );
    sendData( 0 );

	waitUntilIdle();

	return true;
}

//
//
void Display::reset()
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
void Display::waitUntilIdle() 
{
	if( nullptr == m_busy )
		return;

    while( m_busy->isActive() )
	{
        delay(10);
    }      
}

//
//
void Display::initializeLut()
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
	if( m_fullUpdate )
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
void Display::spiTransfer( uint8_t data )
{
    m_cs->activate();
    SPI.transfer( data );
    m_cs->deactivate();
}

//
//
void Display::sendCommand( uint8_t command )
{
    m_dc->deactivate();
    spiTransfer( command );
}

//
//
void Display::sendData( uint8_t data )
{
    m_dc->activate();
    spiTransfer( data );
}

//
//
void Display::clear() 
{
	uint16_t size = m_width * m_heightBytes;
	for( uint8_t loop = 0; loop < 2; loop ++ )
	{
		if( !loop || m_fullUpdate )
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
void Display::update() 
{
    sendCommand( DISPLAY_UPDATE_CONTROL_2 );
    sendData( m_fullUpdate ? 0xC7 : 0x0C );
    sendCommand( MASTER_ACTIVATION );
    waitUntilIdle();
}

//
//
void Display::setMemoryPointer( uint8_t x, uint8_t y ) 
{
    sendCommand( SET_RAM_AXIS1_ADDRESS_COUNTER );
    sendData( ( y >> 3 ) & 0x3F );
    sendCommand( SET_RAM_AXIS2_ADDRESS_COUNTER );
    sendData( x );
    sendData( 0 );
    waitUntilIdle();
}

//
//
void Display::copy( Canvas &canvas, uint8_t x, uint8_t y, uint8_t width, uint8_t height )
{
	uint8_t drawn_width = min( width ? width : static_cast< uint16_t >( canvas.width() ), m_width );
	uint8_t drawn_height = min( height ? height : static_cast< uint16_t >( canvas.height() ), m_height );
	uint8_t bytes = ( drawn_height + 7 ) / 8;
	
	for( uint8_t loop = 0; loop < 2; loop++ )
	{
		if( !loop || m_fullUpdate )
		{
			uint8_t cmd = !loop ? WRITE_RAM : WRITE_SHADOW_RAM;
			for( uint8_t idx1 = 0; idx1 < drawn_width; idx1++ )
			{
				const uint8_t *image = canvas.image( idx1, 0 );
				setMemoryPointer( m_width - 1 - idx1 - x, y );
				sendCommand( cmd );
				for( uint8_t i = 0; i < bytes; i ++ )
				{
					sendData( *image );
					image++;
				}
			}
		}
	}
	
}    


}