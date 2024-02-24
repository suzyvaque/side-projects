import pygame
import sys
from pygame import draw
from pygame.constants import DROPTEXT, KEYDOWN, QUIT, K_UP, K_DOWN, K_LEFT, K_RIGHT, K_c, K_s
import random
import time

# =========================================================================
# set constants
MainColor= (0,0,0)   # Black
LineColor= (173,46,255) # For Boundary
FontColor= (242,177,36)  # Info

EarthColor1= (2,20,98) # Sky
EarthColor2= (6,48,21) # Grass
UFOLightColor= (193,51,249)
UFOInColor= (28,43,221)   # UFO Background

# Use image
# UFOBodyColor= (31,118,255)
# PhoneColor= (227,52,40)
# PlayerColor= (187,154,117)
# UFOPassageColor= (175,46,253)

UP=(0,-1)
DOWN=(0,1)
LEFT=(-1,0)
RIGHT=(1,0)

screen_width=1000
screen_height=600
lineSpace=1
# playScreen_width=800, playScreen_height=400
playScreen_x1=100
playScreen_x2=900
playScreen_y1=100
playScreen_y2=500
# actual coordinates, from bottom left, top left, top right, bottom right
playScreen_TL= [playScreen_x1,playScreen_y1]
playScreen_BL= [playScreen_x1,playScreen_y2]
playScreen_BR= [playScreen_x2,playScreen_y2]
playScreen_TR= [playScreen_x2,playScreen_y1]
# for info
infoSpace=30   # playScreen_y+-50
infoScreen_BL= [playScreen_x1+infoSpace*4,playScreen_y2+infoSpace]
infoScreen_TL= [playScreen_x1+infoSpace*4,playScreen_y2+infoSpace*2]
infoScreen_TR= [playScreen_x2-infoSpace*4,playScreen_y2+infoSpace*2]
infoScreen_BR= [playScreen_x2-infoSpace*4,playScreen_y2+infoSpace]
# for Earth actual coloring
earthSpace=50

# =========================================================================
totalTimeA=110
totalTimeG=44
start=time.time()
def countdown():
    leftTimeA=totalTimeA-int(time.time()-start)
    printTimeG=int(leftTimeA/2.5)
    subTimeSA=int(leftTimeA/1.7)
    subTimeSG=int(leftTimeA/1.3)
    return leftTimeA,printTimeG,subTimeSA,subTimeSG

def writeInfo():
    textTL="DOOMSDAY - 00D 00H "+str(countdown()[1])+"M"
    textTR1="FELLOW "+str(player.fellow)
    textTR2="CAPTIVE "+str(UFO.captive)
    font=pygame.font.Font("resources/AnticSlab-Regular.otf",15)
    infoTL=font.render(textTL,True,FontColor)
    infoTR1=font.render(textTR1,True,FontColor)
    infoTR2=font.render(textTR2,True,FontColor)
    game_display.blit(infoTL,(playScreen_x1,playScreen_y1-infoSpace))
    game_display.blit(infoTR1,(playScreen_x2-infoSpace*6,playScreen_y1-infoSpace))
    game_display.blit(infoTR2,(playScreen_x2-infoSpace*3,playScreen_y1-infoSpace))
    if countdown()[1]<=4:
        warning=font.render("THE END IS NEAR",True,FontColor)
        game_display.blit(warning,(playScreen_x1,playScreen_y1-infoSpace*2))

message=""
view=True

def setMessage(str):
    global view
    global message
    view=True
    message=str

def writeMsg():
    global message
    startX=(infoScreen_BL[0]+int((infoScreen_BR[0]-infoScreen_BL[0])/2))-int(len(message)/2)*10+infoSpace
    startY=infoScreen_BL[1]+int(infoSpace/4)

    font=pygame.font.Font("resources/AnticSlab-Regular.otf",15)
    msg=font.render(message,view,FontColor)
    game_display.blit(msg,(startX,startY))
    # after 2sec, textView=False
    
