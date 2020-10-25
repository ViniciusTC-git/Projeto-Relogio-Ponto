       
//Bibliotecas
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//Pinos

#define SS_PIN 10
#define RST_PIN 9

String IDtag = ""; //Variável que armazenará o ID da Tag


boolean Permitido = false;


int segundo=0,minuto=0, hora=9, dia=11, mes=3,ano=2019,aux,aux2,aux3,aux4; 


void codigoDataeHora();

void dataehora();

void valida();


int contadorEnt = 0,contadorSai = 1;


String TagAtu = "";//Entrada

String TagAnt = "";

String TagAtu2 = "";//Saida

String TagAnt2 = "";


//Vetor responsável por armazenar os ID's das Tag's cadastradas
const int MAX_TAGS = 4;
String TagsCadastradas[MAX_TAGS] = {"21c02b1e", "f923c55a", "2ce69ed4", "80111a4"};
String Nomes[MAX_TAGS] = {"Luis", "Carlos", "Salve", "Bus   "};


String data = "";//ARMAZENA ID DA TAG
String data2 = "";//ARMAZENA O NOME

int Segundodata;
int Minutodata;
int Horadata;




MFRC522 RFID(SS_PIN, RST_PIN);    // Cria uma nova instância para o leitor e passa os pinos como parâmetro

LiquidCrystal_I2C lcd(0x27,40,4); 




// --- Configurações Iniciais ---
void setup() 
{


  lcd.begin(16, 2);              //Inicializa LCD 4x 2
  lcd.init();                 // Inicializando o LCD
  lcd.backlight();  

  Serial.begin(9600);             // Inicializa a comunicação Serial
  SPI.begin();                    // Inicializa comunicacao SPI
  RFID.PCD_Init();          // Inicializa o leitor RFID


}

void loop() {




  dataehora();//Mostra data e hora no lcd
  
  Leitura();  //Chama a função responsável por fazer a leitura das Tag's
 
  

  
}

void Leitura()
{

  IDtag = ""; //Inicialmente IDtag deve estar vazia.

  // Verifica se existe uma Tag presente
  if ( !RFID.PICC_IsNewCardPresent() || !RFID.PICC_ReadCardSerial() ) {
    delay(150);
    return;
  }


  // Pega o ID da Tag através da função LeitorRFID.uid e Armazena o ID na variável IDtag
  for (byte i = 0; i < RFID.uid.size; i++) 
  {
    IDtag.concat(String(RFID.uid.uidByte[i], HEX));
  }
  
  

   for (int i = 0; i < MAX_TAGS; i++)
    {
      if (IDtag.equalsIgnoreCase(TagsCadastradas[i])) 
      {
      
        valida();
        
        delay(2000);
        lcd.clear();

        Permitido = true; //Variável Permitido assume valor verdadeiro caso o ID Lido estejacadastrado
      }
    }

      if (Permitido == true) 
      {  
        Permitido = false; 
        
      }
      else 
      {
        lcd.clear();
        lcd.print("      TAG");
        lcd.setCursor(0,1); 
        lcd.print("  SEM CADASTRO");
        delay(2000);
        lcd.clear();
      }

 
}



/*void efeitoPermitido()
{
  int qtd_bips = 2; //definindo a quantidade de bips
  for (int j = 0; j < qtd_bips; j++) {
    //Ligando o buzzer com uma frequência de 1500 hz e ligando o led verde.
    tone(BUZZER, 1500);
    digitalWrite(LED_VERDE, HIGH);
    delay(100);

    //Desligando o buzzer e led verde.
    noTone(BUZZER);
    digitalWrite(LED_VERDE, LOW);
    delay(100);
  }
}*/

/*void efeitoNegado()
{
  int qtd_bips = 1;  //definindo a quantidade de bips
  for (int j = 0; j < qtd_bips; j++) {
    //Ligando o buzzer com uma frequência de 500 hz e ligando o led vermelho.
    tone(BUZZER, 500);
    digitalWrite(LED_VERMELHO, HIGH);
    delay(500);

    //Desligando o buzzer e o led vermelho.
    noTone(BUZZER);
    digitalWrite(LED_VERMELHO, LOW);
    delay(500);

  }
}*/

