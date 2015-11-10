void led_test();
void linje_setup();
signed char linje_RGBsveng();	
signed char linje_get_error();
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
	signed char styr_fel=0x00;
	
	if(RGB_data)
		styr_fel=linje_RGBsveng();
	else
		styr_fel=linje_get_error();
	//anropa motorer med styr_fel som argument
	
}

signed char linje_RGBsveng()
{
	signed char styr_fel=0x00;
	if(RGB_data=="röd")
		;
	else if(RGB_data=="grön")
		;
	else //blå
		;
	return styr_fel;
}	

signed char linje_get_error()
{
	signed char styr_fel=0;
	int data=Reflex_data[2]*0x10000+Reflex_data[1]*0x100+Reflex_data[0];
	//styr_fel=Reflex_data[2]>>6; //behöver bättre algoritm
	for(int i=0;i<11;i++)
	{
		styr_fel=styr_fel+(i-5)*data>>(i*2+1)&1;
	}
	return styr_fel;
}
