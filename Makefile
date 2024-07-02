
library:
	aarch64-none-linux-gnu-gcc-ar rcs lib/static/libled.a obj/led.o
	aarch64-none-linux-gnu-gcc -shared -o lib/shared/libbh1750.so obj/bh1750.o

link:
	aarch64-none-linux-gnu-gcc obj/mqtt.o -L/home/duy/Desktop/mosquitto-2.0.18/build/lib -lmosquitto -pthread -Llib/static -lled -Llib/shared -lbh1750 -o bin/mqtt
	
object:
	aarch64-none-linux-gnu-gcc -c src/bh1750.c -o obj/bh1750.o -I./inc -fPIC	
	aarch64-none-linux-gnu-gcc -c src/led.c -o obj/led.o -I./inc
	aarch64-none-linux-gnu-gcc -c mqtt.c -o obj/mqtt.o -I/home/duy/Desktop/mosquitto-2.0.18/include -I./inc

clean:
	rm -rf obj/* bin/* lib/static/* lib/shared/*

