#!/bin/bash

# 获取网络时间
network_time=$(curl -s http://worldtimeapi.org/api/timezone/Asia/Shanghai | jq -r '.datetime')

# 检查是否成功获取时间
if [ -z "$network_time" ]; then
 echo "Failed to get network time"
 exit 1
fi

# 提取日期和时间部分
date=$(echo "$network_time" | cut -c1-10)  # 日期部分
time=$(echo "$network_time" | cut -c12-19)  # 时间部分

# 输出日期和时间
echo "$date $time"
