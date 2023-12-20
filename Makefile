build:
	gcc -o libXnocsd.so main.c `pkgconf --cflags --libs x11` -shared -fPIC
install:
	mkdir -p $(DESTDIR)/usr/libexec/
	install libXnocsd.so $(DESTDIR)/usr/libexec/libXnocsd.so
clean:
	rm -f libXnocsd.so
