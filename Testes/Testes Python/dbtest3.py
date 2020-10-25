import serial #Inclui a comunicação entre o python e arduino
import time#Inclui o tempo do delay entre arduino e python
import MySQLdb#Inclui a comunicação entre o python e o mysql

db = MySQLdb.connect("localhost","root","123456","teste2")#Adiciona endereco do rasp,o usuario do mysql,a senha do mysql,e o banco de dados criado

ser = serial.Serial('/dev/ttyUSB0',9600)#Adiciona o a entrada da comunicação e o tempo padrão do arduino



cursor = db.cursor()#Variavel que executa as funções do mysql


def insereDados():
    

    #Apos a validação dos dados e a execução do codigo,o arduino era reiniciar, e apos passar tag, a tag, o nome e entrada do usuario serão printados
    print("Tag %s registrada Usuario %s registrado entrada %s " %(TagOut,NomeOut,EntradaOut))
     


    #Comando para inserir os dados no MYSQL
    sql = "INSERT INTO Entrada(data,nome,tag,presenca) VALUES(NOW(),'%s','%s','%s')" %(NomeOut,TagOut,EntradaOut)

          


    #Cursor ira executar o comando de cima para inserir os dados    
    cursor.execute(sql)

    #Ocorre uma  atualização dos dados no MYSQL
    db.commit()     





             
         
    


while 1:#laço

    
    Tag = ser.readline().decode().split('\r\n') #A variavel tag contem o conteudo da variavel 'data', que o arduino enviou atraves da comunicação serial    
    TagOut = Tag[0]#ser.readline vai ler a linha que foi enviada,e depois ira decodificar a sujeira que seria o \r(espaço) e \n(quebra de linha)

  
    Nome = ser.readline().decode().split('\r\n')#Mesma coisa 

    NomeOut = Nome[0]

  
    Entrada = ser.readline().decode().split('\r\n')#Mesma coisa

    EntradaOut = Entrada[0]




    cursor.execute("select nome from Entrada where tag = '%s' and NOW() + 1 and presenca is not null" %(TagOut))
    resultado = cursor.fetchall()







    if resultado:

         
            print("Entrada ja registrada")

        
        
    else:
        
          insereDados()
    

  
        
        

              
              
          



          
       
              



  
