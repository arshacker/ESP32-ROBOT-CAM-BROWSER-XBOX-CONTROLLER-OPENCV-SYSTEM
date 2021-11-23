#ANN:1
import pygame
from pygame.locals import *
import dashboard_26_module
import time
import datetime
import math
import keyboard
#ANN:2
import websocket  #actually websocket-client; websocket gives wrong WebSocket()

ws = websocket.WebSocket()
ws.connect("ws://X.X.X.X:82/ws") #ROBOT_BROWSER_SERVER IP ADDRESS

i = 0

listBIN = [0,0,0,0,0,0,0]


def DecToBin(num,i):
            
    if num > 1:
        DecToBin(num // 2,i+1) #push down stack, first in-last out
    #print("num = ",num)
    #print("num%2=",num % 2)
    #print("i=",i)
    listBIN[6-i]=num % 2  #6-i reverses so last out goes into listBIN[0]
    #print(listBIN)




index = 0

BYTE_NUMB = 127
#ANN:3
JStick_Arc_Radius = dashboard_26_module.radius_1

listFunc = [dashboard_26_module.Button_A,dashboard_26_module.Button_B,\
            dashboard_26_module.Button_X,dashboard_26_module.Button_Y,\
            dashboard_26_module.Button_L,dashboard_26_module.Button_R,\
            dashboard_26_module.Button_L1,dashboard_26_module.Button_R1,\
            0,0,0,\
            dashboard_26_module.JStick_2X,dashboard_26_module.JStick_2Y,\
            dashboard_26_module.TrigL_Sig,\
            dashboard_26_module.JStick_1X,dashboard_26_module.JStick_1Y,\
            dashboard_26_module.TrigR_Sig,\
            dashboard_26_module.HatX,dashboard_26_module.HatY]

listSensorFunc = [dashboard_26_module.SENSOR_1_VALUE,\
                  dashboard_26_module.SENSOR_2_VALUE,\
                  dashboard_26_module.SENSOR_3_VALUE]
#ANN:4
list0 = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]
#list0 = [0,1,1,0,1,0,1,1,0,0,0,117,-122,1,127,-127,1,-1,1,0]
#        0 1 2 3 4 5 6 7 X X X 11  12  13   14  15  16    17 18  19
#                              J2X J2Y LTRG J1X J1Y RTRG  HX HY  PULSE
#                               + -
#                              0-127

                 #ascii [=91  ]=93  ,=44  sp=32  -=45  0=48

listACK1 = [0,0,0,0,0]
listACK2 = [0,0,0,0]

list_rcv = [0,0,0,0,0,0,0,0,0,0]

listACK_rcv = [0,0,0,0,0,0,0,0,0,0]

listACK_rcv_ord = [0,0,0,0,0,0,0,0,0,0]  #only first 4 locs used

button_out = [0,0,0,0,0,0,0,0,0,0,0]
button_out_prev = [0,0,0,0,0,0,0,0,0,0,0]
button_prev_zero = 0
button_out_diff = [0,0,0,0,0,0,0,0,0,0,0]

running = True


black = (0,0,0)
gray = (128,128,128)
light_gray = (200,200,200)
dark_gray = (64,64,64)
white = (255,255,255)
red = (255,0,0)
light_red =(128,0,0)
green = (0,255,0)
blue = (0,0,255)
listColor_in=[0,0,0]
listColor_in[0]=abs(-255)
Color_in = tuple(listColor_in)

