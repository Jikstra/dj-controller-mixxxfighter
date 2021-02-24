#!/bin/bash

python ./scripts/generate-mappings.py > ~/.mixxx/controllers/MixxxFighter.midi.xml
cp ./scripts/MixxxFighter.midi.js ~/.mixxx/controllers/
