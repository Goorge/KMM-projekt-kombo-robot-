
signed char linje_RGBsveng();
int linje_get_error();
bool detect_goal();
bool detect_labyrint();
bool sekvens_goal_detekted();




void linje_main()
{
	//Reflex_data=Reflex_data & 0xffffff;
	current_position=linje_get_error();
	linje();
	/*if(distans_fram<10){
		start=0; // kör inte in i väggar
	}
	else if((RGB_data==1) | (RGB_data==2) | (RGB_data==3)){ // == röd,grön,blå
		current_position=linje_RGBsveng();
		linje();
	}
	else if(detect_goal()==true){
		start=0;
		//signalera i mål och stanna
	}
	else if(detect_labyrint()==true){
		regulator_mode=2; //byt till kör i labyrintmode(är 2 rätt eller ska det vara 0)
	}
	else{
		current_position=linje_get_error();
		linje();
		
	}*/
}

signed char linje_RGBsveng()
{
	signed char styr_fel=0x00;
	static int time;
	if(time>20){
		RGB_data=0;
		time=0;
	}
	else
	time++;
	if(RGB_data==1) //sväng höger
	{
		for(int i=0;i<11;i++)
		{
			if (styr_fel==0)
				if (Reflex_data>>(i*2)&0x3==0x3)
					styr_fel=(i-5)*Reflex_data>>(i*2)&0x3;
		}
	}
	else if(RGB_data==2) //raktfram
	{
		Reflex_data=Reflex_data & 0x03fff0; //ignorera yttersta sensorer
		return linje_get_error();
	}
	else if(RGB_data==3)// sväng hvänster
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

int linje_get_error()
{
	int styr_fel=0;
	int fel_antal=0;
	
	for(int i=0;i<11;i++)
	{
		styr_fel=styr_fel+((5-i)*((Reflex_data>>(i*2))&3));
		fel_antal += ((Reflex_data>>(i*2))&0x3);
	}
	styr_fel = styr_fel/fel_antal;
	return styr_fel;
}


bool detect_goal(){
	static int time;
	if(sekvens_goal_detekted()==true){
		if(time>10)
		return true;
		else{
			time++;
			return false;
		}
	}
	else{
		time=0;
		return false;
	}
}

bool sekvens_goal_detekted(){
	int fel_antal=0;
	int min=11;
	int max=0;
	for(int i=0;i<11;i++)
	{
		if(Reflex_data>>(i*2)&0x02>0 & min>i)
		min=i;
		if(Reflex_data>>(i*2)&0x02>0 & max<i)
		max=i;
		if(Reflex_data>>(i*2)&0x02>0)
		fel_antal++;
	}
	if(fel_antal>3 & fel_antal < 7 & max-min > 5 & max-min < 8) // måste kalibreras
		return true;
	return false;
}

bool detect_labyrint(){
	static int time;
	if(distans_left<40 & distans_left<40){ // borde kalibreras
		if(time>10)
		return true;
		else{
			time++;
			return false;
		}
	}
	else{
		time=0;
		return false;
	}
}