# =========================================================================
class Player:
    #location="UFO"
    location="Earth"
    struck=False
    fellow=0

    x= int((playScreen_x1+playScreen_x2)/2)
    y= playScreen_y2-earthSpace
    actualCoords=[(x,y),(x+10,y+10),(x,y+10),(x+10,y+10)]

    playerImg= pygame.image.load("resources/PlayerTrans.png")
    playerImg= pygame.transform.scale(playerImg,(20,20))

    def __init__(self,display):
        self.display=display

    def draw(self):
        self.display.blit(self.playerImg,(self.x,self.y))

    def handle_event(self,event):
        if self.location== "Earth":
            if event.type==KEYDOWN:
                if event.key== K_c:
                    self.call()

        if self.location== "UFO":
            if event.type==KEYDOWN:
                if event.key== K_s:
                    if self.location== "UFO":
                        self.tryExit()
    
    def move(self,keys):
        if keys[pygame.K_UP]:
            if self.location=="Earth":
                if self.y>playScreen_y1+earthSpace*5:
                    self.y-=10
            else:   # self.location=="UFO"
                if self.y>playScreen_y1:
                    self.y-=10
        if keys[pygame.K_DOWN]:
            if self.y+20<playScreen_y2:
                self.y+=10
        if keys[pygame.K_LEFT]:
            if self.x>playScreen_x1:
                self.x-=10
        if keys[pygame.K_RIGHT]:
            if self.x+20<playScreen_x2:
                self.x+=10
        self.actualCoords=[(self.x,self.y),(self.x+10,self.y+10),(self.x,self.y+10),(self.x+10,self.y+10)]

    def bgAsLocation(self, game_display):
        if self.location=="Earth":
            game_display.fill(MainColor)
            pygame.draw.lines(game_display, LineColor, False, [playScreen_BL,playScreen_TL,playScreen_TR,playScreen_BR,playScreen_BL], lineSpace)
            pygame.draw.lines(game_display, LineColor, False, [infoScreen_BL,infoScreen_TL,infoScreen_TR,infoScreen_BR,infoScreen_BL], lineSpace)
            pygame.draw.rect(game_display,EarthColor1,(playScreen_x1+lineSpace,playScreen_y1+lineSpace,playScreen_x2-playScreen_x1-lineSpace,earthSpace*5-lineSpace))
            pygame.draw.rect(game_display,EarthColor2,(playScreen_x1+lineSpace,playScreen_y1+earthSpace*5+lineSpace,playScreen_x2-playScreen_x1-lineSpace,earthSpace*3-lineSpace))

        else:   # self.location== "UFO"
            game_display.fill(MainColor)
            pygame.draw.lines(game_display, LineColor, False, [playScreen_BL,playScreen_TL,playScreen_TR,playScreen_BR,playScreen_BL], lineSpace)
            pygame.draw.lines(game_display, LineColor, False, [infoScreen_BL,infoScreen_TL,infoScreen_TR,infoScreen_BR,infoScreen_BL], lineSpace)
            pygame.draw.rect(game_display,UFOInColor,(playScreen_x1+lineSpace,playScreen_y1+lineSpace,playScreen_x2-playScreen_x1-lineSpace,playScreen_y2-playScreen_y1-lineSpace))
    
    def call(self):
        for coordPlayer in player.actualCoords: # player
            for phone in phones:
                for coordPhone in phone.actualCoordsPhone:  # one phone obj
                    if coordPlayer==coordPhone:
                        if(random.randint(1,100)<=77):
                            setMessage("VALID CALL: YOU DISCOVERED A FELLOW !")
                            player.fellow+=1
                        else:
                            setMessage("INVALID CALL")
                        phone.unused=False
                        phone.redraw()
                        return None

    def struckBy_light(self):
        global ufos
        for ufo in ufos:
            laserMap=UFO.findLaserMap(ufo.x,ufo.y)
            for laser in laserMap[:ufo.count]:  # level
                for eachCoord in laser: # one specific coord
                    for playerCoord in self.actualCoords:
                        if eachCoord==playerCoord:
                            self.struck=True
                            setMessage("ABDUCTED BY ALIENS: FIND THE EXIT AND ESCAPE")
                            self.location="UFO"
                            ufos.clear()
                            break
    
    def tryExit(self):
        for coordPlayer in player.actualCoords: # player
            for passage in passages:
                for coordPassage in passage.actualCoordsPassage:  # one phone obj
                    if coordPlayer==coordPassage:
                        if passage.validExit==True:
                            setMessage("FOUND THE EXIT: RETURNED TO EARTH")
                            player.fellow-=1
                            UFO.captive+=1
                            player.struck=False
                            player.x=int((playScreen_x1+playScreen_x2)/2)
                            player.y=playScreen_y2-earthSpace
                            player.location="Earth"
                            # reset passages
                            Passage.reset()
                        else:
                            setMessage("NOT AN EXIT: YOU SACRIFICED TWO FELLOWS")
                            player.fellow-=2
                            UFO.captive+=2
                        return None

