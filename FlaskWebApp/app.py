#!/usr/bin/python
# -*- coding: utf-8 -*-
import serial #Inclui a comunicação entre o python e arduino
import time#Inclui o tempo do delay entre arduino e python
from flask import Flask
import pymysql
from flask import Flask, flash, jsonify,redirect, make_response,url_for, render_template, request, session, abort
from pymysql import cursors
import os
import pdfkit


 



app = Flask(__name__)

db = pymysql.connect(host='localhost',user='root',password='123456',db='banco')


cursor = db.cursor()

cursor1 = db.cursor(cursors.DictCursor)



tag = ''
TagOut = ''
gera =''
globalmes = ''
globalano = ''
globalidenti = ''
block = ''
rota = ''






@app.route('/')
def home():
            
   
            
        return render_template("login.html")

            
 

@app.route('/login', methods=['POST'])

def do_admin_login():
    
   
    if request.method == "POST":
        
       
        nomeadmin =  request.form['Nome']
        senhaadmin =  request.form['Senha']

        cursor.execute("select *from admin where nome = %s and senha= %s ", (nomeadmin, senhaadmin))
        r = cursor.fetchall()

        if r:
            global block
            
            block = 'off'

 
            return main()
           

        else:

            return home()



@app.route('/main')
def main():


        if block == 'off':

                       
                db.commit()

                cursor.execute("SET lc_time_names = 'pt_BR'")
                cursor1.execute("SELECT DISTINCT DATE_FORMAT(CURRENT_DATE,'%W, %d de %M de %Y') from Registro")
                r1 = cursor1.fetchall()
                cursor1.execute("select date_format(data,'%W'),date_format(data,'%d/%m/%Y'),id,nome,STR_TO_DATE(entrada,'%H:%i:%s'),adiant,STR_TO_DATE(atraso,'%H:%i:%s'),STR_TO_DATE(saida,'%H:%i:%s'),timediff(STR_TO_DATE(saida,'%H:%i:%s'),STR_TO_DATE(entrada,'%H:%i:%s')) from Registro where data = CURDATE()")                           
                r2 = cursor1.fetchall()
                




                
                return render_template('main.html',r1=r1,r2=r2)


        else:
              return "<p>Indisponivel</p>"        
                


@app.route('/_processo/', methods=['POST'])
def processo():


        global tag
        global TagOut
        
        ser = serial.Serial('/dev/ttyUSB0',9600)#Adiciona o a entrada da comunicação
        
        TagOut = ser.readline().decode().split('\r\n') #A variavel tag contem o conteudo da variavel 'data', que o arduino enviou atraves da comunicação serial    
        tag = TagOut[0]#ser.readline vai ler a linha que foi enviada
        time.sleep(1)
        
        result = tag

                                                     
        return jsonify({'data': render_template('recebe.html',result=result)})

                
     
              

        


@app.route('/cadastro', methods=['GET','POST'])
def cadastro():


        if block == 'off':
                
                if request.method == "GET":

                        
          
                        return render_template('cadastro.html')



                        
                if request.method == "POST":

                               
                                               
                        nome = request.form['nome']
                        
                        
                        if nome == '' or tag == '':

                                verifica()
                                
                                teste = True

                                return render_template('cadastro.html',teste=teste)
                        
          

                                

                        else:
                                cursor1.execute("select *from Cadastro where Tag = %s ", (tag))
                                teste2 = cursor1.fetchall()
                                
                                time.sleep(1)
                

                                if teste2:
                                
                                        verifica()
                                        
                                        return render_template('cadastro.html',teste2=teste2)
                        


                        
                                
                                else:
                                
                                        cursor.execute("INSERT INTO Cadastro(Nome,Tag) values(%s,%s)",(nome,tag))

                                        cursor1.execute("select *from Cadastro where Tag = %s",(tag))
                                        r2 = cursor1.fetchall()
                                
                            

                                        if r2:
                                                
                                                verifica()
                                                
                                                return render_template('cadastro.html',r2=r2)


        else:

             return "<p>Indisponivel</p>"     



