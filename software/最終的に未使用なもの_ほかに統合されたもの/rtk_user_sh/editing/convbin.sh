# convbin.shのconfig_master.txtへの対応
#ログのディレクトリ名になっている日付を引数にする
if [ $# != 1 ]; then
    echo 引数エラー: $*
    exit 1
else
    echo OK
fi

. ./config.txt

OBS_DIR=obs/
OBS_PATH=${logDirPath}/$1${OBS_DIR}

ROVER_DATA=roverLog
BASE_DATA=baseLog
OUTPUT_FILE=result_after.pos

#-v(ver rinex version) -f(freq number of frequencies(L1やL2の数？))の数値は状況に合わせて調整する
cd RTKLIB/app/convbin/gcc
./convbin -od -os -oi -ot -f 1 -v 2.11 ${DATA_DIR}${ROVER_DATA}.txt -r ubx -d ${OBS_PATH}
./convbin -od -os -oi -ot -f 1 -v 2.11 ${DATA_DIR}${BASE_DATA}.txt -r rtcm3 -d ${OBS_PATH}

cd ../../rnx2rtkp/gcc/
./rnx2rtkp -k ${CONFIG_FILE} ${OBS_PATH}${ROVER_DATA}.obs ${OBS_PATH}${BASE_DATA}.obs ${OBS_PATH}${ROVER_DATA}.nav -o ${OBS_PATH}${OUTPUT_FILE}
