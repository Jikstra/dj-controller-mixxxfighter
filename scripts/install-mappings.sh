#!/bin/bash

python ./mappings/generate-mappings.py > ~/.mixxx/controllers/MixxxFighter.midi.xml
cp ./mappings/MixxxFighter.midi.js ~/.mixxx/controllers/