class Phone:
    pbImg= pygame.image.load("resources/PhoneTrans.png")
    pbImg= pygame.transform.scale(pbImg,(20,20))
    actualCoordsPhone=[]    # single
    coords=[]   # [[(x,y),(x,y+10)...]]
    unused=True

    def __init__(self,display):
        
        while True:
            self.x=random.randint(int(playScreen_x1/10)+lineSpace*2,int(playScreen_x2/10)-lineSpace*2)*10
            self.y=random.randint(int((playScreen_y1+earthSpace*5)/10)+lineSpace*2,int(playScreen_y2/10)-lineSpace*2)*10
            self.actualCoordsPhone=[(self.x,self.y),(self.x+10,self.y),(self.x,self.y+10),(self.x+10,self.y+10)]

            for coord in self.coords:   # no overlapping with existing phone
                for singleCoord in coord:   # 4 in each
                    for actualCoord in self.actualCoordsPhone:
                        #if singleCoord==actualCoord:
                        if singleCoord[0]==actualCoord[0] or singleCoord[0]+10==actualCoord[0] or singleCoord[0]-10==actualCoord[0]:
                            if singleCoord[1]==actualCoord[1] or singleCoord[1]+10==actualCoord[1] or singleCoord[1]-10==actualCoord[1]:
                                continue
            for singleCoord in player.actualCoords:   # no overlapping with player
                    for actualCoord in self.actualCoordsPhone:
                        if singleCoord==actualCoord:
                            continue

            self.coords.append(self.actualCoordsPhone)
            break
        self.display=display

    def draw(self):
        self.display.blit(self.pbImg,(self.x,self.y))
        self.unused=True

    def redraw(self):
        for phone in phones:
            if phone.unused==False:
                phones.remove(phone)
        newPhone=Phone(game_display)
        phones.append(newPhone)

