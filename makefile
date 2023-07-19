main: main.o interface.o inputs.o gfxlib/libisentlib.a engine.o genpiece.o persistance.o sound.o
	mkdir build -p
	cp -r image build/image
	cp -r sound build/sound
	gcc -Wall main.o interface.o inputs.o genpiece.o engine.o persistance.o sound.o -o build/main gfxlib/libisentlib.a -lm -lglut -lGL -lX11 -I/usr/local/include -L/usr/local/lib -lfmod
	rm *.o -f

main.o: src/main.c src/interface.h
	gcc -c src/main.c -Wall

interface.o: src/interface.c src/interface.h
	gcc -c src/interface.c -Wall

inputs.o: src/inputs.c src/inputs.h
	gcc -c src/inputs.c -Wall

genpiece.o: src/genpiece.c src/genpiece.h
	gcc -c src/genpiece.c -Wall

engine.o: src/engine.c src/engine.h
	gcc -c src/engine.c -Wall

sound.o: src/sound.c src/sound.h
	gcc -c src/sound.c -Wall

persistance.o: src/persistance.c src/persistance.h
	gcc -c src/persistance.c -Wall


#installe les dépendance sur le system
prepare:
	sudo apt install freeglut3-dev
	sh makegfxlib.sh
	sudo sh installfmod.sh

clean:
	rm *.o -f
