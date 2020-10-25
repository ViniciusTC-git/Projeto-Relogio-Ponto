       
//Bibliotecas
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//Pinos

#define SS_PIN 10
#define RST_PIN 9


String IDtag = ""; //Variável que armazenará o ID da Tag


void mostra();

String data = "";//envia o ID da tag



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
  mostra();//Mostra data e hora no lcd



}

void loop() 
{

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

         
          Serial.println(IDtag);
           delay(1000);
          

          
          lcd.clear();
          lcd.print("ID:"+IDtag);
          delay(4000);
          
  
          lcd.clear();
          mostra();

}

void mostra()
{
  
  
  lcd.print("Insira a Tag");
  

            
}






          
