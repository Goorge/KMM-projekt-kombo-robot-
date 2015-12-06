
signed char linje_RGBsveng();
int linje_get_error();
bool detect_goal();
bool detect_labyrint();
bool sekvens_goal_detekted();

int fel_antal=0;
int RGB_reset_timer = 0;
int Lab_reset_timer = 0;

void linje_main() //funktion so  sköter linjeföjlning och hantering av specialfall
{
	if(distans_fram<34){
		start=0; // kör inte in i väggar (värkar som sensor fram ger minimum 30)
		PORTD |= (1 << PD1);
		PORTD |= (1 << PD0);
	}
	else if((RGB_data==1) | (RGB_data==2) | (RGB_data==3)){ // == röd,grön,blå
		current_position=linje_RGBsveng();
		linje();
	}
	else if(detect_goal()==true){
		start=0;
		//PORTD |= (1 << PD1);
		//signalera i mål och stanna
	}
	else if(detect_labyrint()==true){
		PORTD |= (1 << PD0);
		start = 0;
		//regulator_mode=0; //byt till kör i labyrintmode(är 2 rätt eller ska det vara 0)
	}
	else{
		current_position=linje_get_error();
		linje();
	}
	
}

signed char linje_RGBsveng() //om RGB ger utslag
{
	cli();
	signed char styr_fel=0x00;
	int Reflex_data_tmp = Reflex_data;
	int Reflex_data2_tmp = Reflex_data2;
	sei();
	
	#ifndef time
		static int time;
	#endif
	/*if(time>20){ // färdig med sväng förhopningsvis
		RGB_data=0;
		time=0;
	}
	else // fortsätt med RGB sväng
		time++;*/
	
	if((RGB_data > 0) && (RGB_reset_timer == 0)){  // Detta kommer bara göras en gång då vi fått någon RGB data , röd/blå/grön
		counter_timer_line = 0;
		RGB_reset_timer = 1;
	}
	else if(counter_timer_line <= 20){  // 2sekunder ish
		
		if(RGB_data==1){ //sväng höger reglera bara på sensor mest till höger
			
			for(int i=0;i<11;i++)
			{
				if (styr_fel==0)
				if(i < 8){ //första 8 sensorenrna
					styr_fel= (i-5) * ((Reflex_data_tmp >> (i * 2) & 0x02)>>1);
				}
				else{ // sista 3 sensorerna
					styr_fel= (i-5) * ((Reflex_data2_tmp >> ((i-8) * 2) & 0x02)>>1);
				}
			}
		}
		else if(RGB_data==2){ //raktfram
			
			Reflex_data = Reflex_data_tmp & 0xfff0; //ignorera yttersta sensorer
			Reflex_data2 = Reflex_data2_tmp & 0x03;
			return linje_get_error();
		}
		else if(RGB_data == 3){// sväng hvänster reglera bara på sensor mest till vänster
			for(int i=11;i>0;i--){
				if (styr_fel == 0)
				if(i < 8){ //första 8 sensorenrna
					styr_fel= (i-5) * ((Reflex_data_tmp >> (i * 2) & 0x02)>>1);
				}
				else{ // sista 3 sensorerna
					styr_fel= (i-5) * ((Reflex_data2_tmp >> ((i-8) * 2) & 0x02)>>1);
				}
			}
		}
		else{
		return 0x00; //något har blivit fel hoppas på att det löser sig
		}
	}
	else{
		RGB_reset_timer = 0;
	}
	 
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
		if(i < 8){ //första 8 sensorenrna
			styr_fel += ((5-i) * ((Reflex_data_tmp>>(i*2))&3));
			fel_antal += ((Reflex_data_tmp>>(i*2))&0x3);
		}
		else{ // sista 3 sensorerna
			styr_fel += ((5-i) * ((Reflex_data2_tmp>>((i-8)*2))&3));
			fel_antal += ((Reflex_data2_tmp>>((i-8)*2))&0x3);
		}
	}
	if (fel_antal == 0)
		styr_fel = 0; //kan inte dela med 0, specialfall
	else
		styr_fel /= fel_antal; // få fram medelvärde av viktade sensorer
	return styr_fel;
}


bool detect_goal(){// brettar om robotten är i mål eller inte
		#ifndef time
			static int time;
			static int count;
		#endif
	// ***************************************Ny mål algorithm, kan funka mabe***********************************************'	
	if(sekvens_goal_detekted() == true && (count == 0)){			// Kollar om counter är 0, alltså första indikeringen
		counter_timer_line = 0;										// Nollar timern som sitter i timerintrerruptet, ISR för timern sker 10ggr per sekund
		count++;						
		return false;
	}	
	else if(sekvens_goal_detekted() == true && (count == 1)&& (counter_timer_line < 20)){	// Kontroll görs att timer countern inte passerat tiden X (just nu inom 2sekunder) innan nästa indikering
		count++;
		return false;
	}
	else if(sekvens_goal_detekted() == true && (count == 2)&& (counter_timer_line < 20)){	// Kontroll görs att timer countern inte passerat tiden X (just nu inom 2sekunder) innan nästa indikering
		count = 0;
		return true;																		// Om tre korsningar har passerats inom tiden av X kommer mål indikeras
	}
	else if(counter_timer_line >= 20){								// Timeout, tiden har passerat. Alltså inget mål utan bara en T korsning
		count = 0;
		return false;
	}
	// ************************************************************************************************************************'
		
	/*	
	if(sekvens_goal_detekted() == true && ((count == 0) | (count == 2))){ //linje 1(count=0) eller 2(count=2) upptäkt procid
			count++;
			time++;
			return false;
		}

	else if(sekvens_goal_detekted() == false && ((count == 1) | (count == 3))){// mellanrum mellan linje 1-2(count=1) eller 2-3(count=3) upptäkt
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
	}*/
}

bool sekvens_goal_detekted(){
	int fel_antal=0;
	int min=11;
	int max=0;
	// Goal funkar inte
	//if(fel_antal>28)// om robbot paserar tejp på tvären 
	if(((Reflex_data2 >= 42)|| (Reflex_data2 >= 10)) && ((Reflex_data >= 43690) || (Reflex_data >= 2730))){ // om alla sensorer visar "10" eller högre
		
		return true;
	}
	else
	PORTD ^= (1 << PD1);
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

bool detect_labyrint(){ //om robboten har kommmit in i en labyrint 
		#ifndef time
			static int time;
		#endif
		
	if(distans_left<25 && distans_right<25){ // om det finns väggar på båda sidor     
		
		if(Lab_reset_timer == 0){
			Lab_reset_timer = 1;
			counter_timer_line_lab = 0;
		}
		else if(counter_timer_line_lab >=10){  // 1 sekund borde vara lagom
			Lab_reset_timer = 0;
			return true;
		}
		else{
			return false;
		}
		/*
		if(time>20)//har väntat slutsats det här är en labyrint 			
			return true;
		else{// vänta så det inte bara var något litet 
			time++;
			return false;
		}*/
	}
	else{ // det var ingen labyrint
		time = 0;
		return false;
	}
}