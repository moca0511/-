#!/bin/bash
URL=`python3 /home/rakuhoku/slackBot/sub/slackRead_url.py camera_rover`
echo $URL"/?action=stream"