class UFO:
    captive=random.randint(13,17)

    ufoImg= pygame.image.load("resources/UFOTrans.png")
    ufoImg= pygame.transform.scale(ufoImg,(20,20))    
    appear=False

    coords=[]   # [[(x,y),(x,y+10)...], []...]
    actualCoordsUFO=[]  # one UFO obj's actual positions

    laserComp=[]
    count=0
    previousTimeA=110
    previousTimeG=44
    previousTimeSA=65
    previousTimeSG=84
    

    def __init__(self,display):
        self.appear=True
        self.display=display

        while True:
            self.x=random.randint(int(playScreen_x1/10)+lineSpace*2,int(playScreen_x2/10)-lineSpace*2)*10
            # seperated from land
            self.y=random.randint(int(playScreen_y1/10)+lineSpace*2, int((playScreen_y1+earthSpace*4)/10)+lineSpace*2)*10
            self.actualCoordsUFO=[(self.x,self.y),(self.x+10,self.y),(self.x,self.y+10),(self.x+10,self.y+10)]

            for coord in self.coords:   # no overlapping with existing phone
                for singleCoord in coord:   # 4 in each
                    for actualCoord in self.actualCoordsUFO:
                        # do not appear in the same vertical line
                        if singleCoord[0]==actualCoord[0] or singleCoord[0]+10==actualCoord[0] or singleCoord[0]-10==actualCoord[0]:
                            continue
            self.coords.append(self.actualCoordsUFO)
            break
        
    def create():
        if countdown()[3]<UFO.previousTimeSG:   # 1.pace of creating new UFOs
            newUFO=UFO(game_display)
            ufos.append(newUFO)
            UFO.laserComp.append(newUFO.createLaserMap(newUFO.x,newUFO.y))   # update laserCoords for drawLaser()
            UFO.previousTimeSG=countdown()[3]    # update time   # 1.pace of creating new UFOs

    def draw(self):
        self.display.blit(self.ufoImg,(self.x,self.y))
        self.drawLaser(self.x,self.y)

    def createLaserMap(self,x,y):
        self.laserCoords=[]
        fixedX=x+10
        tempY=y+10 # level 1
        #for _ in range(30):
        for _ in range(int((playScreen_y2-y)/10)):
            if tempY+10 < playScreen_y1+earthSpace*5:
                self.lastLaserPos=[(fixedX,tempY+10)]   
            else:   # at land
                self.lastLaserPos=[(fixedX,tempY+10)]
                for i in range(1, int((tempY-(playScreen_y1+earthSpace*5))/10)+2 ):
                    self.lastLaserPos.append((fixedX-i*10, tempY+10))
                    self.lastLaserPos.append((fixedX+i*10, tempY+10))
            self.laserCoords.append(self.lastLaserPos)
            tempY+=10
        return(self.laserCoords)

    def findLaserMap(x,y):
        for i in range(len(UFO.laserComp)):
            if UFO.laserComp[i][0][0]==(x+10,y+10+10):
                return UFO.laserComp[i]

    def drawLaser(self,x,y):
        laserCoords=UFO.findLaserMap(x,y)
        if countdown()[2]<self.previousTimeSA*2:    # 2.pace of lasers
            self.count+=1
        if self.count>int((playScreen_y2-y)/10):    # matches createLaserMap range
            self.appear=False
            self.count=0    # or else, struck() reacts late
            return None
        for laser in laserCoords[:self.count]: # for each level
            for eachCoord in laser: # coords with the same y
                if playScreen_x1<eachCoord[0]<playScreen_x2 and eachCoord[1]<playScreen_y2:
                    pygame.draw.rect(game_display,UFOLightColor,[eachCoord[0],eachCoord[1],3,3])
        self.previousTimeSA=countdown()[2]  # update time    # 2.pace of lasers
        
class Passage:
    pssImg=pygame.image.load("resources/PassageTrans.png")
    pssImg= pygame.transform.scale(pssImg,(20,20))
    actualCoordsPassage=[]
    validExit=False
    coords=[]

    def __init__(self,display,TF):
        while True:
            self.x=random.randint(int(playScreen_x1/10)+lineSpace*2,int(playScreen_x2/10)-lineSpace*2)*10
            self.y=random.randint(int(playScreen_y1/10)+lineSpace*2,int(playScreen_y2/10)-lineSpace*2)*10
            self.actualCoordsPassage=[(self.x,self.y),(self.x+10,self.y),(self.x,self.y+10),(self.x+10,self.y+10)]

            for coord in self.coords:   # no overlapping with existing passage
                for singleCoord in coord:   # 4 in each
                    for actualCoord in self.actualCoordsPassage:
                        if singleCoord[0]==actualCoord[0]:
                            continue
            for singleCoord in player.actualCoords:   # no overlapping with player
                    for actualCoord in self.actualCoordsPassage:
                        if singleCoord[0]==actualCoord[0]:
                            continue

            self.coords.append(self.actualCoordsPassage)
            break 
        if TF==True:
            self.validExit=True
        self.display=display
    
    def reset():
        global passages
        passages=[Passage(game_display,True) for _ in range(13)] # 10 are validExit==True
        for _ in range(13):
            passages.append(Passage(game_display,False))

    def draw(self):
        self.display.blit(self.pssImg,(self.x,self.y))

# =========================================================================
pygame.init()
game_display= pygame.display.set_mode((screen_width,screen_height))
pygame.display.set_caption("call_404")
clock= pygame.time.Clock()

