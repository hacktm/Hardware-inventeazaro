#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"


echo $DIR
cd $DIR

cp -r website/dist /tmp
git add website
git commit -am "updated website files"
git checkout gh-pages
cp -r /tmp/dist/* .
git add .
git commit -am "updated website files"
git push

git checkout master
git push
