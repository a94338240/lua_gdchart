all:
	gcc -std=gnu99 -Wall -Werror -fPIC --shared -o lua_gdchart.so lua_gdchart.c `pkg-config --libs --cflags lua5.2` -lgdc
	strip --strip-debug --strip-all lua_gdchart.so
	chmod 755 test.lua

clean:
	rm -f *~ lua_gdchart.so
