#!/bin/bash
cd "$(dirname "$0")" || exit

cd main || exit

cd server/ || exit
make
cd ../

cd m5/ || exit
make
cd .. || exit

cd rover/ || exit
make
cd .. || exit

cd relay_with_server/transfer || exit
make
cd ../.. || exit

cd INA || exit
make
cd .. || exit


