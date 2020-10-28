#define SENSOR_PIN A0 // A0 pin central del potenciometro
int sensorValue = 0;

#define RPWM_Output 5 // BTS7960 5v - 27v - Arduino PWM output pin 5; connect to IBT-2 pin 1 (RPWM)
#define LPWM_Output 6 // BTS7960 5v - 27v - Arduino PWM output pin 6; connect to IBT-2 pin 2 (LPWM)

#define trig 10 // Ultrasonido HC-SR04
#define eco 11 // Ultrasonido HC-SR04
int duracion;
int distancia;

#define buz 7


void setup(){
    digitalWrite(13, LOW);
    
    pinMode(RPWM_Output, OUTPUT);
    pinMode(LPWM_Output, OUTPUT);

    pinMode(trig, OUTPUT);
    digitalWrite(trig, LOW);

    pinMode(buz, OUTPUT);
    
    Serial.begin(9600);
}


int medir(){
    // confiabilidad = menos de 84cm | de 2 a 40cm segun datasheet
    // https://www.devacron.com/how-to-connect-a-distance-sensor-sharp-2y0a710f-to-arduino/
    //https://diyprojects.io/hc-sr04-ultrasound-vs-sharp-gp2y0a02yk0f-ir-vl53l0x-laser-solutions-choose-distance-measurement-arduino-raspberrypi/#.X0xN6HYzaEA
    digitalWrite(trig, LOW);
    delayMicroseconds(2);
    
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);
    
    duracion = pulseIn(eco, HIGH);
    distancia = duracion / 59;

    Serial.print(distancia); Serial.println(" cm");
}


void controlMotor(){
    int val = analogRead(SENSOR_PIN); // 0-1023    
    tone(buz, val, 50);
    
    val = constrain(map(val, 0, 1023, -255, 255), -255, 255);
    
    if (sensorValue != val){
        sensorValue = val;
        
        if (val < -50){
            analogWrite(LPWM_Output, 0);
            analogWrite(RPWM_Output, val*-1);
        }
        else if (val > 50){
            analogWrite(LPWM_Output, val);
            analogWrite(RPWM_Output, 0);
        }
        else{
            analogWrite(LPWM_Output, 0);
            analogWrite(RPWM_Output, 0);
        }
    }
}


void loop(){
    controlMotor();
    medir();
    delay(100);
}
