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
	m_cs( cs ), 
	m_dc( dc ),
	m_reset( reset ),
	m_busy( busy ),
	m_fullUpdated( true )
{
}

//
//
bool Display::initializePins()
{
	if( ( nullptr == m_cs ) || ( nullptr == m_dc ) )
		return false;

	//	CS
	m_cs->setMode( OUTPUT );
	m_cs->setActiveLow();
	m_cs->setTriggerMode( Gpio::Pin::Level );
	m_cs->setFilter( true );

	//	DC
	m_dc->setMode( OUTPUT );
	m_dc->setActiveHigh();
	m_dc->setTriggerMode( Gpio::Pin::Level );
	m_dc->setFilter( true );

	//	Reset
	if( nullptr != m_reset )
	{
		m_reset->setMode( OUTPUT );
		m_reset->setActiveLow();
		m_reset->setTriggerMode( Gpio::Pin::Level );
		m_reset->setFilter( false );
	}

	//	busy
	if( nullptr != m_busy )
	{
		m_busy->setMode( INPUT );
		m_busy->setActiveHigh();
		m_busy->setTriggerMode( Gpio::Pin::Level );
		m_busy->setFilter( false );
	}

	return true;
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
void Display::spiTransfer( uint8_t data )
{
//    m_cs->activate();
    SPI.transfer( data );
//    m_cs->deactivate();
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


}