// https://www.airspayce.com/mikem/arduino/RadioHead/classRH__ASK.html
#include <RH_ASK.h>
#ifdef RH_HAVE_HARDWARE_SPI
#include <SPI.h>
#endif

// Default settings (Speed = 2000bps; RX pin = 11; TX pin = 12; Ptt pin = 10) RX pin (receiver) - TX pin (transmitter)
RH_ASK driver(2000, 11, 12, -1);

struct Data {
    int motor = 0;
    int x = 0;
    int y = 0;
};

Data data;


#define SENSOR_PIN A0 // A0 pin central del potenciometro
int sensorValue = 0;

#define RPWM_Output 5 // BTS7960 5v - 27v - Arduino PWM output pin 5; connect to IBT-2 pin 1 (RPWM)
#define LPWM_Output 6 // BTS7960 5v - 27v - Arduino PWM output pin 6; connect to IBT-2 pin 2 (LPWM)

/*
#define trig 10 // Ultrasonido HC-SR04
#define eco 11 // Ultrasonido HC-SR04
int duracion;
int distancia;
*/

#include<ServoTimer2.h>
//ServoTimer2 motor;

ServoTimer2 servo;


void setup(){
    digitalWrite(LED_BUILTIN, LOW);
    
    pinMode(RPWM_Output, OUTPUT);
    pinMode(LPWM_Output, OUTPUT);

    //pinMode(trig, OUTPUT);
    //digitalWrite(trig, LOW);

    // Aeromodelismo
    //pinMode(9, OUTPUT);
    //motor.attach(9);
    //Activar el motor
    //motor.write(1000);
    //delay(5000);

    pinMode(8, OUTPUT);
    servo.attach(8);
    servo.write(90);
    
    Serial.begin(9600);
    Serial.println(F("Iniciado"));

    // Radio Frecuencia
    if (driver.init()){digitalWrite (LED_BUILTIN, LOW);}
    else{Serial.println(F("ERROR Radiofrecuencia"));}
}

/*
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
*/

void controlMotor(){
    int val = analogRead(SENSOR_PIN); // 0-1023
    data.motor = val;
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

/*
void motor_aeromodelismo(){
    int vel = analogRead(SENSOR_PIN);  //potentiometer (value between 0 and 1023)
    vel = map(vel, 0, 1023, 1000, 2100);  // funciona de 1500 a 2100
    data.motor = vel;
    motor.write(vel);  
}
*/

void controlServo(){
    int grados = analogRead(SENSOR_PIN);  //potentiometer (value between 0 and 1023)
    grados = constrain(map(grados, 0, 1023, 0, 180), 0, 180);  // funciona de 1500 a 2100
    servo.write(grados);
    //Serial.println(grados);
}


void loop(){
    controlMotor();
    //motor_aeromodelismo();
    controlServo();
    //medir();
    
    emitir();
    delay(100);
}

void emitir() {
    bool envio = driver.send((uint8_t*)&data, sizeof(struct Data));
    driver.waitPacketSent();
    if (envio) {
        Serial.println(data.motor);
    }
}
