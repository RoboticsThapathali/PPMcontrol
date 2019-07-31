/*
 * Author -> Rhimesh Lwagun 
 *
 *
 *    t0   t1   t2      t0   t1 ....
 *     _    _    _       _    _  
 * ___| |__| |__| |_____| |__| |___
 *    |----|----| 
 *      ch1  ch2 ->rising edge or see falling edge
 *      
 * this is for synchronization (while booting or by some delays)
 * 
 * if(channel value > 3000)
 * t[0] = lastpulsetime
 * set pulse 1
 * 
 * PCINT -> normal interrupt can be use as interrupt for tha reason 
 * PCINT -> D8
 */

#define TIMEGAP 500
 unsigned long ch[9], t[10]; //for 8 channel 1-> stop/start
int pulse=0,last;

void setup() {
  PCICR |= (1<<PCIE0); //interrupt enable
  PCMSK0 |= (1<<PCINT0); //interrupt pin 0 -> D2
  Serial.begin(115200);
}
 
void loop() {
  // put your main code here, to run repeatedly:
for(int i = 0 ; i<9;i++)
  Serial.print(i+1),Serial.print(">"),Serial.print(ch[i]),Serial.print("\t");
 Serial.println();
 delay(100);

}

ISR(PCINT0_vect){
  if(PINB & B00000001){
    if(micros()-last < TIMEGAP){
      last = micros();
      return ;
    }
    t[pulse] = micros();
    if(pulse > 0||pulse == 1)
    {
      ch[pulse-1] = t[pulse]- t[pulse-1];
      if(ch[pulse-1]>3000){
        t[0] = t[pulse];
        pulse = 0;
      }
    }
    pulse++;
  }
}

