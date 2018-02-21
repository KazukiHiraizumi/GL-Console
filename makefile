all:
	c++ -DX11 glokn.c objview.c cparser.c -lglfw -lGL -lGLU -lm -o glcon

kio:
	c++ -DKIOSK -DX11 glokn.c objview.c cparser.c -lglfw -lGL -lGLU -lm -o glcon

install:
	cp glcon /usr/local/bin
