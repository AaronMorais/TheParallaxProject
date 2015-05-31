#!/usr/bin/env bash
git pull

cd legoizer
make run

screen -X -S worker quit
cd ../worker
source venv/bin/activate
pip install -r requirements.txt
screen -S worker -d -m python ~/TheParallaxProject/worker/worker.py

screen -X -S server quit
kill -9 $(ps aux | grep '\snode\s' | awk '{print $2}')
cd ../web-server
npm install
cd ~/
screen -S server -d -m bash -c "sudo node ~/TheParallaxProject/web-server/server.js"