import pygame,sys
import time
from pygame.locals import *
pygame.font.init()
myfont = pygame.font.SysFont('Comic Sans MS', 30)
color = (250,250,250)
height = 1000
width = 1000
screen = pygame.display.set_mode((width,height))
pygame.display.set_caption('N-body simulations')
screen.fill(color)
clock = pygame.time.Clock()
n = 0
N = 128
timestep = 0.05
t = 10

F = open("output.txt","r")
coor = []
for line in F.readlines():
    x = map(float,line.split())
    x[0] = int(x[0]*100 + 400)
    x[1] = int(x[1]*100 + 300)
    coor.append(x)

while (n < (N*10)/timestep):
    for event in pygame.event.get():
        if event.type == QUIT:
            pygame.quit()
            sys.exit()
    screen.fill(color)
    for i in range(N):
        pygame.draw.circle(screen, (0,0,255), coor[n+i], 5)
    n += N
    time.sleep(0.05)
    pygame.display.update()
