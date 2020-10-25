
#include <iostream>
#include <conio.h>

using namespace std;

void horas2(int dia2, int hora2, int minuto2, int segundo2)
{
  
	   
     if (dia2  == 0)
     cout << dia2 << "0:";
     
     if (dia2 > 0 && dia2 < 10)
     cout << "0" << dia2  << ":";  
     
     if (dia2 > 9)
     cout << dia2  << ":";

     if (hora2 == 0)
     cout << hora2 << "0:";
     
     if (hora2 > 0 && hora2 < 10)
     cout << "0" << hora2 << ":";  
     
     if (hora2 > 9)
     cout << hora2 << ":";


     if (minuto2 == 0)
     cout << minuto2 << "0:";
     
     if (minuto2 > 0 && minuto2 < 10)
     cout << "0" << minuto2 << ":";  
     
     if (minuto2 > 9)
     cout << minuto2 << ":";
         
     if (segundo2 == 0)
     cout << segundo2 << "0";
     
     if (segundo2 > 0 && segundo2 < 10)
     cout << "0" << segundo2;  
     
     if (segundo2 > 9)
     cout << segundo2;
     
     
     
    
}
void horas(int dia, int hora, int minuto, int segundo)
{
  
	   
     if (dia  == 0)
     cout << dia << "0:";
     
     if (dia > 0 && dia < 10)
     cout << "0" << dia  << ":";  
     
     if (dia > 9)
     cout << dia  << ":";

     if (hora == 0)
     cout << hora << "0:";
     
     if (hora > 0 && hora < 10)
     cout << "0" << hora << ":";  
     
     if (hora > 9)
     cout << hora << ":";


     if (minuto == 0)
     cout << minuto << "0:";
     
     if (minuto > 0 && minuto < 10)
     cout << "0" << minuto << ":";  
     
     if (minuto > 9)
     cout << minuto << ":";
         
     if (segundo == 0)
     cout << segundo << "0";
     
     if (segundo > 0 && segundo < 10)
     cout << "0" << segundo;  
     
     if (segundo > 9)
     cout << segundo;
     
     
     
     
     
     
     
    
}


int main()
{
    int dia;
    int hora,hora2=0;
    int minuto;
    int segundo;
    
 
    
 
    int comfirma;
    int atraso=0,atraso2=0;
    int i, j;
    
    cout << "Digite o dia: ";
    cin >> dia;
    
    if (dia < 0 || dia > 31)
    cout << "Erro";
    
    cout << "Digite a hora: ";
    cin >> hora;
    
    if (hora < 0 || hora > 24)
    cout << "Erro";

    
    cout << "Digite os minutos: ";
    cin >> minuto;
    
    if (minuto < 0 || minuto > 59)
    cout << "Erro";

    
    cout << "Digite os segundos: ";
    cin >> segundo;

    if (segundo < 0 || segundo > 59)
    cout << "Erro";



		printf("\nPasse o cartao:");
    	scanf("%d", &comfirma);   	
    	
    	if(hora <8 && comfirma == 1)
    	{
		
			printf("\nBem vindo");
			getch();
			
		}

		
		if(hora >=8 && comfirma == 1)
		{
			
			hora2=hora-8;
			
			printf("\nVoce esta Atrasado\n");
			printf("\nHH:MM:SS\n");
			
			if(hora2 == 0)
   			cout << hora2 << "0:";
     
   			if(hora2 > 0 && hora2 < 10)
     		cout << "0" << hora2 << ":";  
     
   			if(hora2 > 9)
   			cout << hora2 << ":";
   			  
   			if(minuto == 0)
  			cout << minuto << "0:";
     
    		if(minuto > 0 && minuto < 10)
    		cout << "0" << minuto << ":";  
     
   			if(minuto > 9)
    		cout << minuto << ":";
         
    		if(segundo == 0)
    		cout << segundo << "0";
     
     		if(segundo > 0 && segundo < 10)
   			  cout << "0" << segundo;  
    		 
    		if(segundo > 9)
   			  cout << segundo;
     
			getch();
			
		}
		
		if(comfirma == 0)
		{
			printf("Nao possui cadastro");
			getch();
		}  

			

	
    system("cls");
    horas(dia,hora, minuto, segundo);
    
    while (segundo < 60)
    {
          segundo++;
          system("cls");
          horas(dia,hora, minuto, segundo);
          
          for (i = 1; i<=800; i++)
              for (j = 1; j<=400000; j++)
          
          if (segundo == 60)
          {
               system("cls"); 
                
               minuto++;
               segundo = 0;
               

              if (minuto == 60)
              {
                hora++;
                minuto = 0;
                
                if (hora == 24)
                {
                  dia++;	
                  hora = 0;
                  minuto = 0;
                }
                 	if (dia > 31)
                	{
                 		dia = 0;	
                		hora = 0;
                		minuto = 0;
               		}
                
                
                
                
                else 
				{
					
               	 	system("cls");
                	horas(dia,hora, minuto, segundo);
                }
              }        
              
              system("cls");
              
              horas(dia,hora, minuto, segundo);
              for (i = 1; i <= 800; i++)
              for (j = 1; j <= 400000; j++);
                      
                      
          }
          
    
    }
 
 
    system("pause");
    
    return 0;  
}

