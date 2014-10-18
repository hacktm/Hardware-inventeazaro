#!/bin/bash 
cp -r dist /tmp
git commit -am "updated website files"
git checkout gh-pages
cp -r /tmp/dist/* .
git add .
git commit -am "updated website files"
git push 
git checkout master 
git push 
