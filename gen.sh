#!/bin/sh

if [[ $1 == "clean" ]]; then
	rm -rf cmake-xcode;
	rm -rf cmake-build;

elif [[ $1 = "xcode" ]]; then
	mkdir -p cmake-xcode;
	cd cmake-xcode;
	cmake -GXcode ..;

else
	mkdir -p cmake-build;
	cd cmake-build;
	cmake ..;
fi
