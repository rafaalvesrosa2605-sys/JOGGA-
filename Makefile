# JOGGA - Makefile
#   make        compila o programa
#   make run    compila e executa
#   make test   compila e roda os testes
#   make clean  apaga os executaveis

CC      = gcc
CFLAGS  = -Wall -Wextra -std=c99 -pedantic

CAMADAS = $(wildcard src/repositorio/*.c) \
          $(wildcard src/servico/*.c)

TELAS   = $(wildcard src/apresentacao/*.c)

all: jogga

jogga: main.c $(CAMADAS) $(TELAS)
	$(CC) $(CFLAGS) -o $@ main.c $(CAMADAS) $(TELAS)

run: jogga
	./jogga

test: tests/test_servicos
	./tests/test_servicos

tests/test_servicos: tests/test_servicos.c $(CAMADAS)
	$(CC) $(CFLAGS) -o $@ tests/test_servicos.c $(CAMADAS)

clean:
	rm -f jogga jogga.exe tests/test_servicos tests/test_servicos.exe

.PHONY: all run test clean
