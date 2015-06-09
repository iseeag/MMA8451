# You need WiringPi which you can get from wiringpi.com/download-and-install/ 

all: demo 

HDRS = wiringPi_MMA8451.h
MMA_OBJS = wiringPi_MMA8451.o

CXXFLAGS = -Wall 

$(MMA_OBJS) : $(HDRS)

demo: $(MMA_OBJS) 
	$(CXX) -v -o $@ $^ -lwiringPi

clean: 
	rm -f $(MMA_OBJS) demo
