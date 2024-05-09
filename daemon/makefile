all: build run

buildcpp:
	cmake .. && make -f Makefile

buildgo:
	cd godemo && go build -o demo-bin && mv  demo-bin ../

build: buildgo buildcpp

cpbin:
	cp daemon/daemon-bin daemon-bin

run: cpbin
	./daemon-bin ./demo-bin -f 123