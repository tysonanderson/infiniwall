.PHONY: clean

BINARIES = anaerobic_tracker cat_shm cli_incline cli_log cli_speed heartrate \
           hr_tracker kiosk_sim logger mkshm scripted_session timestamp \
           ct5k_w1

BINDIR = ./bin/
SOURCEDIR = ./src/
LIBSHMDIR = $(SOURCEDIR)libshm_vars/
LIBSHM = $(LIBSHMDIR)libshm_vars.a
TARGET_LIST = $(addprefix $(BINDIR), $(BINARIES))
INCLUDES = -I./src -I./src/libshm_vars
CC = g++

LDFLAGS = -L./src/libshm_vars -lshm_vars -lpthread -lrt 

all: $(TARGET_LIST)

$(BINDIR)%: $(SOURCEDIR)%.cpp $(LIBSHM)
	$(CC) $< $(INCLUDES) $(LDFLAGS) -o $@

$(LIBSHM): $(LIBSHMDIR)Shm.h $(LIBSHMDIR)Shm_vars.h  \
           $(LIBSHMDIR)Shm_vars.cpp $(LIBSHMDIR)Command.cpp
	
	cd $(LIBSHMDIR); \
	g++ -c Command.cpp; \
	g++ -c Shm_vars.cpp; \
	ar -cq libshm_vars.a Shm_vars.o Command.o; \
	rm -f  Shm_vars.o Command.o 

clean:
	for target in $(BINARIES); do \
	  rm -f $(BINDIR)/$$target; \
        done
	rm -f $(LIBSHMDIR)Shm_vars.o $(LIBSHMDIR)Command.o
	rm -f $(LIBSHM)
