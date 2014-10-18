#!/bin/bash 
cp -r website/dist /tmp
git commit -am "updated website files"
git checkout gh-pages
cp -r /tmp/dist/* .
git commit -am "updated website files"
git checkout master 
