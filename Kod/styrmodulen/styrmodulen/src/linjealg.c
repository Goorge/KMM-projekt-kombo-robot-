
signed char linje_RGBsveng();
int linje_get_error();
bool detect_goal();
bool detect_labyrint();
bool sekvens_goal_detekted();

int fel_antal=0;


void linje_main() //funktion so  sk�ter linjef�jlning och hantering av specialfall
{
	if(distans_fram<10){
		start=0; // k�r inte in i v�ggar (v�rkar som sensor fram ger minimum 30)
	}
	else if((RGB_data==1) | (RGB_data==2) | (RGB_data==3)){ // == r�d,gr�n,bl�
		current_position=linje_RGBsveng();
		linje();
	}
	else if(detect_goal()==true){
		start=0;
		//signalera i m�l och stanna
	}
	else if(detect_labyrint()==true){
		regulator_mode=2; //byt till k�r i labyrintmode(�r 2 r�tt eller ska det vara 0)
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
	if(time>20){ // f�rdig med sv�ng f�rhopningsvis
		RGB_data=0;
		time=0;
	}
	else // forts�tt med RGB sv�ng
		time++;
	if(RGB_data==1) //sv�ng h�ger reglera bara p� sensor mest till h�ger
	{
		for(int i=0;i<11;i++)
		{
			if (styr_fel==0)
				if(i < 8){ //f�rsta 8 sensorenrna
					styr_fel= (i-5) * ((Reflex_data_tmp >> (i * 2) & 0x02)>>1);
				}
				else{ // sista 3 sensorerna
					styr_fel= (i-5) * ((Reflex_data2_tmp >> ((i-8) * 2) & 0x02)>>1);
				}
		}
	}
	else if(RGB_data==2) //raktfram
	{
		Reflex_data = Reflex_data_tmp & 0xfff0; //ignorera yttersta sensorer
		Reflex_data2 = Reflex_data2_tmp & 0x03;
		return linje_get_error();
	}
	else if(RGB_data == 3){// sv�ng hv�nster reglera bara p� sensor mest till v�nster
		for(int i=11;i>0;i--){
			if (styr_fel == 0)
				if(i < 8){ //f�rsta 8 sensorenrna
					styr_fel= (i-5) * ((Reflex_data_tmp >> (i * 2) & 0x02)>>1);
				}
				else{ // sista 3 sensorerna
					styr_fel= (i-5) * ((Reflex_data2_tmp >> ((i-8) * 2) & 0x02)>>1);
				}
		}
	}
	else
		return 0x00; //n�got har blivit fel hoppas p� att det l�ser sig 
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
	}
}

bool sekvens_goal_detekted(){
	int fel_antal=0;
	int min=11;
	int max=0;
	if(fel_antal>28)// om robbot paserar tejp p� tv�ren 
		return true;
	else
		return false;
	
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
	if(distans_left<25 && distans_left<25){ // om det finns v�ggar p� b�da sidor
		if(time>20)//har v�ntat slutsats det h�r �r en labyrint 			
			return true;
		else{// v�nta s� det inte bara var n�got litet 
			time++;
			return false;
		}
	}
	else{ // det var ingen labyrint
		time = 0;
		return false;
	}
}