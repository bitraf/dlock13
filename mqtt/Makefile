DESTDIR ?=/usr/local
BROKER = bomba.bitraf.no
DOOR = boxy2

all:

bitraf-door-subscriber.service: bitraf-door-subscriber.service.tmpl
	sed -e "s/@DOOR@/${DOOR}/" -e "s/@BROKER@/${BROKER}/" $< > $@

install: bitraf-door-subscriber.service
	mkdir -p $(DESTDIR)/bin
	cp bitraf-door-publisher $(DESTDIR)/bin
	cp bitraf-door-subscriber $(DESTDIR)/bin
	cp bitraf-door-sanitize-delay $(DESTDIR)/bin
	@echo "To install subscriber, do this:"
	@echo "sudo cp bitraf-door-subscriber.service /etc/systemd/system"
	@echo "sudo vim /etc/systemd/system/bitraf-door-subscriber.service"
	@echo "sudo systemctl daemon-reload"
	@echo "sudo systemctl enable bitraf-door-subscriber"
