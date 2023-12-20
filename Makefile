build:
	$(CC) -o libXnocsd.so main.c -O2 -s \
	    `pkg-config --cflags --libs x11` \
	     -shared -fPIC -nostdlib -lc
install:
	mkdir -p $(DESTDIR)/usr/libexec/
	install libXnocsd.so $(DESTDIR)/usr/libexec/libXnocsd.so
clean:
	rm -f libXnocsd.so
