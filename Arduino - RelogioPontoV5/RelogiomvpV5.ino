       
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


int segundo=45,minuto=59, hora=13, dia=11, mes=3,ano=2019,aux; 


void codigoDataeHora();

void dataehora();


int contadorEnt = 0;
int contadorSai = 0;


char verifica='n';



//Vetor responsável por armazenar os ID's das Tag's cadastradas
String Nome="";
char var;


String data = "";//ARMAZENA ID DA TAG
String data2 = "";//ARMAZENA O NOME

int Segundodata;
int Minutodata;
int Horadata;

int Segundodata2;
int Minutodata2;
int Horadata2;


int atrasoData;
int atrasoData2;
int atrasoData3;

char check;


int Adiant1;
int Adiant2;
int Adiant3;

int adiantData;
int adiantData2;
int adiantData3;





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
         if ( !RFID.PICC_IsNewCardPresent() || !RFID.PICC_ReadCardSerial() ) 
         {
             delay(150);
             return;
          }


          // Pega o ID da Tag através da função LeitorRFID.uid e Armazena o ID na variável IDtag
          for (byte i = 0; i < RFID.uid.size; i++) 
          {
            IDtag.concat(String(RFID.uid.uidByte[i], HEX));
          }


         lcd.clear(); 
         lcd.print("Aguarde...");  
         
         if(data != IDtag)//se a tag for diferente da anterior
         {
            //Ocorre uma limpeza nas variaveis
            data = "";
            verifica = 'n';
            contadorEnt = 0;
            contadorSai = 0;
            delay(1000);
        
         }


  
          data = IDtag;//data vai armazenar o id da tag
          
          if(hora>=0 && hora <14 || hora >=14 && hora <=18)//Se a hora estiver entre esse horarios envia os dados para o python
          {
            Serial.println(hora);//Envia a hora atual para o python
            delay(1000);
          
  
            Serial.println(data);//Envia a tag para o python
            delay(1000);
            
          }
   
         
          Permitido = true;
          
  
         if(Serial.available())//Inicia o recebimento de dados
         {


             verifica = Serial.read();//Esperaa  algum valor do python 
             delay(1000);  

             Nome = Serial.readStringUntil('\n');
             delay(1000);    

             
         }

       
            
        if(verifica == '0')//se o dado recebido for igual a 0 envia os dados para o python
        {

          
            
            if (hora>= 0 && hora <14)//se a Hora for igual aos numeros
            {

              
              contadorEnt++;//Contador da entrada começa a conta 


               Entrada();//chama a função que valida as entradas
              
            }   


            if (hora>= 14 && hora < 19)//se a Hora for igual aos numeros
            {
             
              
              contadorSai++;//Contador da saida começa a conta 


               Saida();//chama a função que valida a saida
              
            }  
                               
             
        } 

         

        if(verifica == '1' && hora < 14)//se o conteudo recebido for igual a 1 os dados ja foram enviados
        {

             contadorEnt = 2;
               
             if(contadorEnt == 2)
             {
                   //Imprime no lcd   
                   lcd.clear();
                   lcd.print("Entrada");
                   lcd.setCursor(0,1);
                   lcd.print("Ja Registrada!!");
                   delay(3000);
                   lcd.clear();
                   
             }                    
                  
        }
        else
        {
          contadorEnt=0;//contador da entrada zera
          
        }


        
        if(verifica == '1' && hora > 13)//se o conteudo recebido for igual a 1 os dados ja foram enviados
        {
             contadorSai = 2; 
             
             if(contadorSai == 2)
             {
                   //Imprime no lcd       
                   lcd.clear();
                   lcd.print("Saida");
                   lcd.setCursor(0,1);
                   lcd.print("Ja Registrada!!");
                   delay(3000);
                   lcd.clear();               
             } 


        }
        else
        {
          contadorSai=0;//contador da saida zera
        }


  
          if(Permitido == true)
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





          

       lcd.clear();
       delay(2000);
        
        

 
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

