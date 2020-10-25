#!/usr/bin/python
# -*- coding: utf-8 -*-
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
    print("Tag %s Usuario %s " %(TagOut,Nome))

    cursor.execute("select *from Registro where entrada is null and id = %d and data = CURDATE()"%(ID))
    resultado4 = cursor.fetchall()
    
    time.sleep(1)
    
    if resultado4:
        
        print("Entrada:%s" %EntradaOut)
        #Comando para inserir os dados no MYSQL
        sql = "update Registro set entrada = '%s' where entrada is null and id = %d and data = CURDATE()" %(EntradaOut,ID)

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
    
   
    

    
    cursor.execute("select *from Registro where adiant is null and id = %d  and data = CURDATE()" %(ID))
    resultado2 = cursor.fetchall()

    if resultado2:

        print("Adiantado:%s" %adiantadoOut)    
        sql2 = "update Registro set adiant = '%s' where adiant is null and id = %d and data = CURDATE()" %(adiantadoOut,ID)
        cursor.execute(sql2)
        db.commit()
         
       


def valida2():

    
    atraso = ser.readline().decode().split('\r\n')#Mesma coisa

    atrasoOut = atraso[0]
    
    time.sleep(1)
    
   

    cursor.execute("select *from Registro where atraso is null and id = %d and data = CURDATE()" %(ID))
    resultado3 = cursor.fetchall()

    if resultado3:

                print("Atrasado:%s"%atrasoOut) 
                sql3 = "update Registro set atraso = '%s' where atraso is null and id = %d and data = CURDATE()" %(atrasoOut,ID)
                cursor.execute(sql3)
                db.commit()
                
               
def valida3():

                    
        Saida = ser.readline().decode().split('\r\n')#Mesma coisa

        SaidaOut = Saida[0]
                
        time.sleep(1)
      
               
        cursor.execute("select *from Registro where saida is null and id = %d and data = CURDATE()"%(ID))
        resultado4 = cursor.fetchall()

        if resultado4:

            print("Saida:%s"%SaidaOut) 
            sql5 = "update Registro set saida = '%s' where saida is null and id = %d and data = CURDATE()" %(SaidaOut,ID)
            cursor.execute(sql5)
            db.commit()
            
 

    

while 1:#laço


           hora = ser.readline()#Recebe a hora do arduino

           hora = int(hora) 
      
                
           if hora >= 0 and hora < 14:

               Tag = ser.readline().decode().split('\r\n') #A variavel tag contem o conteudo da variavel 'data', que o arduino enviou atraves da comunicação serial    
               TagOut = Tag[0]#ser.readline vai ler a linha que foi enviada

               cursor.execute("select ID,Nome,Tag from Cadastro where Tag = '%s' " %(TagOut))
               resultado9 = cursor.fetchall()

               if resultado9:

                      for row in resultado9:
                              
                           ID = row[0];
                           ID = int(ID)

                           Nome = row[1]
                                    
                           Tag2 = row[2]
                                 
                           print("ID:%d \nNome:%s \nTag:%s" %(ID,Nome,Tag2))
             
       
                           cursor.execute("select nome from Registro where id = %d and data = CURDATE() and entrada is not null"%(ID))
                           resultado = cursor.fetchall()
                       

                           if resultado:

                                verifVolta =  b'1'

                                ser.write(verifVolta)

                                time.sleep(1)
                                

                                print("Entrada ja registrada")
                                

                                      
                           else:
                               
                                sql = "INSERT INTO Registro(data,id,nome,entrada,adiant,atraso,saida) VALUES(CURDATE(),%d,'%s',NULL,NULL,NULL,NULL)" %(ID,Nome)
                                #Cursor ira executar o comando de cima para inserir os dados    
                                cursor.execute(sql)

                                #Ocorre uma  atualização dos dados no MYSQL
                                db.commit()
                                
                                verifVolta =  b'0'
                                          
                                ser.write(verifVolta)
                                
                                print("Entrada Enviada")

                                time.sleep(1)
                                
                                ser.write(Nome.encode())
                                
                                time.sleep(3)
    
                                
                                insereDados()


            

               else:

                   print("Tag sem Cadastro")


     

           if hora >= 14 and hora < 19:

               Tag = ser.readline().decode().split('\r\n') #A variavel tag contem o conteudo da variavel 'data', que o arduino enviou atraves da comunicação serial    
               TagOut = Tag[0]#ser.readline vai ler a linha que foi enviada

               cursor.execute("select *from Cadastro where Tag = '%s' " %(TagOut))
               resultado5 = cursor.fetchall()

               if resultado5:

                        for row in resultado5:
                              
                                ID = row[0];
                                ID = int(ID)

                                Nome = row[1]
                                            
                                Tag2 = row[2]
                                         
                                print("ID:%d \nNome:%s \nTag:%s" %(ID,Nome,Tag2))
                     
                                    
                                
                                    

                                cursor.execute("select nome from Registro where data = CURDATE() and id = %d and saida is not null" %(ID))
                                resultado8 = cursor.fetchall()

                                if resultado8:
                                    
                                    verifVolta =  b'1'

                                    ser.write(verifVolta)

                                    time.sleep(1)

                                    print("Saida ja registrada")



                              
                                else:
                                    
                                    verifVolta =  b'0'
                                                  
                                    ser.write(verifVolta)

                                    print("Saida Enviada")

                                    time.sleep(1)

                                    ser.write(Nome.encode())
                                
                                    time.sleep(3)

                                    valida3() 


      

               else:

                 print("Tag sem Cadastro")   
               
    
 

                








           
            

          
          

          
      

        


              
    

              
              
          



          
     
              



  
