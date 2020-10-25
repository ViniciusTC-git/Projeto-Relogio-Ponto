import serial #Inclui a comunicação entre o python e arduino
import time#Inclui o tempo do delay entre arduino e python
import MySQLdb#Inclui a comunicação entre o python e o mysql

db = MySQLdb.connect("localhost","root","123456","banco")#Adiciona endereco do rasp,o usuario do mysql,a senha do mysql,e o banco de dados criado

ser = serial.Serial('/dev/ttyUSB0',9600)#Adiciona o a entrada da comunicação e o tempo padrão do arduino



cursor = db.cursor()#Variavel que executa as funções do mysql


  
   


def insereDados():


    Entrada = ser.readline().decode().split('\r\n')#Mesma coisa

    EntradaOut = Entrada[0]
  
     
    #Apos a validação dos dados e a execução do codigo,o arduino era reiniciar, e apos passar tag, a tag, o nome e entrada do usuario serão printados
    print("Tag %s Usuario %s registrada entrada %s  " %(TagOut,Nome,EntradaOut))

    #Comando para inserir os dados no MYSQL
    sql = "INSERT INTO Registro(data,nome,tag,entrada,adiant,atraso,saida) VALUES(CURDATE(),'%s','%s','%s',NULL,NULL,NULL)" %(Nome,TagOut,EntradaOut)


    #Cursor ira executar o comando de cima para inserir os dados    
    cursor.execute(sql)

    #Ocorre uma  atualização dos dados no MYSQL
    db.commit()
  

    check = ser.readline().decode().split('\r\n')#Mesma coisa
    checkOut = check[0]

    time.sleep(1)


    if checkOut == 'n':#Se o dado enviado pelo arduino for igual a n
            
         valida()#adiantado


    if checkOut == 's':#Se o dado enviado pelo arduino for igual a s
            
        valida2()#atrasado



  
def valida():
    
    adiantado = ser.readline().decode().split('\r\n')#Mesma coisa

    adiantadoOut = adiantado[0]
    
    time.sleep(1)
    
   
    

    
    cursor.execute("select *from Registro where adiant is null and tag = '%s'  and data = CURDATE()" %(TagOut))
    resultado2 = cursor.fetchall()

    if resultado2:

        print("Adiantado:%s" %adiantadoOut)    
        sql2 = "update Registro set adiant = '%s' where adiant is null and tag = '%s' and data = CURDATE()" %(adiantadoOut,TagOut)
        cursor.execute(sql2)
        db.commit()
         
       


def valida2():

    
    atraso = ser.readline().decode().split('\r\n')#Mesma coisa

    atrasoOut = atraso[0]
    
    time.sleep(1)
    
   

    cursor.execute("select *from Registro where atraso is null and tag = '%s' and data = CURDATE()" %(TagOut))
    resultado3 = cursor.fetchall()

    if resultado3:

                print("Atrasado:%s"%atrasoOut) 
                sql3 = "update Registro set atraso = '%s' where atraso is null and tag = '%s' and data = CURDATE()" %(atrasoOut,TagOut)
                cursor.execute(sql3)
                db.commit()
                
               
def valida3():

                    
        Saida = ser.readline().decode().split('\r\n')#Mesma coisa

        SaidaOut = Saida[0]
                
        time.sleep(1)
      
               
        cursor.execute("select *from Registro where saida is null and tag = '%s' and data = CURDATE()"%(TagOut))
        resultado4 = cursor.fetchall()

        if resultado4:

            print("Saida:%s"%SaidaOut) 
            sql5 = "update Registro set saida = '%s' where saida is null and tag = '%s'  and data = CURDATE()" %(SaidaOut,TagOut)
            cursor.execute(sql5)
            db.commit()
            
 

    

while 1:#laço


           hora = ser.readline()#Recebe a hora do arduino

           hora = int(hora) 
      
                
           if hora >= 0 and hora < 14:

               Tag = ser.readline().decode().split('\r\n') #A variavel tag contem o conteudo da variavel 'data', que o arduino enviou atraves da comunicação serial    
               TagOut = Tag[0]#ser.readline vai ler a linha que foi enviada

               cursor.execute("select *from Cadastro where tag = '%s' " %(TagOut))
               resultado9 = cursor.fetchall()

               if resultado9:
                   
                   cursor.execute("select *from Registro where data = CURDATE() and entrada is null")
                   resultado = cursor.fetchall()
               

                   if resultado:

                        verifVolta =  b'0'
                                  
                        ser.write(verifVolta)

                        cursor.execute("select nome from Cadastro where tag = '%s' " %(TagOut))
                        resultado5 = cursor.fetchall()

                        if resultado5:
                        
                            Nome = row[0];
                            
                            ser.write(Nome.encode())
                        
                            #continua...
                       
    
                        print("Entrada Enviada")

                        time.sleep(1)

                        insereDados()



                              
                   else:

                        verifVolta =  b'1'

                        ser.write(verifVolta)

                        time.sleep(1)

                        print("Entrada ja registrada")


            

               else:

                   print("Tag sem Cadastro")


     

           if hora >= 14 and hora <= 18:

               Tag = ser.readline().decode().split('\r\n') #A variavel tag contem o conteudo da variavel 'data', que o arduino enviou atraves da comunicação serial    
               TagOut = Tag[0]#ser.readline vai ler a linha que foi enviada

               cursor.execute("select *from Cadastro where tag = '%s' " %(TagOut))
               resultado5 = cursor.fetchall()

               if resultado5:

                   cursor.execute("select *from Registro where data = CURDATE() and saida is null")
                   resultado8 = cursor.fetchall()
               

                   if resultado8:

                        verifVolta =  b'0'
                                  
                        ser.write(verifVolta)

                        cursor.execute("select nome from Cadastro where tag = '%s' " %(TagOut))
                        resultado5 = cursor.fetchall()

                        if resultado5:
                        
                            Nome = row[0];
                            
                            ser.write(Nome.encode())
                        

                        print("Saida Enviada")

                        time.sleep(1)

                        valida3()   



                              
                   else:

                        verifVolta =  b'1'

                        ser.write(verifVolta)

                        time.sleep(1)

                        print("Saida ja registrada")

      

                  
               
           else:

                print("Tag sem Cadastro")     
 

                








           
            

          
          

          
      

        


              
    

              
              
          



          
     
              



  