void Entrada()//Função que valida entrada
{

  

   if(contadorEnt == 1)//Se o contador for igual a 1
   {  


      
      Minutodata=minuto;//A variavel minutodata pega o conteudo de minuto
      Segundodata=segundo;//A variavel segundodata pega o conteudo de segundo
      Horadata=hora;//A variavel horadata pega o conteudo da hora
  
      //Envia o horario da entrada do usuario
      Serial.println((String)Horadata+':'+(String)Minutodata+':'+(String)Segundodata);//ENVIA o horario que o usuario bateu o cartão para o python
      
      if(hora <8 && contadorEnt == 1)//Adiantado
      {
  
          Adiant1=7-hora;
      
          Adiant2=59-minuto;
       
          Adiant3=59-segundo;
  
          lcd.clear();
          lcd.print("Bem vindo " + Nome);
          lcd.setCursor(0,1);
     
          //Mostra no lcd o quanto o usuario se adiantou
          lcd.print("Adiant");
          lcd.print("+");
          if(Adiant1 < 10)
          {
              lcd.print("0");
  
          } 
          lcd.print(Adiant1);
          lcd.print(":");
          if(Adiant2 < 10)
          {
              lcd.print("0");
  
          } 
          lcd.print(Adiant2);
          lcd.print(":");
          if(Adiant3< 10)
          {
              lcd.print("0");
  
          } 
          lcd.print(Adiant3);
  
             
          check = 'n';
      
          if(check == 'n')//Se check for igual a 'n'
          {
                
               Serial.println(check);//envia o valor para o python
               delay(1000);
  
               adiantData=Adiant1;//variavel adiantData pega o resultado de adiant1
               adiantData2=Adiant2;//variavel adiantData2 pega o resultado de adiant2
               adiantData3=Adiant3;//variavel adiantData3 pega o resultado de adiant3
        
               if(adiantData < 10)
               {
                  Serial.print("+0");//Bota um + e zero na frente da hora
        
               } 
                
               //Envia os dados de quantas horas,minutos,segundos o usuario se adiantou
               Serial.println((String)adiantData+':'+(String)adiantData2+':'+(String)adiantData3);//ENVIA o horario que o usuario bateu o cartão para o python
           
        
         }
              
          delay(3000);
          lcd.clear();     
         
     } 
    
  
    if((hora == 8) && contadorEnt == 1)
    {  
      
        lcd.clear(); 
        lcd.print("Bem vindo "+ Nome);//Mostra o nome do usuario no lcd 
         
        delay(2000);
        lcd.clear();
    }
      
 
   if((hora >=8 && hora <14) && contadorEnt == 1)//Atraso
   {
      
    //Mostra no lcd o atraso
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

   
    check = 's';
    
      if(check == 's')//se a variavel check for igual a 's'
      {
          
        Serial.println(check);//envia o valor paraa o python
        delay(1000);
         
        atrasoData=aux;//atrasodata pega os valores da conta
        atrasoData2=minuto;//atrasodata2  pega o minuto atual
        atrasoData3=segundo;//atrasodata3 pega o segundo atual
  
  
        if(atrasoData < 10)//se a hora for menor do 10
        {
            Serial.print("0");//Botaa um zero na frente da hora
        } 
          
        //Envia os dados de quantas horas,minutos,segundos o usuario se atrasou
        Serial.println((String)atrasoData+':'+(String)atrasoData2+':'+(String)atrasoData3);//ENVIA o horario que o usuario
          
      }
      

        
        delay(2000);
        lcd.clear();
   
    }
   

  
 }



  
}
void Saida()//Função que valida saida
{
    if(contadorSai == 1)//Se contadorSai for igual a 1 
    {
      
     
    
     if((hora >= 14 && hora <=18 ) && contadorSai == 1)
     {
      
      
        lcd.clear();
        lcd.print("Saida"); 
        lcd.setCursor(0,1);
        lcd.print("Flw "+Nome);//Mostra o nome do usuario no lcd 
                     
     }
        

        

          
        Minutodata2=minuto;//A variavel minutodata pega o conteudo de minuto
        Segundodata2=segundo;//A variavel segundodata pega o conteudo de segundo
        Horadata2=hora;//A variavel horadata pega o conteudo da hora


        //Envia o horario da saida do usuario
        Serial.println((String)Horadata2+':'+(String)Minutodata2+':'+(String)Segundodata2);//ENVIA o horario que o usuario bateu o cartão para o python
        delay(1000);
        
        

        delay(2000);
        lcd.clear();
     
    
    
    
  }
  




  
}

    









void codigoDataeHora()//Função engrenagem da hora e data
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




void dataehora()//Função data e hora no lcd
{
  
  codigoDataeHora();//Engrenagem

    

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



}

  
    
  
 
 

 


 
