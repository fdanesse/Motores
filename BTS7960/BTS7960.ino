// BTS7960 5v - 27v

int SENSOR_PIN = 0; // A0 pin central del potenciometro
int sensorValue = 0;

int RPWM_Output = 5; // Arduino PWM output pin 5; connect to IBT-2 pin 1 (RPWM)
int LPWM_Output = 6; // Arduino PWM output pin 6; connect to IBT-2 pin 2 (LPWM)


void setup(){
    pinMode(RPWM_Output, OUTPUT);
    pinMode(LPWM_Output, OUTPUT);
}


void controlMotor(){
    int val = analogRead(SENSOR_PIN);

    if (sensorValue != val){
        sensorValue = val;
        int pwm = 0;
        if (sensorValue < 512){
            // reverse rotation
            pwm = -(sensorValue - 511) / 2;
            analogWrite(LPWM_Output, 0);
            analogWrite(RPWM_Output, pwm);
        }
        else{
            // forward rotation
            pwm = (sensorValue - 512) / 2;
            analogWrite(LPWM_Output, pwm);
            analogWrite(RPWM_Output, 0);
        }
    }  
}


void loop(){
    controlMotor();
}
