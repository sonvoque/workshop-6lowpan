DEFINES+=PROJECT_CONF_H=\"project-conf.h\"

all: hello-world


PROJECTDIRS += ./res
PROJECTDIRS += ./dev
PROJECT_SOURCEFILES += res-hello.c
PROJECT_SOURCEFILES += res-temp.c
PROJECT_SOURCEFILES += res-hum.c
PROJECT_SOURCEFILES += hih6130.c

APPS += er-coap
APPS += rest-engine

CONTIKI = ../contiki/
include $(CONTIKI)/Makefile.include
