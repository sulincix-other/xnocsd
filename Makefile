build:
	$(CC) -o libXnocsd.so main.c -O2 -s \
	    `pkg-config --cflags --libs x11` \
	     -shared -fPIC -nostdlib -lc
install:
	mkdir -p $(DESTDIR)/usr/libexec/ $(DESTDIR)/etc/X11/xinit/xinitrc.d
	install libXnocsd.so $(DESTDIR)/usr/libexec/libXnocsd.so
	install xnocsd.xinit $(DESTDIR)/etc/X11/xinit/xinitrc.d/52-xnocsd
clean:
	rm -f libXnocsd.so
