
//Bibliotecas
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

//Pinos
#define LED_VERDE 13
#define LED_VERMELHO 12
#define BUZZER 33
#define SS_PIN 53
#define RST_PIN 5

String IDtag = ""; //Variável que armazenará o ID da Tag

boolean TagAtu = false;

boolean TagAnt = false;

bool Permitido = false; //Variável que verifica a permissão


int segundo=50,minuto=0, hora=19, dia=11, mes=3,ano=2019,aux,aux2; 

int contadorEnt = 0,contadorSai = 1;


//Vetor responsável por armazenar os ID's das Tag's cadastradas
const int MAX_TAGS = 4;
String TagsCadastradas[MAX_TAGS] = {"ad53e975", "441c67dc", "2ce69ed4", "80111a4"};
String Nomes[MAX_TAGS] = {"Rodrigo", "Luiz", "Carlos", "Bus   "};

MFRC522 RFID(SS_PIN, RST_PIN);    // Cria uma nova instância para o leitor e passa os pinos como parâmetro

LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

Servo myservo;

unsigned UtlTime;

// --- Configurações Iniciais ---
void setup() {

  lcd.begin(16, 2);              //Inicializa LCD 16 x 2

  myservo.attach(48);
  Serial.begin(9600);             // Inicializa a comunicação Serial
  SPI.begin();                    // Inicializa comunicacao SPI
  RFID.PCD_Init();          // Inicializa o leitor RFID
  pinMode(LED_VERDE, OUTPUT);     // Declara o pino do led verde como saída
  pinMode(LED_VERMELHO, OUTPUT);  // Declara o pino do led vermelho como saída
  pinMode(BUZZER, OUTPUT);        // Declara o pino do buzzer como saída

  Serial.println("Aproxime a tag do leitor...");
  Serial.println();

}

void loop() {

  Leitura();  //Chama a função responsável por fazer a leitura das Tag's

  dataehora();

  delay(3000);
  
  lcd.setCursor(0, 0);                                //Posiciona cursor na coluna 1, linha 1
  lcd.print("Aproxime a tag    ");                    //Imprime mensagem
  lcd.setCursor(0, 1);                                //Posiciona cursor na coluna 1, linha 2
  lcd.print("do leitor...      ");                    //Imprime mensagem
  myservo.write (20);                                  // posição inicial do servo

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
  for (byte i = 0; i < RFID.uid.size; i++) {
    IDtag.concat(String(RFID.uid.uidByte[i], HEX));
  }


  for (int idx = 0; idx < MAX_TAGS; idx++)
  {
    if (IDtag.equalsIgnoreCase(TagsCadastradas[idx])) 
    {
      lcd.setCursor(0, 0);
      lcd.print(Nomes[idx] + " " + IDtag);           // Apresenta o nome e a tag caso cadastrada
      lcd.setCursor(0, 1);
      lcd.print("Acesso Liberado");
      Serial.println(Nomes[idx] + " " + IDtag);
      Serial.println("Acesso Liberado");
      myservo.write ( 120 );                          // Libera o acesso 

      Permitido = true; //Variável Permitido assume valor verdadeiro caso o ID Lido esteja cadastrado
    }
  }

  if (Permitido == true) 
  {
    acessoLiberado(); //Se a variável Permitido for verdadeira será chamada a função acessoLiberado()
  }
  else 
  {
    acessoNegado(); //Se não será chamada a função acessoNegado()
  }

  delay(3000); //aguarda 3 segundos para efetuar uma nova leitura
}

void acessoLiberado() 
{
  Serial.println("Tag Cadastrada: " + IDtag); //Exibe a mensagem "Tag Cadastrada" e o ID da tag não cadastrada
  Serial.println("");

  valida();//Valida entrada e saida


  efeitoPermitido();  //Chama a função efeitoPermitido()
  Permitido = false;  //Seta a variável Permitido como false novamente

}

void acessoNegado() 
{
  Serial.println("Tag NAO Cadastrada: " + IDtag); //Exibe a mensagem "Tag NAO Cadastrada" e o ID da tag cadastrada
  Serial.println("Verifique com a Secretaria");
  Serial.println("");

  efeitoNegado(); //Chama a função efeitoNegado()

  lcd.setCursor(0, 0);
  lcd.print("Nao Cadastrada: " + IDtag);
  lcd.setCursor(0, 1);
  lcd.print("Verifique");
}

void efeitoPermitido()
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
}

void efeitoNegado()
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
}
void valida()//Codigo que valida entrada e saida 
{
  
   if (hora>=14)
  {
    contadorEnt = 0;
  } 
    
    
  if (hora>= 0 && hora <14)
  {
    
    TagAtu = RFID.PICC_ReadCardSerial();

    
    if( TagAtu && !TagAnt)
    {
    contadorEnt++;
      
      if(contadorEnt > 1 && hora <14)
      {
        
 
          lcd.clear();
          lcd.print("Entrada ja");
          lcd.setCursor(0,1);
          lcd.print("Registrada!!!");
          delay(2000);
          lcd.clear();
          contadorEnt = 2;

      }
   
  
        
    }

   TagAnt = TagAtu;
      
  }
  
 
  if(hora >18)
  {
    contadorSai = 0; 
  }
    
   if (hora>= 14 && hora <=18)
  {
    
    
     
    TagAtu =  RFID.PICC_ReadCardSerial();

 
     
    if(TagAtu && !TagAnt )
    {
        
         contadorSai++;
    
      
      if(contadorSai> 2 && hora >=14)
      {
   
 
          lcd.clear();
          lcd.print("Saida ja");
          lcd.setCursor(0,1);
          lcd.print("Registrada!!!");
          delay(2000);
          lcd.clear();
          contadorSai = 3;  

      }
 
        
    }

    TagAnt = TagAtu;
  }
  

   if( contadorEnt == 1)
   {  
   
    if(hora <8 && RFID.PICC_ReadCardSerial() == 1)
    {
    
        lcd.clear();
        lcd.print("Bem vindo");
        lcd.setCursor(0,1);
        delay(2000);
        lcd.clear(); 
       
       
  } 
  
    if((hora == 8 && minuto <=15) &&  RFID.PICC_ReadCardSerial() == 1)
    {  
      lcd.clear();
      lcd.print("Bem vindo");
      delay(2000);
      lcd.clear();
    }
      
 
    if((hora >=8 && hora <14)&&  RFID.PICC_ReadCardSerial() == 1)
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
     if((hora >= 14 && hora <=18 ) &&  RFID.PICC_ReadCardSerial() == 1)
    {
      
      
        lcd.clear();
        lcd.print("Saida");
        lcd.setCursor(0,1); 
        delay(2000);
        lcd.clear();
     
    }  


    if((hora >= 19 && hora <=23 ) && RFID.PICC_ReadCardSerial() == 1)
    {
    lcd.clear();
    lcd.print("Sem operacao");
    lcd.setCursor(0,1); 
    delay(2000);
    lcd.clear();
    } 


    
    
    
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
    
} 

void dataehora()//Mostra data e hora no lcd
{

    codigoDataeHora();
     
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
    lcd.print(" T"); 
    lcd.print(contadorEnt); 
  
  
  
}
