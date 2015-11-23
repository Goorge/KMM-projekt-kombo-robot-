void led_test();
void linje_setup();
signed char linje_RGBsveng();	
signed char linje_get_error();
byte RGB_data;
char Reflex_data_raw[3];
int Reflex_data;
//Reflex_data=Reflex_data_raw[2]*0x10000+Reflex_data_raw[1]*0x100+Reflex_data_raw[0]; // göras då data tas emot

void led_test(){
	DDRD =1<<PD1;
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
	if(RGB_data=="röd") //sväng höger
	{
		for(int i=0;i<11;i++)
		{
			if (styr_fel==0)
				styr_fel=(i-5)*Reflex_data>>(i*2)&0x2;
		}	
	}
	else if(RGB_data=="grön") //raktfram
	{
		Reflex_data=Reflex_data & 0x03fff0; //ignorera yttersta sensorer
		return linje_get_error();
	}	
	else if(RGB_data=="blå")// sväng hvänster
	{
		for(int i=11;i>0;i--)
		{
			if (styr_fel==0)
				styr_fel=(i-5)*Reflex_data>>(i*2)&0x2;
		}
	}
	else
		return 0x00;
	return styr_fel;
}	

signed char linje_get_error()
{
	signed char styr_fel=0;
	int fel_antal=0;
	
	//styr_fel=Reflex_data[2]>>6; //behöver bättre algoritm
	for(int i=0;i<11;i++)
	{
		styr_fel=styr_fel+(i-5)*Reflex_data>>(i*2)&0x3;
		fel_antal += Reflex_data>>(i*2)&0x3;
	}
	styr_fel = styr_fel/fel_antal;
	return styr_fel;
}
