#!/usr/bin/env bash
# git submodule add https://github.com/i-naoyuki/baseRover.git 
cd "$(dirname $0)" || exit
git submodule update -i
cp ./sparse-checkout ./../../.git/modules/src/test/subModule/setting/baseRover/info
cd baseRover || exit
git config core.sparsecheckout true
git read-tree -mu HEAD

# https://qiita.com/SLEAZOIDS/items/f707b6292a763a8e47e9#initsh%E3%81%AE%E8%AA%AC%E6%98%8E
