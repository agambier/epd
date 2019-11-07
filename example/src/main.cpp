#include <Arduino.h>
#include <SPI.h>
#include <gpioarduino.h>
#include <gpiolist.h>

#define GPIO_CS		0
#define GPIO_DC		1
#define GPIO_RESET	2
#define GPIO_BUSY	3
Gpio::List< 4, Gpio::Arduino > g_gpios;

#include "epd_canvasbw.h"
Epd::CanvasBW< 100, 32 > g_canvas;

#include "epd_display.h"
Epd::Display g_display( 250, 122, g_gpios[ GPIO_CS ], g_gpios[ GPIO_DC ], g_gpios[ GPIO_RESET ], g_gpios[ GPIO_BUSY ] );

#include <Fonts/FreeMono9pt7b.h>
const PROGMEM uint8_t whatsapp_bits[] = {
   0x00, 0xf0, 0x0f, 0x00, 0x00, 0xfe, 0x7f, 0x00, 0x80, 0xff, 0xff, 0x01,
   0xc0, 0xff, 0xff, 0x03, 0xe0, 0xff, 0xff, 0x07, 0xf0, 0xff, 0xff, 0x0f,
   0xf8, 0xff, 0xff, 0x1f, 0xfc, 0xf9, 0xff, 0x3f, 0xfc, 0xf0, 0xff, 0x3f,
   0x7e, 0xf0, 0xff, 0x7f, 0x7e, 0xe0, 0xff, 0x7f, 0x7e, 0xe0, 0xff, 0x7f,
   0x7f, 0xe0, 0xff, 0xff, 0x7f, 0xf0, 0xff, 0xff, 0x7f, 0xf0, 0xff, 0xff,
   0xff, 0xf0, 0xff, 0xff, 0xff, 0xe1, 0xff, 0xff, 0xff, 0xc1, 0xff, 0xff,
   0xff, 0x83, 0xdf, 0xff, 0xff, 0x07, 0x0f, 0xff, 0xfe, 0x0f, 0x04, 0x7c,
   0xfe, 0x1f, 0x00, 0x7c, 0xfe, 0x3f, 0x00, 0x7e, 0xfc, 0xff, 0x00, 0x3e,
   0xf8, 0xff, 0x87, 0x3f, 0xf8, 0xff, 0xff, 0x1f, 0xf8, 0xff, 0xff, 0x0f,
   0xfc, 0xff, 0xff, 0x07, 0xfc, 0xff, 0xff, 0x03, 0xfe, 0xff, 0xff, 0x01,
   0x0e, 0xfe, 0x7f, 0x00, 0x07, 0xf0, 0x0f, 0x00 };

void setup() 
{
	// put your setup code here, to run once:
	Serial.begin(115200);
	Serial.println();

    SPI.begin();
    SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));

	g_gpios[ GPIO_CS ]->setPin( 10 );
	g_gpios[ GPIO_DC ]->setPin( 9 );
	g_gpios[ GPIO_RESET ]->setPin( 8 );
	g_gpios[ GPIO_BUSY ]->setPin( 7 );
	g_display.initializePins();
	g_gpios.begin();

	g_display.reset();
	g_display.activateFullUpdate();

	g_display.clear();
	g_canvas.fillScreen( Epd::Canvas::White );
	g_canvas.drawRoundRect( 0, 0, g_canvas.width()-1, g_canvas.height()-1, 5, Epd::Canvas::Black );
	g_canvas.drawFastHLine( 0, 16, g_canvas.width() / 2, Epd::Canvas::Black );
	g_canvas.setCursor( 3, 3 );
	g_canvas.print( "Hello World !" );
	g_display.copy( 0, 0, g_canvas );
	g_display.update();
}

void loop() 
{
}