Max_Arc_Width = math.floor((dashboard_26_module.ScreenWidth/250)*25) #25
Max_JStick_Amplitude = 127
#ANN:5
def update_dashboard():
    for i in [0,1,2,3,4,5,6,7,11,12,13,14,15,16,17,18]:
        if(i<8):
            if(list0[i]==1):
               listFunc[i](green) 
            if(list0[i]==0):
               listFunc[i](gray)
        if(i==13 or i == 16):
            #MAPPING FOR NEG X   RED=127+ABS(X)  GREEN=127-ABS(X)
            #        FOR POS X   RED=127-ABS(X)  GREEN=127+ABS(X)
            if(list0[i]<0):
               listFunc[i]((127+abs(list0[i]),127-abs(list0[i]),0))
            if(list0[i]>=0):
               listFunc[i]((127-abs(list0[i]),127+abs(list0[i]),0))
        if(i==11 or i==12 or i==14 or i==15):
            if(list0[i]<0):               
               #ARC WIDTH = FRACTIONAL (MAX AMPL-AMPL)/MAX AMPL 
               listFunc[i](math.floor(((Max_JStick_Amplitude-abs(list0[i]))/\
                 Max_JStick_Amplitude)*Max_Arc_Width),Max_Arc_Width)  
            if(list0[i]>=0):               
               #ARC WIDTH = FRACTIONAL (MAX AMPL-AMPL)/MAX AMPL                 
               listFunc[i](Max_Arc_Width,\
                 math.floor(((Max_JStick_Amplitude-abs(list0[i]))/\
                 Max_JStick_Amplitude)*Max_Arc_Width))               
        if(i==17 or i==18):
            if(list0[17]==1 and list0[18]==1):
               listFunc[17](gray,green)
               listFunc[18](green,gray)               
            if(list0[17]==1 and list0[18]==0):
               listFunc[17](gray,green)
               listFunc[18](gray,gray)                              
            if(list0[17]==1 and list0[18]==-1):
               listFunc[17](gray,green)
               listFunc[18](gray,red)                              
            if(list0[17]==0 and list0[18]==1):
               listFunc[17](gray,gray)
               listFunc[18](green,gray)               
            if(list0[17]==0 and list0[18]==0):
               listFunc[17](gray,gray)
               listFunc[18](gray,gray)                  
            if(list0[17]==0 and list0[18]==-1):
               listFunc[17](gray,gray)
               listFunc[18](gray,red)                  
            if(list0[17]==-1 and list0[18]==1):
               listFunc[17](red,gray)
               listFunc[18](green,gray)               
            if(list0[17]==-1 and list0[18]==0):
               listFunc[17](red,gray)
               listFunc[18](gray,gray)                  
            if(list0[17]==-1 and list0[18]==-1):
               listFunc[17](red,gray)
               listFunc[18](gray,red)               
    for j in [0,1,2]:
            if (j==0):
                 listSensorFunc[j](green,listACK2[j]/127)
            if (j==1):
                 listSensorFunc[j](green,listACK2[j]/127)
            if (j==2):
                 listSensorFunc[j](red,listACK2[j])     
#ANN:6
def do_running():
  global i  
  global running
  global index
  global listACK1
  while (running):
    start_time = datetime.datetime.now()
    #time.sleep(4)
    #ANN:7
    dashboard_26_module.JStick_1(red,green,red,green)
    dashboard_26_module.JStick_2(red,green,red,green)
    dashboard_26_module.SENSOR_1_FILL(gray,-80,260)
    dashboard_26_module.SENSOR_2_FILL(gray)
    dashboard_26_module.SENSOR_3_FILL(green)
###################FROM XBOX_TW0_TEST_8_EXPT2############
    list0[19] = index    #pulse of the transmission sytem 
    index = index % BYTE_NUMB
    index = index + 1
    #print("List0 = ",list0)
    #ANN:8
  ###GET CONTROLLER INPUTS#################  
   # Get count of joysticks
    joystick_count = pygame.joystick.get_count()
    #print(joystick_count)
    for i in range(joystick_count):
        joystick = pygame.joystick.Joystick(i)
        joystick.init()


        # Get the name from the OS for the controller/joystick
        name = joystick.get_name()
   
        # Usually axis run in pairs, up/down for one, and left/right for the other.
        axes = joystick.get_numaxes()
        #print(axes)
        for j in range( axes ):
            axis = joystick.get_axis( j )
            list0[j+11] = round(BYTE_NUMB*axis)

        buttons = joystick.get_numbuttons()

        for i in range( buttons ):
            button_out[i] = joystick.get_button( i )
            if(button_out[i]==1 and button_out_prev[i]==0):                
                button_out_diff[i] =1
            else:
                button_out_diff[i] =0
            button_out_prev[i] = button_out[i]
            list0[i] = button_out_diff[i]^list0[i]
            

        # Hat switch. All or nothing for direction, not like joysticks.
        # Value comes back in a tuple.
        hats = joystick.get_numhats()

        for i in range( hats ):
            hat = joystick.get_hat(i )
            mytuple = hat            
        list0[17] = mytuple[0]
        list0[18] = mytuple[1]            
            

    print("list0 = ",list0)
