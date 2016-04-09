DESTDIR ?=/usr/local

all:

install:
	mkdir -p $(DESTDIR)/bin
	cp bitraf-door-publisher $(DESTDIR)/bin
	cp bitraf-door-subscriber $(DESTDIR)/bin
	cp bitraf-door-sanitize-delay $(DESTDIR)/bin
	@echo "To install subscriber, do this:"
	@echo "sudo cp bitraf-door-subscriber.service /etc/systemd/system"
	@echo "sudo vim /etc/systemd/system/bitraf-door-subscriber.service"
	@echo "sudo systemctl daemon-reload"
	@echo "sudo systemctl enable bitraf-door-subscriber"
