import serial
import pygame
import time
import struct

port = "/dev/ttyUSB0"
baud_rate = 115200

screen = pygame.display.set_mode((800, 600))


ser = serial.Serial(port, baud_rate, timeout=1)

time.sleep(2)

clock = pygame.time.Clock()

pygame.event.set_grab(True)
pygame.mouse.set_pos(400, 300)

while True:
    clock.tick(60)
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            quit()


    

    v = pygame.mouse.get_pos()
    if v != (400, 299):
        pygame.mouse.set_pos(400, 299)

    keys = pygame.key.get_pressed()

    byte_keys = 0

    if pygame.mouse.get_pressed()[0]:
        byte_keys += 1

    if keys[pygame.K_w]:
        byte_keys += 2
    if keys[pygame.K_s]:
        byte_keys += 4
    if keys[pygame.K_a]:
        byte_keys += 8
    if keys[pygame.K_d]:
        byte_keys += 16
    if keys[pygame.K_SPACE]:
        byte_keys += 32
    if keys[pygame.K_LSHIFT]:
        byte_keys += 64


    



    data = struct.pack('2iB', *[v[0] - 400 ,v[1] - 299, byte_keys])
    
    ser.write(data)
