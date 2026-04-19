#!/bin/sh

pio run -e native_run &&
	./.pio/build/native_run/program
