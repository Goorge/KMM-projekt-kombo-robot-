
signed char linje_RGBsveng();
int linje_get_error();
bool detect_goal();
bool detect_labyrint();
bool sekvens_goal_detekted();

int fel_antal=0;
int RGB_reset_timer = 0;
int Lab_reset_timer = 0;
int Goal_reset_timer = 0;
int RGB_slow = 1;
int RGB_force = 0;

void linje_main() //funktion so  sk�ter linjef�jlning och hantering av specialfall
{
	RGB_slow = 1;
	/*if(distans_fram<34){
		start=0; // k�r inte in i v�ggar (v�rkar som sensor fram ger minimum 30)
		PORTD |= (1 << PD1);
		PORTD |= (1 << PD0);
	}
	else*/ if((RGB_data==1) | (RGB_data==2) | (RGB_data==3)){ // == r�d,gr�n,bl�
		current_position=linje_RGBsveng();
		RGB_slow = 2;
		linje();
	}
	else if(detect_goal()==true){
		
		_delay_ms(2500);
		start=0;
		PORTD |= (1 << PD1);
		//signalera i m�l och stanna
	}
	else if(detect_labyrint()==true){
		PORTD |= (1 << PD0);
		start = 0;
		//regulator_mode=0; //byt till k�r i labyrintmode(�r 2 r�tt eller ska det vara 0)
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
	/*if(time>20){ // f�rdig med sv�ng f�rhopningsvis
		RGB_data=0;
		time=0;
	}
	else // forts�tt med RGB sv�ng
		time++;*/
	
	if((RGB_data > 0) && (RGB_reset_timer == 0) && (RGB_force == 0)){  // Detta kommer bara g�ras en g�ng d� vi f�tt n�gon RGB data , r�d/bl�/gr�n
		counter_timer_line_RGB = 0;
		RGB_reset_timer = 1;
		RGB_force = 1;
	}
	else if(counter_timer_line_RGB <= 3){  // 2sekunder ish
		
		if(RGB_data==3){ //sv�ng h�ger reglera bara p� sensor mest till h�ger
			
			for(int i=0;i<11;i++)
			{
				if (styr_fel==0){
					if(i < 8){ //f�rsta 8 sensorenrna
						styr_fel= (i-5) * ((Reflex_data_tmp >> (i * 2) & 0x02)>>1);
					}
					else{ // sista 3 sensorerna
						styr_fel= (i-5) * ((Reflex_data2_tmp >> ((i-8) * 2) & 0x02)>>1);
					}
				}
			}
		styr_fel = -3;
		}
		else if(RGB_data==2){ //raktfram
			
			Reflex_data = Reflex_data_tmp & 0xfff0; //ignorera yttersta sensorer
			Reflex_data2 = Reflex_data2_tmp & 0x03;
			return linje_get_error();
		}
		else if(RGB_data == 1){// sv�ng hv�nster reglera bara p� sensor mest till v�nster
			for(int i=11;i>0;i--){
				if (styr_fel == 0){
					if(i < 8){ //f�rsta 8 sensorenrna
						styr_fel= (i-5) * ((Reflex_data_tmp >> (i * 2) & 0x02)>>1);
					}
					else{ // sista 3 sensorerna
						styr_fel= (i-5) * ((Reflex_data2_tmp >> ((i-8) * 2) & 0x02)>>1);
					}
				}
			}
			styr_fel = 3;
		}
		else{
			return 0x00; //n�got har blivit fel hoppas p� att det l�ser sig
		}
	}
	else{
		RGB_data=0;
		RGB_reset_timer = 0;
		RGB_force = 0;
		
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
	for(int i=0;i<11;i++) //vikta sensorer fr�n -5 till 5 med 0 i mitten 
	{
		if(i < 8){ //f�rsta 8 sensorenrna
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
		styr_fel /= fel_antal; // f� fram medelv�rde av viktade sensorer
	return styr_fel;
}


bool detect_goal(){// brettar om robotten �r i m�l eller inte
		#ifndef time
			static int time;
			static int count;
		#endif
		
	int goal_timer = 20;
	
	if((Goal_reset_timer == 0) && (sekvens_goal_detekted() == true)){		
		Goal_reset_timer = 1;	
		counter_timer_line_goal = 0;																						// Nollar timern som sitter i timerintrerruptet, ISR f�r timern sker 10ggr per sekund						
	}	
	if((sekvens_goal_detekted() == true) && ((count == 0) | (count == 2)) && (counter_timer_line_goal < goal_timer)){		//linje 1(count=0) eller 2(count=2) uppt�kt procid
		count++;
		return false;
	}
	else if((sekvens_goal_detekted() == false) && ((count == 1) | (count == 3)) && (counter_timer_line_goal < goal_timer)){	// mellanrum mellan linje 1-2(count=1) eller 2-3(count=3) uppt�kt
		count++;
		return false;																		
	}
	else if((sekvens_goal_detekted() == true) && (count == 4)  && (counter_timer_line_goal < goal_timer)){					//linje 3 uppt�kt indikerar m�l
		Goal_reset_timer = 0;
		return true;
	}
	else if(counter_timer_line_goal >= goal_timer){																			// Timeout, tiden har passerat. Allts� inget m�l utan bara en T korsning
		count = 0;
		Goal_reset_timer = 0;
		return false;
	}
	// ************************************************************************************************************************'
		
	/*	
	if(sekvens_goal_detekted() == true && ((count == 0) | (count == 2))){ //linje 1(count=0) eller 2(count=2) uppt�kt procid
			count++;
			time++;
			return false;
		}

	else if(sekvens_goal_detekted() == false && ((count == 1) | (count == 3))){// mellanrum mellan linje 1-2(count=1) eller 2-3(count=3) uppt�kt
			count++;
			time++;
			return false;
	}
	else if(sekvens_goal_detekted() == true && count == 4){ //linje 3 uppt�kt indikerar m�l
		return true;
	}
	else if(time<10){	// ingen f�r�ndring r�kna upp tid
		time++;
		return false;
	}
	else{	// time out detta �r inte m�l
		time=0;
		count=0;
		return false;
	}*/
}

bool sekvens_goal_detekted(){
	//int fel_antal=0;
	linje_get_error();		// Borde uppdatera fel_antal
	if(fel_antal>28){		// om robbot paserar tejp p� tv�ren 
		PORTD |= (1 << PD1);
		return true;
	}
	else{
		return false;
	}
	//orginal m�l tre paralela linjer
	/*for(int i=0;i<11;i++)
	{
		if(Reflex_data>>(i*2)&0x02>0 & min>i)
		min=i;
		if(Reflex_data>>(i*2)&0x02>0 & max<i)
		max=i;
		if(Reflex_data>>(i*2)&0x02>0)
		fel_antal++;
	}
	if(fel_antal>3 & fel_antal < 7 & max-min > 5 & max-min < 8) // m�ste kalibreras b�r nog vara && ?
		return true;
	return false;
	*/
}

bool detect_labyrint(){ //om robboten har kommmit in i en labyrint 
		#ifndef time
			static int time;
		#endif
		
	if(distans_left<25 && distans_right<25){ // om det finns v�ggar p� b�da sidor     
		
		if(Lab_reset_timer == 0){
			Lab_reset_timer = 1;
			counter_timer_line_lab = 0;
		}
		else if(counter_timer_line_lab >=30){  // 1 sekund borde vara lagom
			Lab_reset_timer = 0;
			return true;
		}
		else{
			return false;
		}
		/*
		if(time>20)//har v�ntat slutsats det h�r �r en labyrint 			
			return true;
		else{// v�nta s� det inte bara var n�got litet 
			time++;
			return false;
		}*/
	}
	else{ // det var ingen labyrint
		time = 0;
		return false;
	}
}