@app.route('/buscar', methods =['GET','POST'])
def buscar():

        if block == 'off':
        
                if request.method == "GET":
                         
                        return render_template("buscar.html")
                    
              
                        
                if request.method == "POST":

                        global globalidenti
                        
                        globalidenti = request.form['id']
                        

                        if globalidenti == '':

                                teste=True

                                return render_template('buscar.html',teste=teste)


                        cursor.execute("SET lc_time_names = 'pt_BR'")
                        cursor1.execute("SELECT DISTINCT DATE_FORMAT(CURRENT_DATE,'%W, %d de %M de %Y') from Registro")
                        r1 = cursor1.fetchall()
                        cursor1.execute("select *from Cadastro where ID = %s ", (globalidenti))
                        r2 = cursor1.fetchall()
                        cursor1.execute("select date_format(data,'%%W'),date_format(data,'%%d/%%m/%%Y'),id,nome,STR_TO_DATE(entrada,'%%H:%%i:%%s'),adiant,STR_TO_DATE(atraso,'%%H:%%i:%%s'),STR_TO_DATE(saida,'%%H:%%i:%%s'), timediff(STR_TO_DATE(saida,'%%H:%%i:%%s'),STR_TO_DATE(entrada,'%%H:%%i:%%s')) from Registro where id = %s and data = CURDATE()" ,(globalidenti))
                        r3 = cursor1.fetchall()
                     
          

                       

                        if r2:
                                global rota

                                rota = '2'


                                return render_template('buscarResultado.html',r1=r1,r2=r2,r3=r3)

                        else:
                                teste2=True

                                return render_template('buscar.html',teste2=teste2)

        else:

               return "<p>Indisponivel</p>"


        
@app.route('/RegistrosAnteriores', methods =['GET','POST'])
def registrosAnteriores():

        if block == 'off':

                if request.method == "GET":
                         
                        return render_template('buscarRegistrosAnt.html')

                

                if request.method == "POST":

                        global globalmes
                        global globalano
             
                        globalmes = request.form['mes']
                        globalano = request.form['ano']
                        

                        
                        if  globalmes == '' or  globalano == '':

                                teste=True
                                

                                return render_template('buscarRegistrosAnt.html',teste=teste)
                         

                          
                        cursor1.execute("select *from Cadastro where ID = %s ", (globalidenti))
                        r = cursor1.fetchall()
                        cursor.execute("SET lc_time_names = 'pt_BR'")
                        
                        cursor1.execute("select date_format(data,'%%%%W'),date_format(data,'%%%%d/%%%%m/%%%%Y'),id,nome,STR_TO_DATE(entrada,'%%%%H:%%%%i:%%%%s'),adiant,STR_TO_DATE(atraso,'%%%%H:%%%%i:%%%%s'),STR_TO_DATE(saida,'%%%%H:%%%%i:%%%%s'),timediff(STR_TO_DATE(saida,'%%%%H:%%%%i:%%%%s'),STR_TO_DATE(entrada,'%%%%H:%%%%i:%%%%s')) from Registro where MONTH(data) = %s and YEAR(data) = %%s and id = %%s " % globalmes,(globalano,globalidenti))
                        r2 = cursor1.fetchall()
                        cursor1.execute("SELECT DISTINCT DATE_FORMAT(data,'%%%%M de %%%%Y') from Registro where MONTH(data) = %s and YEAR(data) = %%s " %globalmes ,( globalano))
                        r3 = cursor1.fetchall()
                        cursor1.execute("SELECT time_format(SEC_TO_TIME(SUM(TIME_TO_SEC(TIMEDIFF(STR_TO_DATE(saida,'%%%%H:%%%%i:%%%%s'),STR_TO_DATE(entrada,'%%%%H:%%%%i:%%%%s'))))),'%%%%H:%%%%i:%%%%s'),time_format(SEC_TO_TIME(SUM(TIME_TO_SEC(STR_TO_DATE(atraso,'%%%%H:%%%%i:%%%%s')))),'%%%%H:%%%%i:%%%%s')  from Registro where MONTH(data) = %s and YEAR(data) = %%s and id = %%s " %globalmes,(globalano,globalidenti))
                        r4 = cursor1.fetchall()
          
                                        
                        if r2:
                                global gera

                                gera = '3'        

                                return render_template('resultadoRegistrosAnt.html',r=r,r2=r2,r3=r3,r4=r4)
                        else:
                                teste2=True
                                
                                return render_template('buscarRegistrosAnt.html',teste2=teste2)
        

        else:
                
             return "<p>Indisponivel</p>"   




     
                
