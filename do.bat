@echo off
call build.bat 
cd dist 
python -m http.server 8000 
cd ..