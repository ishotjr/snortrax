/*
 * -----------------------------------------------------------------------------
 *
 * snortrax - track your snoring with LightBlue Bean+
 * 
 * -----------------------------------------------------------------------------
 *
 */

#define DEBUG 0

// pin setup
const int sound_pin = A3;

const int audible = 199;
const int loud = 231;
const int trim = 16;


void setup() {

  // set up sound sensor
  pinMode(sound_pin, INPUT);

#if DEBUG
  // debug via virtual serial
  Serial.begin(57600);
#endif
}

void loop() {

  // measure/average sound    
  long sum = 0;
  byte scale = 3; // exponent of samples to take
  for (int i = 0; i < (1 << scale); i++) {
    sum += analogRead(sound_pin);
    Bean.sleep(10);
  }
  sum >>= scale;

  byte red = 0;
  byte green = 0;
  byte blue = 0;

  int snore = (int) sum;
#if DEBUG
  Serial.print(snore);
#endif

  // trim for ambient sound etc.
  snore -= trim;
#if DEBUG
  Serial.print("->");
  Serial.print(snore);
#endif

  if (snore > loud) {
    // display as red intensity
    snore -= loud;

    // cap at 255
    if (snore > 255) {
      snore = 255;
    }
    red = snore;

    // persist for 1 second to highlight the loud snore
    //Bean.sleep(1000);
  } else if (snore > audible) {
    // display as offset green intensity
    snore -= audible;
    green = snore * (128 / (loud - audible)); // limit to half brightness
  } else {
    // gentle blue
    blue = 7;
  }

  Bean.setLed(red, green, blue);

#if DEBUG
  Serial.print("->");
  Serial.print(snore);
  Serial.print(" [");
  Serial.print(red);
  Serial.print("|");
  Serial.print(green);
  Serial.print("|");
  Serial.print(blue);
  Serial.println("]");
#endif
  
  //Bean.sleep(10);

}
