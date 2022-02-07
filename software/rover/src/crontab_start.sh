#!/bin/sh
# crontab で毎分動作チェックを行う

cd "$(dirname "$0")" || exit

sleep 3

GPIO_TMP_FILE=/tmp/reboot_GPIO.tmp
PS="$(ps aux | grep ./rover.out | grep -v grep)"
BODY_SH="$(echo ${PS} )"

echo "${PS}"
echo "${BODY_SH}"

if [ -e  "${GPIO_TMP_FILE}" ]; then
	rm "${GPIO_TMP_FILE}"
	echo 0 > /tmp/start_cron.log
	echo rm
	# 空の時のみ
	if [ -z "$BODY_SH" ]; then
		echo body
		sleep 3
		sh body.sh
	fi
fi