def verifica():


        global tag
        global TagOut
        
        tag=''
        TagOut=''
   


@app.route('/atualizar/<string:id>',methods = ['GET','POST'])
def atualiza(id):

        if block == 'off':


                if request.method == "GET":
                        
                        verifica()
                        
                        cursor1.execute("select ID,Nome,Tag from Cadastro where ID = %s",[id])
                        result = cursor1.fetchall()

                        return render_template("atualizar.html",result=result)

                if request.method == "POST":




                        nome = request.form['nome']

                        if tag != '':

                                cursor1.execute("update Cadastro set Tag = %s where ID = %s",(tag,[id]))          
                                r3 = cursor1.fetchall()

                                if r3:

                                        teste2 = True
                                        
                 
                                else:
                                        verifica()

                                        teste = True
                                        
                                        return render_template("atualizar.html",teste=teste)



                        else:
                                cursor1.execute("update Cadastro set Nome = %s where ID = %s",(nome,[id]))          
                                r = cursor1.fetchall()
                                cursor1.execute("update Registro set nome = %s where id = %s",(nome,[id]))          
                                r2 = cursor1.fetchall()


                                if r and r2:

                                        teste = True

                                else:
                                        verifica()
                                        teste = True
                                        return render_template("atualizar.html",teste=teste)
        else:

                return "<p>Indisponivel</p>"
        


@app.route('/remove/<string:id>',methods = ['POST'])
def remove(id):



        cursor.execute("DELETE FROM Cadastro WHERE ID = %s",[id])
        db.commit()
             
        cursor.execute("DELETE FROM Registro WHERE id = %s",[id])
        db.commit()

        if rota == '1':
  
                        
                return redirect(url_for('listarUsuario'))
                
        if rota == '2':
                        
                return redirect(url_for('buscar'))
                
                
                        

                
                   
                  

@app.route('/listarUsuario')
def listarUsuario():
        
       

        cursor1.execute("select *from Cadastro")
        r2 = cursor1.fetchall()
        global rota

        rota ='1'
                        
        return render_template('listarUsuarios.html',r2=r2)
       

        return "<p>Indisponivel</p>"
       

                
                

@app.route('/listarRegistro')
def listarRegistro():


        if block == 'off':
     
                cursor1.execute("select date_format(data,'%W'),date_format(data,'%d/%m/%Y'),id,nome,STR_TO_DATE(entrada,'%H:%i:%s'),adiant,STR_TO_DATE(atraso,'%H:%i:%s'),STR_TO_DATE(saida,'%H:%i:%s'),timediff(STR_TO_DATE(saida,'%H:%i:%s'),STR_TO_DATE(entrada,'%H:%i:%s')) from Registro where MONTH(data) = MONTH(CURRENT_DATE()) and YEAR(data) = YEAR(CURRENT_DATE())")
                r2 = cursor1.fetchall()
                cursor.execute("SET lc_time_names = 'pt_BR'")
                cursor1.execute(" SELECT DATE_FORMAT(CURRENT_DATE,'%M de %Y')");

                r3 = cursor1.fetchall()

                global gera

                gera = '1'
                

                return render_template('listarRegistro.html',r2=r2,r3=r3)

        else:
                return "<p>Indisponivel</p>"  





           
