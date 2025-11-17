#!/bin/bash

python3 -m venv .venv
source ./.venv/bin/activate
pip install -r gpu-requirements.txt
pip install -r requirements.txt
