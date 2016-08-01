DESTDIR ?=/usr/local
BROKER ?= mqtt.bitraf.no
DOOR ?= boxy2

all:

bitraf-door-subscriber.service: bitraf-door-subscriber.service.tmpl
	sed -e "s/@DOOR@/${DOOR}/" -e "s/@BROKER@/${BROKER}/" $< > $@

install-scripts:
	mkdir -p $(DESTDIR)/bin
	cp bitraf-door-publisher $(DESTDIR)/bin
	cp bitraf-door-subscriber $(DESTDIR)/bin
	cp bitraf-door-sanitize-delay $(DESTDIR)/bin

install-service: bitraf-door-subscriber.service
	cp bitraf-door-subscriber.service /etc/systemd/system/
	systemctl daemon-reload
	systemctl enable bitraf-door-subscriber

install: install-scripts install-service

.PHONY: bitraf-door-subscriber.service
