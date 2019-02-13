#include <Adafruit_NeoPixel.h>
#include <avr/power.h>
#include <RCSwitch.h>

#define PIN_RX   2 
#define	PIN_EXT  3
#define PIN            7
#define NUMPIXELS      8

void ledred();
void ledgreen();

RCSwitch mySwitch;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(9600);
  pixels.begin();
  mySwitch = RCSwitch();
  digitalWrite( PIN_EXT, 0 );
  pinMode( PIN_EXT, OUTPUT );
  mySwitch.enableReceive(0);
}

int c=0;

void loop() {
	if (mySwitch.available()) {
		int value = mySwitch.getReceivedValue();
		switch (value) {
            case '1':
                ledrgreed();
                break;

            case '2':
                ledred();
                break;

            default:
                break;
         }
    }
    mySwitch.resetAvailable();
}

void ledred(){
	while(1){
	    pixels.setPixelColor(0, pixels.Color(150,0,0));
	    pixels.setPixelColor(1, pixels.Color(150,0,0));
	    pixels.setPixelColor(2, pixels.Color(150,0,0));
	    pixels.setPixelColor(3, pixels.Color(150,0,0));
	    pixels.setPixelColor(4, pixels.Color(150,0,0));
	    pixels.setPixelColor(5, pixels.Color(150,0,0));
	    pixels.setPixelColor(6, pixels.Color(150,0,0));
	    pixels.setPixelColor(7, pixels.Color(150,0,0));
	    pixels.show();
	    delay(50);
	    pixels.setPixelColor(0, pixels.Color(0,0,0));
	    pixels.setPixelColor(1, pixels.Color(0,0,0));
	    pixels.setPixelColor(2, pixels.Color(0,0,0));
	    pixels.setPixelColor(3, pixels.Color(0,0,0));
	    pixels.setPixelColor(4, pixels.Color(0,0,0));
	    pixels.setPixelColor(5, pixels.Color(0,0,0));
	    pixels.setPixelColor(6, pixels.Color(0,0,0));
	    pixels.setPixelColor(7, pixels.Color(0,0,0));
	    delay(50);
    }
}

void ledgreen(){
	int n;
	for(n=8;n>0; n--){
	    pixels.setPixelColor(n, pixels.Color(0,150,0));
	    pixels.show();
	    delay(50);
	    pixels.setPixelColor(n, pixels.Color(0,0,0));
	    delay(50);
	}
	n=8;
}