game_over=False
show_ending=False
player=Player(game_display)
phones=[Phone(game_display) for _ in range(5)]
ufos=[]
passages=[Passage(game_display,True) for _ in range(13)] # 10 are validExit==True
for _ in range(13):
    passages.append(Passage(game_display,False))

def check_over():
    global game_over
    global show_ending
    if player.location=="UFO" and player.fellow<=0:
        setMessage("NO FELLOW TO SACRIFICE: YOU CANNOT ESCAPE")
        return True
    if countdown()[1]<=0:
        setMessage("TIMEOVER")
        return True

while not game_over:
    if check_over():
        game_over=True
        show_ending=True
        break

    for event in pygame.event.get():
        if event.type== QUIT:
            game_over=True
            break    
        player.handle_event(event)

    player.struckBy_light()

    keys=pygame.key.get_pressed()
    player.move(keys)

    player.bgAsLocation(game_display)
    if player.location=="Earth":
        for phone in phones:
            if phone.unused:
                phone.draw()    # create
        UFO.create()
        for ufo in ufos:
            if ufo.appear:
                ufo.draw()     
    else:   # player.location=="UFO"
        for passage in passages:
            passage.draw()
        fakePassages=[Passage(game_display,False) for _ in range(44)]
        for fakePassage in fakePassages:
            fakePassage.draw()

    player.draw()
    #player.struckBy_light()

    writeInfo()
    writeMsg()

    pygame.display.update()
    clock.tick(13)   # 13 loop for every 1 sec  # 3. pace of movement

while show_ending==True:
    game_display.fill(MainColor)

    for event in pygame.event.get():
        if event.type== QUIT:
            show_ending=False
            break
    
    messageP1="YOU ARE ALIVE"
    messageP2="YOU ARE DEAD"
    messageE1="AND SAVED THE EARTH !"
    messageE2="BUT SAVED THE EARTH !"
    messageE3="... BUT THE END IS HERE ANYWAY"

    if message!="TIMEOVER": # "YOU ARE DEAD ...BUT THE END IS HERE ANYWAY"
        messageP=messageP2
        messageE=messageE3
    else:
        if player.location=="Earth":
            if player.fellow>UFO.captive:   # "YOU ARE ALIVE\nAND SAVED THE EARTH!"
                messageP=messageP1
                messageE=messageE1
            else:   # "YOU ARE ALIVE ...BUT THE END IS HERE ANYWAY"
                messageP=messageP1
                messageE=messageE3
        else:
            if player.fellow>UFO.captive:   # "YOU ARE DEAD BUT SAVED THE EARTH!"
                messageP=messageP2
                messageE=messageE2
            else: # "YOU ARE DEAD ...BUT THE END IS HERE ANYWAY"
                messageP=messageP2
                messageE=messageE3

    messageS="YOU ARE AT THE "+str(player.location).upper()+", WITH "+str(player.fellow)+" FELLOWS AT EARTH AND "+str(UFO.captive)+" ALIEN CAPTIVES."
    startSX=(int(screen_width/2))-int(len(messageS)/2)*10+infoSpace*3
    startSY=int(screen_height/2)-infoSpace*3
    fontS=pygame.font.Font("resources/AnticSlab-Regular.otf",15)
    msgS=fontS.render(messageS,True,FontColor)

    startPX=(int(screen_width/2))-int(len(messageP)/2)*10
    startPY=int(screen_height/2)-infoSpace
    startEX=(int(screen_width/2))-int(len(messageE)/2)*10
    startEY=int(screen_height/2)+infoSpace
    font=pygame.font.Font("resources/AnticSlab-Regular.otf",20)
    msgP=font.render(messageP,True,FontColor)
    msgE=font.render(messageE,True,FontColor)

    game_display.blit(msgS,(startSX,startSY))
    game_display.blit(msgP,(startPX,startPY))
    game_display.blit(msgE,(startEX,startEY))

    pygame.display.update()
    clock.tick(1)

pygame.quit
sys.exit