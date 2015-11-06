void led_test();
void linje_setup();
void linje_RGBsveng();	
byte linje_get_error();
byte RGB_data;
char* Reflex_data;

void led_test(){
	DDRD =0xff;
	PORTD=1<<PD1;
	
	
	
};


void linje_setup()
{

		
}
	
	
void linje_main()
{
	if(RGB_data)
		linje_RGBsveng();
	linje_get_error();
	
	
}

void linje_RGBsveng()
{
	
	
}	