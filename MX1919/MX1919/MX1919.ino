// MX1919 2v - 10v

int SENSOR_PIN = 0; // A0 pin central del potenciometro

int D_delante = 4;
int D_atras = 5;

int I_delante = 7;
int I_atras = 6;

int sensorValue = 0;

int trig = 10;
int eco = 11;
int duracion;
int distancia;


void setup(){
    pinMode(D_delante, OUTPUT);
    pinMode(D_atras, OUTPUT);
    pinMode(I_delante, OUTPUT);
    pinMode(I_atras, OUTPUT);

    pinMode(trig, OUTPUT);
    Serial.begin(9600);
}

void avanzar(int sensorValue){
    int pwm = -(sensorValue - 511) / 2;
    analogWrite(D_atras, 0);
    analogWrite(D_delante, pwm);
    analogWrite(I_atras, 0);
    analogWrite(I_delante, pwm);
    Serial.println(pwm);
}

void retroceder(int sensorValue){
    int pwm = (sensorValue - 512) / 2;
    analogWrite(D_atras, pwm);
    analogWrite(D_delante, 0);
    analogWrite(I_atras, pwm);
    analogWrite(I_delante, 0);
    Serial.println(pwm);
}

void detener(){
    analogWrite(D_atras, 0);
    analogWrite(D_delante, 0);
    analogWrite(I_atras, 0);
    analogWrite(I_delante, 0);
}

int medir(){
    digitalWrite(trig, HIGH);
    delay(1);
    digitalWrite(trig, LOW);
    duracion = pulseIn(eco, HIGH);
    distancia = duracion / 58.2;
    Serial.println(distancia);
    return distancia;
}


void loop(){
    /*
    int distancia = medir();
    if (distancia < 30) {
        retroceder(1023);
    }
    else {
        avanzar(0);
    }
    */
    int val = analogRead(SENSOR_PIN);

    if (sensorValue != val){
        sensorValue = val;
        int pwm = 0;
        //Serial.println(val); // 1023-800 min atras izq, 250-0 min delante
        if (sensorValue >= 250 && sensorValue <= 800){
            detener();
          }
        else if (sensorValue < 512){
            avanzar(sensorValue);
        }
        else{
            retroceder(sensorValue);
        }
    }
    
}
