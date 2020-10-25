#include <LiquidCrystal.h>


LiquidCrystal lcd(12, 11, 5, 4, 3, 2);


int segundo,minuto, hora, dia, mes,ano,aux,aux2; 

unsigned UtlTime;

void setup() 
{   
 	pinMode(7,INPUT);
  	pinMode(8,INPUT);
  

segundo=0;
minuto=59;   
hora=18;   
dia=11;   
mes=3;   
ano=2019; 
Serial.begin(9600);   
lcd.begin(16, 2);      



	
}  

void loop() 
{

/*if(millis()-UtlTime<0)   
{     
UtlTime=millis();   
}   
else   
{     
segundo=int((millis()-UtlTime)/1000);   
}  */
  
 segundo++;
 delay(980);
 
if(segundo > 59)   
{  
lcd.clear();
segundo=0;
minuto++;
UtlTime=millis();     
 
  
if(minuto > 59)     
{
  
hora++;       
minuto=0;

  
if(hora > 23)       
{ 
    
dia++;         
hora=0;  
  
if(mes==1||mes==3||mes==5||mes==7||mes==8||mes==10||mes==12)         
{           
if(dia > 31)           
{             
dia=1;             
mes++;             
if(mes >12)             
{               
ano++;               
mes=1;             
}           
}         
}         
else if(mes==2)         
{           
if(ano%400==0)           
{             
if(dia>29)             
{               
dia=1;               
mes++;             
}           
}           
else if((ano%4==0)&&(ano%100!=0))           
{             
if(dia>29)             
{              
dia=1;               
mes++;             
}           
}           
else           
{             
	if(dia>28)             
{               
	dia=1;               
	mes++;             
}           
	}         
	}         
	else         
		{           
			if(dia>30)           
			{             
				dia=1;             
				mes++;           
			}         
		}       
}     
}   
}    
 
Serial.print(dia);   
Serial.print("/");   
Serial.print(mes);   
Serial.print("/");   
Serial.print(ano);   
Serial.println();         

lcd.setCursor(0,0);   
lcd.print("Data ");   
lcd.print(dia);   
lcd.print("/");   
lcd.print(mes);   
lcd.print("/");   
lcd.print(ano);    
     
Serial.print(hora);   
Serial.print(":");   
Serial.print(minuto);   
Serial.print(":");   
Serial.print(segundo);   
Serial.print("\n");   
Serial.println();   
  
lcd.setCursor(0,1);   
lcd.print("Hora ");   
lcd.print(hora);   
lcd.print(":");   
lcd.print(minuto);   
lcd.print(":");   
lcd.print(segundo); 
  
   if(hora <8 && digitalRead(7) == 1)
  {
    lcd.clear();
    lcd.print("Bem vindo");
    delay(2000);
    lcd.clear(); 
  } 
   if((hora == 8 && minuto <=15) && digitalRead(7) == 1)
   {  
    lcd.clear();
    lcd.print("Bem vindo");
    delay(2000);
    lcd.clear();
    }
  
  
  
	if((hora >= 11 && hora < 13 ) && digitalRead(7) == 1)
 	{
   	lcd.clear();
   	lcd.print("EntradaIntervalo");
   	lcd.setCursor(0,1); 
    delay(2000);
    lcd.clear();
  	} 
	if((hora == 13 && minuto <=30)  && digitalRead(7) == 1)
 	{
   	lcd.clear();
   	lcd.print("Saida Intervalo");
   	lcd.setCursor(0,1); 
    delay(2000);
    lcd.clear();
  	} 
  
 	if((hora == 13 && minuto >30)  && digitalRead(7) == 1)
 	{
     
    aux2=minuto-30;
    lcd.clear();
    lcd.print("AtrasoIntervalo");
  	lcd.setCursor(0,1);
    lcd.print("H:M:S: ");   
    lcd.print("0");
    lcd.print(":");
    lcd.print(aux2);
    lcd.print(":");
    lcd.print(segundo);
    delay(2000);
    lcd.clear();
  	} 
  	if((hora >= 14 && hora <=18 ) && digitalRead(7) == 1)
 	{
   	lcd.clear();
   	lcd.print("Saida");
   	lcd.setCursor(0,1); 
    delay(2000);
    lcd.clear();
  	} 
  
    if((hora >= 19 && hora <=23 ) && digitalRead(7) == 1)
 	{
   	lcd.clear();
   	lcd.print("Sem operacao");
   	lcd.setCursor(0,1); 
    delay(2000);
    lcd.clear();
  	} 
  
  	if(hora >=8 && digitalRead(7) == 1)
 	{
    aux=hora-8;
    lcd.clear();
    lcd.print("Atrasado H:M:S");
  	lcd.setCursor(0,1); 
    lcd.print(" ");
    lcd.print(aux);
    lcd.print(":");
    lcd.print(minuto);
    lcd.print(":");
    lcd.print(segundo);
    delay(2000);
    lcd.clear();
   
      
  	} 


  
  if(digitalRead(8) == 1)
  {
    
    lcd.clear();
    lcd.print("      TAG");
  	lcd.setCursor(0,1); 
    lcd.print("  SEM CADASTRO");
    delay(2000);
    lcd.clear();
  }
  
  
  
    
}