#####################END FROM XBOX_TW0_TEST_8_EXPT2########   
    
    update_dashboard()
    pygame.display.update()
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            print("QUIT")
            running = False
            pygame.quit()


##############PREPARE AND SEND MESSAGE######################
    #ANN:9
    string0 = str(list0)
    string0_stripped = string0[1:len(string0)-1] #strip [ and ]
    message = string0_stripped.encode()
    print("message =",message)
    #message = bytes(list1)
    #ws.send("hello") #works. sends each char as ascii
    ws.send(message)   #workd. sends each nume ral as ascii
    #ws.send(5) #does not work
    #ws.send_binary(100|35|50)  #does not work
    #end_time1 = datetime.datetime.now()
    #exec_time1 = end_time1 - start_time
    #print("execTime1 = ",exec_time1)   
    index1 = 0
    data = b""
    msgLength = len(message)
    print("msgLength = ",msgLength)

############END PREPARE AND SEND MESSAGE#######################

#----------ALT RCV MSG-------------------------------
    for x in range(7):
        listBIN[x] = 0
    print("listBIN = ",listBIN)

    for x in range(5):
        listACK1[x] = 0
        #listACK2[x] = 1
    print("listACK1 = ",listACK1)

    for x in range(4):
        listACK2[x] = 1    
    print("listACK2 = ",listACK2)

    #for z in range(10):
        #listACK_rcv[z] = 250
    #print("listACK_rcv = ",listACK_rcv)

    
    #ANN:10
    ACKDATA1 = ws.recv()
    print("ACKDATA1 = ",ACKDATA1)
    listACK1 = list(ACKDATA1)
    #print(list(ACKDATA1))
    print("listACK1 = ",listACK1)
    DecToBin(listACK1[4],i)
    print("DECTOBIN = ",listBIN)
    if listBIN[0]==1:
        listACK2[0]=0
    else:
        listACK2[0]=listACK1[0]
    if listBIN[1]==1:
        listACK2[1]=0
    else:
        listACK2[1]=listACK1[1]        
    if listBIN[2]==1:
        listACK2[2]=0
    else:
        listACK2[2]=listACK1[2]        
    if listBIN[3]==1:
        listACK2[3]=0
    else:
        listACK2[3]=listACK1[3]        
    print("listACK2 = ",listACK2)


    #print("listACK2 =",listACK2)
    
    end_time2 = datetime.datetime.now()
    exec_time2 = end_time2 - start_time
    print("execTime2 = ",exec_time2)
    


#----------END ALT RCV MSG---------------------------   
 
def main():
  dashboard_26_module.Dashboard()

  # Used to manage how fast the screen updates
  #clock = pygame.time.Clock()

  # Initialize the joysticks
  pygame.joystick.init()


###############COMM  INIT#############################
#import socket
#sock = socket.socket()
#host = "192.168.1.9" #ESP32 IP in local network
#port = 80             #ESP32 Server Port    
#sock.connect((host, port))



  #ws = websocket.WebSocket()
  #ws.connect("ws://192.168.1.4:82/ws")


##################END COMM INIT#######################

#######################COMM ESTABLISHED#####################

  ACKDATA = ws.recv()
  print("ACKDATA = ",ACKDATA)
  print("PRESS q TO SEND AND z TO CLOSE")

#if(keyboard.is_pressed('q')): 

###################END COMM ESTABLISHED#####################

  do_running()

if __name__=="__main__":
  main()