void valida()//Codigo que valida entrada e saida 
{
  
  
  if (hora>=14)
  {
    contadorEnt = 0;
    
  } 
  
   
  if (hora>= 0 && hora <14)
  {

      

     TagAtu = IDtag;

      
  
    
    if( TagAtu.endsWith(TagAnt))
    {
     contadorEnt++;


        
      
        if(contadorEnt > 1 && hora <14 )
        {
          
       
            lcd.clear();
            lcd.print("Entrada ja");
            lcd.setCursor(0,1);
            lcd.print("Registrada!!!");
            delay(2000);
            lcd.clear();
            contadorEnt = 2;
         

      }

    
      
      
      
      TagAnt =  TagAtu;



    }


       if(IDtag!=TagAnt)
       {
          TagAnt = "";
          TagAtu = "";
          contadorEnt=1;
        
        }
    
  
    
    
      
  }




 
  if(hora >18)
  {
    contadorSai = 0; 
    
  }
    
   if (hora>= 14 && hora <=18)
  {
   
    
     
    TagAtu2 = IDtag;
       
  
     
     
    if(TagAtu2.endsWith(TagAnt2))
    {
        
         contadorSai++;
    
      
      if(contadorSai > 2 && hora >=14)
      {
   
          
          lcd.clear();
          lcd.print("Saida ja");
          lcd.setCursor(0,1);
          lcd.print("Registrada!!!");
          delay(2000);
          lcd.clear();
          contadorSai = 3;  

      }
 
         TagAnt2 = TagAtu2;
         
    }


  if(IDtag!=TagAnt2)
  {
    TagAnt2 = "";
    TagAtu2 = "";
    contadorSai=2;
    
  }


  

  }

  

  

   if(contadorEnt == 1)
   {  
 
     enviaDados();
   
    if(hora <8 && contadorEnt == 1)
    {
    
        aux3=59-minuto;
      
        aux2=7-hora;
      
        aux4=59-segundo;

      
    for (int i = 0; i < MAX_TAGS; i++)
    {
      if (IDtag.equalsIgnoreCase(TagsCadastradas[i])) 
      {
        
        lcd.clear();
        lcd.print("Bem vindo " + Nomes[i]);
        lcd.setCursor(0,1);

       
        
      }
    }
        lcd.print("Adiant");
        lcd.print("+");
        if(aux2 < 10)
        {
            lcd.print("0");

        } 
        lcd.print(aux2);
        lcd.print(":");
        if(aux3 < 10)
        {
            lcd.print("0");

        } 
        lcd.print(aux3);
        lcd.print(":");
        if(aux4 < 10)
        {
            lcd.print("0");

        } 
        lcd.print(aux4);
      
      
      
      
        delay(4000);
        lcd.clear();     
       
  } 
      if((hora == 8) && contadorEnt == 1)
    {  
      
    
    for (int i = 0; i< MAX_TAGS; i++)
    {
      if (IDtag.equalsIgnoreCase(TagsCadastradas[i])) 
      {
        lcd.clear(); 
        lcd.print("Bem vindo "+ Nomes[i]);
   
            
      }
    }  
      
      delay(2000);
      lcd.clear();
    }
      
 
    if((hora >=8 && hora <14) && contadorEnt == 1)
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


  
 }
 
  
  if(contadorSai == 2)
  {
     if((hora >= 14 && hora <=18 ) && contadorSai == 2)
  {
      
      
    lcd.clear();
    lcd.print("Saida"); 
    for (int i= 0; i < MAX_TAGS; i++)
    {
      if (IDtag.equalsIgnoreCase(TagsCadastradas[i])) 
      {
        lcd.setCursor(0,1);
        lcd.print("Flw " +Nomes[i]); 
               
        
      }
    }
    delay(2000);
    lcd.clear();
     
    }  
    
    
    
  }
  
    if(hora >= 19 && hora <=23)
   {
    lcd.clear();
    lcd.print("Sem operacao");
    lcd.setCursor(0,1); 
    delay(2000);
    lcd.clear();
    } 
  




  
  
}

void codigoDataeHora()//Relogio teste
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

segundo=0;
minuto++;
  
 
  
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
    
}  




void dataehora()//Mostra data e hora no lcd
{
  
  codigoDataeHora();

    

lcd.setCursor(0,0);   
lcd.print("Data ");   
lcd.print(dia);   
lcd.print("/");   
lcd.print(mes);   
lcd.print("/");   
lcd.print(ano);    
      
  
lcd.setCursor(0,1);   
lcd.print("Hora "); 
if(hora < 10)
{
  lcd.print("0");
  
}  
lcd.print(hora);   
lcd.print(":");
if(minuto < 10)
{
  lcd.print("0");
  
}   
lcd.print(minuto);   
lcd.print(":");
if(segundo < 10)
{
  lcd.print("0");
  
}   
  
lcd.print(segundo);
lcd.print("T");
lcd.print(contadorSai);


}
void enviaDados()
{
    for (int i= 0; i < MAX_TAGS; i++)
      {
        if (IDtag.equalsIgnoreCase(TagsCadastradas[i])) 
        {

            data = TagsCadastradas[i];//data pega a tag
            data2 = Nomes[i];//data2 pega o nome

            
            Serial.println(data);//ENVIA a tag para o python
          
            Serial.println(data2);//ENVIA o nome para o python

            Minutodata=minuto;//A variavel minutodata pega o conteudo de minuto
            Segundodata=segundo;//A variavel segundodata pega o conteudo de segundo
            Horadata=hora;//A variavel horadata pega o conteudo da hora


            
            if(Horadata < 10)
            {

                Serial.print("0");//Acrescenta um 0 do lado da variavel horadata
            }
            

            Serial.println((String)Horadata+':'+(String)Minutodata+':'+(String)Segundodata);//ENVIA o horario que o usuario bateu o cartão para o python


       


        }

      }

  
}

 


