
signed char linje_RGBsveng();
int linje_get_error();
bool detect_goal();
bool detect_labyrint();
bool sekvens_goal_detekted();

int fel_antal=0;


void linje_main()
{
	//Reflex_data=Reflex_data & 0xffffff;
	if(distans_fram<10){
		start=0; // kör inte in i väggar (värkar som sensor fram ger minimum 30)
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
		
	}
}

signed char linje_RGBsveng() //om RGB ger utslag
{
	signed char styr_fel=0x00;

	#ifndef time
		static int time;
	#endif
	if(time>20){ // färdig med sväng förhopningsvis
		RGB_data=0;
		time=0;
	}
	else // fortsätt med RGB sväng
		time++;
	if(RGB_data==1) //sväng höger reglera bara på sensor mest till höger
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
	else if(RGB_data == 3){// sväng hvänster reglera bara på sensor mest till vänster
		for(int i=11;i>0;i--){
			if (styr_fel == 0)
				styr_fel= (i-5) * Reflex_data >> (i * 2) & 0x02;
		}
	}
	else
		return 0x00;
	return styr_fel;
}



int linje_get_error(){
	float styr_fel=0;
	fel_antal=0;
	cli();
	int Reflex_data_tmp = Reflex_data;
	int Reflex_data2_tmp = Reflex_data2;
	sei();
	for(int i=0;i<11;i++) //vikta sensorer från -5 till 5 med 0 i mitten 
	{
		if(i < 8){
			styr_fel += ((5-i) * ((Reflex_data_tmp>>(i*2))&3));
			fel_antal += ((Reflex_data_tmp>>(i*2))&0x3);
		}
		else{
			styr_fel += ((5-i) * ((Reflex_data2_tmp>>((i-8)*2))&3));
			fel_antal += ((Reflex_data2_tmp>>((i-8)*2))&0x3);
		}
	}
	if (fel_antal == 0)
		styr_fel = 0; //kan inte dela med 0 specialfall
	else
		styr_fel /= fel_antal; // få fram medelvärde av viktade sensorer
	return styr_fel;
}


bool detect_goal(){
		#ifndef time
			static int time;
			static int count;
		#endif
	if(sekvens_goal_detekted() == true && ((count == 0) | (count == 2))){ //en linje eller 2 upptäkta procide
			count++;
			time++;
			return false;
		}

	else if(sekvens_goal_detekted() == false && ((count == 1) | (count == 3))){// mellanrum mellan linje 1-2 eller 2-3 upptäkt
			count++;
			time++;
			return false;
	}
	else if(sekvens_goal_detekted() == true && count == 4){ //linje 3 upptäkt indikerar mål
		return true;
	}
	else if(time<10){	// ingen förändring räkna upp tid
		time++;
		return false;
	}
	else{	// time out detta är inte mål
		time=0;
		count=0;
		return false;
	}
}

bool sekvens_goal_detekted(){
	int fel_antal=0;
	int min=11;
	int max=0;
	if(fel_antal>20)
		return true;
	else
		return false;
	
	//orginal mål tre paralela linjer
	/*for(int i=0;i<11;i++)
	{
		if(Reflex_data>>(i*2)&0x02>0 & min>i)
		min=i;
		if(Reflex_data>>(i*2)&0x02>0 & max<i)
		max=i;
		if(Reflex_data>>(i*2)&0x02>0)
		fel_antal++;
	}
	if(fel_antal>3 & fel_antal < 7 & max-min > 5 & max-min < 8) // måste kalibreras bör nog vara && ?
		return true;
	return false;
	*/
}

bool detect_labyrint(){
		#ifndef time
			static int time;
		#endif
	if(distans_left<25 && distans_left<25){ // borde kalibreras
		if(time>20)
			return true;
		else{
			time++;
			return false;
		}
	}
	else{
		time = 0;
		return false;
	}
}