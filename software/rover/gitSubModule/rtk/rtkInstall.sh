#!/bin/bash

cd "$(dirname "$0")" || exit
./init.sh
cp .gitignore ./RTKLIB
echo .gitignore >>./RTKLIB/.gitignore

cd RTKLIB/app/consapp/str2str/gcc || exit
make
sudo cp str2str /usr/local/bin
