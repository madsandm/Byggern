#!/bin/bash

avrdude -p m162 -c atmelice -U lfuse:w:0xFD:m -U hfuse:w:0x19:m -U efuse:w:0xFF:m -U lock:w:0xFF:m
