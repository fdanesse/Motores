
byte pins[] = {9, 10, 11};
int paso = 0;


const int motorPinSteps[3][9]={
    {1,1,1,1,0,0,0,0,0},
    {1,0,0,0,0,0,1,1,1},
    {0,0,0,1,1,1,1,0,0}
};


void setup(){
    for(int x=0; x<3; x++) {
        pinMode(pins[x], OUTPUT);
    }
    //Serial.begin(9600);
}

void loop(){
    for(int x=0; x<3; x++) {
        digitalWrite(pins[x], motorPinSteps[x][paso]); 
    }
    paso++; if (paso > 8){paso = 0;}
    delay(5);
}
