
import pygame
from random import randint
from copy import deepcopy

WIDTH, HEIGHT = 1000, 700
tile = 20
w, h = WIDTH // tile, HEIGHT // tile
fps = 30

pygame.init()
sc = pygame.display.set_mode((WIDTH, HEIGHT))
clock = pygame.time.Clock()

next_field = [[0 for i in range(w)] for j in range(h)]
current_field = [[randint(0, 1) for i in range(w)] for j in range(h)]


def check_cell(current_field, x, y):
    count = 0
    for i in range(y - 1, y + 2):
        for j in range(x - 1, x + 2):
            if current_field[i][j]:
                count += 1

    if current_field[y][x]:
        count -= 1
        if count == 2 or count == 3:
            return 1
        return 0

    else:
        if count == 3:
            return 1
        return 0


while True:

    sc.fill(pygame.Color('black'))
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            exit()

    [pygame.draw.line(sc, pygame.Color('dimgray'), (x, 0), (x, HEIGHT)) for x in range(0, WIDTH, tile)]
    [pygame.draw.line(sc, pygame.Color('dimgray'), (0, y), (WIDTH, y)) for y in range(0, HEIGHT, tile)]

    for x in range(1, w-1):
        for y in range(1, h-1):
            if current_field[y][x]:
                pygame.draw.rect(sc, pygame.Color('green'), (x * tile + 2, y * tile + 2, tile - 2, tile - 2))
            next_field[y][x] = check_cell(current_field, x, y)

    current_field = deepcopy(next_field)
    
    pygame.display.flip()
    clock.tick(fps)