@app.route('/buscarRegistros' , methods =['GET','POST'])
def buscarRegistro():

        if block == 'off':

                if request.method == "GET":
                         
                        return render_template("buscarRegistros.html")

                

                if request.method == "POST":

                        global globalmes
                        global globalano
             
                        globalmes = request.form['mes']
                        globalano = request.form['ano']
                        

                        
                        if  globalmes == '' or  globalano == '':

                                teste=True
                                

                                return render_template('buscarRegistros.html',teste=teste)
                         

                  

                        cursor1.execute("SET lc_time_names = 'pt_BR'")
                        cursor1.execute("select date_format(data,'%%%%W'),date_format(data,'%%%%d/%%%%m/%%%%Y'),id,nome,STR_TO_DATE(entrada,'%%%%H:%%%%i:%%%%s'),adiant,STR_TO_DATE(atraso,'%%%%H:%%%%i:%%%%s'),STR_TO_DATE(saida,'%%%%H:%%%%i:%%%%s'),timediff(STR_TO_DATE(saida,'%%%%H:%%%%i:%%%%s'),STR_TO_DATE(entrada,'%%%%H:%%%%i:%%%%s')) from Registro where MONTH(data) = %s and YEAR(data) = %%s " % globalmes,(globalano))
                        r2 = cursor1.fetchall()
                        cursor1.execute("SELECT DISTINCT DATE_FORMAT(data,'%%%%M de %%%%Y') from Registro where MONTH(data) = %s and YEAR(data) = %%s " %globalmes ,( globalano))
                        r3 = cursor1.fetchall()

                        if r2:
                                global gera

                                gera = '2'        

                                return render_template('resultadoRegistros.html',r2=r2,r3=r3)
                        else:
                                teste2=True
                                
                                return render_template('buscarRegistros.html',teste2=teste2)

                        
        
        else:

                 return "<p>Indisponivel</p>"  
                



                
                        

   

@app.route('/pdf')

