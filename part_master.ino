#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64  


#define OLED_RESET     -1 
Adafruit_SSD1306 OLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void oled_start_display(void);
void oled_display_npn(float value);
void oled_display_pnp(float value);
void extint_init(void);
volatile unsigned char status = 3;

void setup() {

  Wire.begin();
  extint_init();
  Serial.begin(9600);
  if (!OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 allocation failed");
  } else {
    Serial.println("ArdinoAll OLED Start Work !!!");
  }
}

void loop() {
  int c ;
  Wire.requestFrom(0x08, 1);    
  while (Wire.available()) {
    c = Wire.read();
  }

  if(status == 1){
    oled_display_npn(c);

  }else if(status == 2){
    oled_display_pnp(c);

  }else{
    oled_start_display();

  }
  
}


void oled_start_display(void){
  OLED.clearDisplay(); 
  OLED.setTextColor(WHITE, BLACK); 
  OLED.setCursor(0, 0); 
  OLED.setTextSize(2); 
  OLED.println("Select opt"); 

  OLED.setCursor(0, 20);
  OLED.setTextSize(2);
  OLED.println("Left:  npn");

  OLED.setCursor(0, 40);
  OLED.setTextSize(2);
  OLED.println("Right: pnp");

  OLED.display(); 
  delay(2000);
}

void oled_display_npn(float value){
  OLED.clearDisplay(); 
  OLED.setTextColor(WHITE, BLACK); 
  OLED.setCursor(0, 0); 
  OLED.setTextSize(2); 
  
  OLED.println("Beta(npn):"); 
  

  OLED.setCursor(40, 40);
  OLED.setTextSize(2);
  if (value == 66.00) {
    OLED.println("wait!");
  }else {
    OLED.println(value);
  }
  
  OLED.setCursor(20, 40);
  OLED.setTextSize(2);
  OLED.println("=");

  OLED.display(); 
  delay(500);
}

void oled_display_pnp(float value){
  OLED.clearDisplay(); 
  OLED.setTextColor(WHITE, BLACK); 
  OLED.setCursor(0, 0); 
  OLED.setTextSize(2); 
  
  OLED.println("Beta(pnp):"); 
  

  OLED.setCursor(40, 40);
  OLED.setTextSize(2);
  OLED.println("ERROR :(");

  OLED.display(); 
  delay(500);
}

void extint_init(void){
  pinMode(2,INPUT_PULLUP);
  pinMode(3,INPUT_PULLUP);
  EIMSK |= (1<<INT0) | (1<<INT1);
  EICRA |= (1<<ISC11) | (1<<ISC01);
  sei();
}

ISR(INT0_vect){
  status = 1;
}

ISR(INT1_vect){
  status = 2;
}
