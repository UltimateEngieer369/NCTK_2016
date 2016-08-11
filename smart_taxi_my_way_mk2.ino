#define ldr_1 A1
#define ldr_2 A2
int ldr1, ldr2;

byte trig = 3;
byte echo = 2;

byte obs_dist = 40;
int dist;

byte led_ldr = 5;

#define button 7		//change this
#define led 6			//this 2

int black1, black2;
int white1, white2;
int grey1, grey2;

byte a1 = 9, a2 = 8;
byte b1 = 13, b2 = 12;
byte pwma = 10, pwmb = 11;

void setup()
{
	Serial.begin(9600);
	Serial.println("Start :) :) :)");

	delay(200);

	//inputs
	pinMode(ldr_1, INPUT);
	pinMode(ldr_2, INPUT);
	pinMode(button, INPUT_PULLUP);
	
	pinMode(trig, OUTPUT);
	pinMode(echo, INPUT);

	//outputs
	pinMode(a1, OUTPUT);
	pinMode(a2, OUTPUT);
	pinMode(b1, OUTPUT);
	pinMode(b2, OUTPUT);
	pinMode(pwma, OUTPUT);
	pinMode(pwmb, OUTPUT);
	pinMode(led, OUTPUT);
	pinMode(led_ldr, OUTPUT);

	digitalWrite(led_ldr, HIGH);

	Serial.println("Calibration ...... ");

/*		
	motors(255, 255);
	delay(1000);
	motors(255, -255);
	delay(1000);
	motors(-255, 255);
	delay(1000);
	motors(-255, -255);
	delay(1000);

	motors(0, 0);

	delay(100000);
*/	
	while(digitalRead(button) == 1){}		//wait for the button
	

	//black calibration
	for(int i = 0; i<16; i++)
	{
		black1 += analogRead(ldr_1);
		black2 += analogRead(ldr_2);
		digitalWrite(led, HIGH);
		delay(15);
		digitalWrite(led, LOW);
		delay(15);
	}

	black1 = black1 / 16;
	black2 = black2 / 16;

	while(digitalRead(button) == 1){}		//again
	
	//white calibration
	for(int i = 0; i<16; i++)
	{
		white1 += analogRead(ldr_1);
		white2 += analogRead(ldr_2);
		digitalWrite(led, HIGH);
		delay(15);
		digitalWrite(led, LOW);
		delay(15);
	}

	white1 = white1 / 16;
	white2 = white2 / 16;

	//gray for line following
	grey1 = (black1+white1)/2;
	grey2 = (black2+white2)/2;

	Serial.println("Valuse r:");
	Serial.print(white1);
	Serial.print("	");
	Serial.print(white2);
	Serial.print("    ##    ");
	Serial.print(black1);
	Serial.print("	");
	Serial.println(black2);

	delay(300);

	Serial.print(grey1);
	Serial.print("	");
	Serial.println(grey2);
	
	delay(300);

	while(digitalRead(button) == 1)
	{}

	delay(500);

	digitalWrite(led, HIGH);
	delay(50);
	digitalWrite(led, LOW);
	delay(50);
}

void loop()
{
	ldr1 = analogRead(ldr_1);
	ldr2 = analogRead(ldr_2);
	readUltra();

	if(digitalRead(button) == 0)
	{
		while(1)
		{
			digitalWrite(led, HIGH);
			motors(0, 0);
		}
	}

	Serial.print(ldr1);		//remove later
	Serial.print("	");
	Serial.print(ldr2);
	Serial.print("	##	");
	Serial.println(dist);

	if(dist < obs_dist)
	{
		while(dist < obs_dist)
		{
			motors(0, 0);
			readUltra();
		}
	}

	if(ldr1 < grey1)
	{
		if(ldr2 < grey2)
		{
			motors(255, 255);
		}
		else
		{
			motors(0, 255);
		}
	}
	else
	{
		if(ldr2 < grey2)
		{
			motors(255, 0);
		}
		else
		{
			motors(255, 255);
		}
	}

	delay(10);
}

void motors(int m1, int m2)
{
	if(m1 < 0)
	{
		//back
		digitalWrite(a1, HIGH);
		digitalWrite(a2, LOW);
	}
	else if(m1 == 0)
	{
		//stop
		digitalWrite(a1, LOW);
		digitalWrite(a2, LOW);
	}
	else
	{
		//forward
		digitalWrite(a1, LOW);
		digitalWrite(a2, HIGH);
	}

	if(m2 < 0)
	{
		//back
		digitalWrite(b1, HIGH);
		digitalWrite(b2, LOW);
	}
	else if(m2 == 0)
	{
		//stop
		digitalWrite(b1, LOW);
		digitalWrite(b2, LOW);
	}
	else
	{
		//forward
		digitalWrite(b1, LOW);
		digitalWrite(b2, HIGH);
	}

	//pwm speed thingy :)

	analogWrite(pwma, abs(m1));
	analogWrite(pwmb, abs(m2));
}

void readUltra()
{
	digitalWrite(trig, HIGH);
	delayMicroseconds(10);
	digitalWrite(trig, LOW);
	delayMicroseconds(5);

	int us = pulseIn(echo, HIGH, 10000);
	dist = us/2/29;

	if(dist == 0)
	{
		dist = 1023;
	}
	
	Serial.println(dist);
}
