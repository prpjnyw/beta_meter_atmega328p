#include <Wire.h>
#define DELAY 1000
#define NUMR 3
#define R1 1500
#define R2 30000

void extint_init(void);
float beta_calc(float vout,float r,float gain);
float beta_average(float beta_arr[]);

volatile unsigned char status = 0;
// const int resist[NUMR] = {24000,56000,100000};
const int resist[NUMR] = {24,56,100};  //kiloOHM
float beta_values_npn[NUMR] = {0,0,0};
volatile char state = 0;
const float op_gain = 1 + R2/R1;
float real_beta_npn = 0;

void setup() {
  Serial.begin(9600);
  DDRC |= (1<<1) | (1<<2) | (1<<3);
  PORTC |= (1<<1) | (1<<2) | (1<<3);
  Wire.begin(0x08);                // join i2c bus with address #0x08
  Wire.onRequest(requestEvent); // register event
}

void loop() {
  float vout;

  PORTC &= ~(1<<1);
  vout = analogRead(A0) * 5.0/1024;
  Serial.print("vout = ");
  Serial.print(vout);
  Serial.print(" ");
  beta_values_npn[0] = beta_calc(vout, resist[0],op_gain);
  Serial.print("case1:");
  Serial.print("beta_npn[0] = ");
  Serial.print(beta_values_npn[0]);
  Serial.println("");
  delay(DELAY);
  PORTC |= (1<<1);
  
  PORTC &= ~(1<<2);
  vout = analogRead(A0) * 5.0/1024;
  Serial.print("vout = ");
  Serial.print(vout);
  Serial.print(" ");
  beta_values_npn[1] = beta_calc(vout, resist[1],op_gain);
  Serial.print("case2:");
  Serial.print("beta_npn[1] = ");
  Serial.print(beta_values_npn[1]);
  Serial.println("");
  delay(DELAY);
  PORTC |= (1<<2);

  PORTC &= ~(1<<3);
  vout = analogRead(A0) * 5.0/1024;
  Serial.print("vout = ");
  Serial.print(vout);
  Serial.print(" ");
  beta_values_npn[2] = beta_calc(vout, resist[2],op_gain);
  Serial.print("case3:");
  Serial.print("beta_npn[2] = ");
  Serial.print(beta_values_npn[2]); 
  Serial.println("");
  delay(DELAY);
  PORTC |= (1<<3);

  real_beta_npn = beta_average(beta_values_npn);
  if (real_beta_npn > 67.90 && real_beta_npn <= 68) {
   real_beta_npn = 68.00;
  }else if(real_beta_npn > 66.90 && real_beta_npn < 67.00){
   real_beta_npn = 67.00;
  }
  Serial.print("beta is ");
  Serial.print(real_beta_npn);
  Serial.println("");
}

void requestEvent() {
  // if (status == 1) {
  //    Wire.write('a');
  // }else {
  //    Wire.write('b');
  
  Wire.write((int)real_beta_npn);
  
}

float beta_calc(float vout,float r,float gain){
  //this fuction for calculating beta value of each resistance value
    // float vc = vout / gain;
    // Serial.println(vc);
    // float ic = (5.0-vout) / 1000.0 ;
    // Serial.println(ic);
    // float ib = (3.10 - 0.7) / r;
    // Serial.println(ib);
    // beta = ic / ib;
    float nem = ((5.0 * 21) -vout) * r;
    // Serial.println(nem);
    float dem = 21 * (5.0 - 0.7);
    // Serial.println(dem);
    float beta = nem / dem;
    // Serial.println(beta);

    return beta;
}

float beta_average(float beta_arr[]){
  float beta_avr = 0;
  for (int i = 0; i < 3; i++) {
      beta_avr += beta_arr[i];
  }
  return beta_avr / 3.0;
}


