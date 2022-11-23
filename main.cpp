#include "mbed.h"
#include "platform/mbed_thread.h"


DigitalOut Led(LED1);
DigitalOut Trigger(D8);
InterruptIn Echo(D7);

Serial pc(USBTX,USBRX);
Timer t;
Ticker pingT;


int duration, distanceCM = 0;

void ping();
void echoRise();
void echoFall();
void HighSig();
void LowSig();

int main(){
    Echo.rise(&echoRise);
    Echo.fall(&echoFall);
    
    while(1){
        ping();
        
        printf("Distance is: %d cm \r\n",distanceCM);
        if(distanceCM < 100) {
            Led = 1;
            } else {
                Led = 0;
            }
            
            thread_sleep_for(500);
            
        }
    
}

void ping(){
    Trigger = 0;
     pingT.attach_us(&HighSig,2);
     thread_sleep_for(60);
}

void HighSig(){
    pingT.detach();
    Trigger = 1;    
    pingT.attach_us(&LowSig,10);
}

void LowSig(){
    Trigger = 0;
    pingT.detach();
    
}

void echoRise(){
    t.start();
}

void echoFall(){
    t.stop();
    duration = t.read_us();
    distanceCM = (duration *343)/20000;
    t.reset();
}

