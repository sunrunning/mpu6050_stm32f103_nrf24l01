# src Makefile

include ../Makefile.common

OBJS+=startup.o
OBJS+=main.o
OBJS+=uart.o
OBJS+=spi.o
OBJS+=nrf24l01.o


all: src

src: app.a

app.a: $(OBJS)
	$(AR) cr app.a $(OBJS)
.PHONY: src clean tshow

clean:
	rm -f app.a $(OBJS)
tshow:
		@echo "######################################################################################################"
		@echo "################# optimize settings: $(InfoTextLib), $(InfoTextSrc)"
		@echo "######################################################################################################"
