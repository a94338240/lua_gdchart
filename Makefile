all: lua_gdchart.so

lua_gdchart.so: lua_gdchart.c
	gcc -std=gnu99 -Wall -Werror -fPIC --shared -o lua_gdchart.so lua_gdchart.c `pkg-config --libs --cflags lua5.2` -lgdc
	chmod 755 test.lua

install: lua_gdchart.so
	install -m 0775 -o root -d /usr/local/lib/lua/5.2/
	install -m 0664 -o root -s lua_gdchart.so /usr/local/lib/lua/5.2

clean:
	rm -f *~ lua_gdchart.so
