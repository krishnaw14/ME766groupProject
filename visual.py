import pygame,sys
import time
from pygame.locals import *
pygame.font.init()
myfont = pygame.font.SysFont('Comic Sans MS', 30)
color = (0,40,40)
height = 1000
width = 1000
screen = pygame.display.set_mode((width,height))
pygame.display.set_caption('N-body visualization')
screen.fill(color)
clock = pygame.time.Clock()
n = 3840
F = open("output.txt","r")
coor = []
for line in F.readlines():
    x = map(float,line.split())
    x[0] = int(x[0]*100 + 400)
    x[1] = int(x[1]*100 + 300)
    coor.append(x)

while True:
    for event in pygame.event.get():
        if event.type == QUIT:
            pygame.quit()
            sys.exit()
    screen.fill(color)
    for i in range(128):
        pygame.draw.circle(screen, (255,255,0), coor[n+i], 5)
    #n += 128
    time.sleep(0.05)
    pygame.display.update()