def pdf_template():
        
        if block == 'off':

                if gera == '1':#Gera pdf dos registros do mês atual
                
                        cursor1.execute("select date_format(data,'%d/%m/%Y'),id,nome,STR_TO_DATE(entrada,'%H:%i:%s'),adiant,STR_TO_DATE(atraso,'%H:%i:%s'),STR_TO_DATE(saida,'%H:%i:%s'),timediff(STR_TO_DATE(saida,'%H:%i:%s'),STR_TO_DATE(entrada,'%H:%i:%s')) from Registro where MONTH(data) = MONTH(CURRENT_DATE()) and YEAR(data) = YEAR(CURRENT_DATE())")
                        r2 = cursor1.fetchall()
                        cursor.execute("SET lc_time_names = 'pt_BR'")
                        cursor1.execute(" SELECT DATE_FORMAT(CURRENT_DATE,'%M de %Y')");

                        r3 = cursor1.fetchall()

                        render = render_template('pdf.html',r2=r2,r3=r3)

                        pdf = pdfkit.from_string(render,False)

                        response = make_response(pdf)
                        response.headers['Content-Type'] = 'application/pdf'
                        response.headers['Contente=Disposition'] = 'inline; filename=output.pdf'

                        return response
                      
                        

                if gera == '2':#Gera pdf dos registros do mês especifico   

                        cursor1.execute("select date_format(data,'%%%%d/%%%%m/%%%%Y'),id,nome,STR_TO_DATE(entrada,'%%%%H:%%%%i:%%%%s'),adiant,STR_TO_DATE(atraso,'%%%%H:%%%%i:%%%%s'),STR_TO_DATE(saida,'%%%%H:%%%%i:%%%%s'),timediff(STR_TO_DATE(saida,'%%%%H:%%%%i:%%%%s'),STR_TO_DATE(entrada,'%%%%H:%%%%i:%%%%s')) from Registro where MONTH(data) = %s and YEAR(data) = %%s " %globalmes,(globalano))
                        r2 = cursor1.fetchall()
                        cursor.execute("SET lc_time_names = 'pt_BR'")
                        cursor1.execute(" SELECT DISTINCT DATE_FORMAT(data,'%%%%M de %%%%Y') from Registro where MONTH(data) = %s and YEAR(data) = %%s " % globalmes,(globalano))
                        r3 = cursor1.fetchall()

                        render = render_template('pdf2.html',r2=r2,r3=r3)

                        pdf = pdfkit.from_string(render,False)

                        response = make_response(pdf)
                        response.headers['Content-Type'] = 'application/pdf'
                        response.headers['Contente=Disposition'] = 'inline; filename=output.pdf'

                        return response

                if gera == '3':#Gera pdf dos registros do usuario especifico

                        cursor1.execute("select *from Cadastro where ID = %s ", (globalidenti))
                        r = cursor1.fetchall()
                        cursor1.execute("SET lc_time_names = 'pt_BR'")
                        cursor1.execute("select date_format(data,'%%%%d/%%%%m/%%%%Y'),id,nome,STR_TO_DATE(entrada,'%%%%H:%%%%i:%%%%s'),adiant,STR_TO_DATE(atraso,'%%%%H:%%%%i:%%%%s'),STR_TO_DATE(saida,'%%%%H:%%%%i:%%%%s'), timediff(STR_TO_DATE(saida,'%%%%H:%%%%i:%%%%s'),STR_TO_DATE(entrada,'%%%%H:%%%%i:%%%%s')) from Registro where MONTH(data) = %s and YEAR(data) = %%s and id = %%s " % globalmes,(globalano,globalidenti))
                        r2 = cursor1.fetchall()
                        cursor1.execute("SELECT DISTINCT DATE_FORMAT(data,'%%%%M de %%%%Y') from Registro where MONTH(data) = %s and YEAR(data) = %%s " %globalmes ,( globalano))
                        r3 = cursor1.fetchall()
                        cursor1.execute("SELECT time_format(SEC_TO_TIME(SUM(TIME_TO_SEC(TIMEDIFF(STR_TO_DATE(saida,'%%%%H:%%%%i:%%%%s'),STR_TO_DATE(entrada,'%%%%H:%%%%i:%%%%s'))))),'%%%%H:%%%%i:%%%%s'),time_format(SEC_TO_TIME(SUM(TIME_TO_SEC(STR_TO_DATE(atraso,'%%%%H:%%%%i:%%%%s')))),'%%%%H:%%%%i:%%%%s')  from Registro where MONTH(data) = %s and YEAR(data) = %%s and id = %%s " %globalmes,(globalano,globalidenti))
                        r4 = cursor1.fetchall()



                        render = render_template('pdf3.html',r=r,r2=r2,r3=r3,r4=r4)

                        pdf = pdfkit.from_string(render,False)

                        response = make_response(pdf)
                        response.headers['Content-Type'] = 'application/pdf'
                        response.headers['Contente=Disposition'] = 'inline; filename=output.pdf'

                        return response

        else:

                return "<p>Indisponivel</p>"
        
                
@app.route('/sair')
def sair():
        
        global block

        block = 'on'
        
        return home()






extra_dirs = ['/home/pi/webaap/templates',]

extra_files = extra_dirs[:]
for extra_dir in extra_dirs:
    for dirname, dirs, files in os.walk(extra_dir):
        for filename in files:
            filename = os.path.join(dirname, filename)
            if os.path.isfile(filename):
                extra_files.append(filename)
   


if __name__ == "__main__":
 
    
  
   
    app.secret_key = os.urandom(12)
   
    app.run(threaded=True, extra_files=extra_files,debug=True,host='0.0.0.0',port=80)

         




