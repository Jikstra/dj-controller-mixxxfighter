#!/bin/bash

python ./scripts/generate-mappings.py > ~/.mixxx/controllers/MiniX1.midi.xml
cp ./scripts/MiniX1.midi.js ~/.mixxx/controllers/
