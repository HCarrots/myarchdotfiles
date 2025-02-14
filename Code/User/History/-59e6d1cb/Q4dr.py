# -*- coding: utf-8 -*-
import sys
import json
import os
import re
import subprocess
import threading
import time
from datetime import datetime
from dependence import file_selector 
from dependence.path_loader import path_loader


script_name = os.path.splitext(os.path.basename(__file__))[0]
#experiment_names = [t]
#data_ranges = []

max_threads = 100
partition_input = ''

iscript = 1

# 检查命令行参数的数量
#if len(sys.argv) > 5:
    ## sys.argv[0] 是脚本的名称，所以参数从 sys.argv[1] 开始
    #experiment_names.append(sys.argv[1]) 
    #data_ranges.append(sys.argv[2])
    #partition_input = sys.argv[3]
    #max_threads = int(sys.argv[4])
    #iscript = int(sys.argv[5])
    #print(f"{experiment_names}, {data_ranges}, {max_threads}, {partition_input}")
#else:
    #print("没有提供足够的参数，需要5个参数。")


#data_dir = 'RootDataDir'
# 调用 select_files 函数，设置 print_files=True 以打印所有选定的文件
#all_selected_files = file_selector.select_files(experiment_names, data_ranges, data_dir, print_files=True)

#Nfile=1 non-meaning
Nfile=1
all_selected_files = ['/home/hooke/works/data//works/data/FEE.fee01.20240305012348.001.root']


# 日志文件的名称
log_filename = f"{script_name}-log.txt"
# 打开日志文件准备写入
with open(log_filename, "a") as log_file:
    # 要执行的命令列表
    a = 0
    total = len(all_selected_files)
    for file in all_selected_files:
        a += 1
        current_time = datetime.now()
        formatted_datetime = current_time.strftime("%Y-%m-%d %H:%M:%S")
        start_time = time.time()
        filename = os.path.basename(file)
        filename_onlyname,extension = os.path.splitext(filename)
        #event_data_dir = path_loader().get('WaveDataDir')
        event_data_dir = '/home/hooke/works/wnri-bluet-v5/output'
        sourcefile_dir= f"{event_data_dir}/{filename_onlyname}"
        print(sourcefile_dir)
        commands =[]
        for i in range(max_threads):
            command = ['srun', '-p', partition_input, '../build/yihBluetWork/BluetWaveFactory', str(file), str(max_threads), str(i), str(Nfile), sourcefile_dir]
            commands.append(command)
        mkdir = ['mkdir', sourcefile_dir]
        subprocess.Popen(mkdir, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, universal_newlines=True, bufsize=1)
            
        sem = threading.Semaphore(max_threads)

        def execute_command(cmd):
            with sem:
                """执行命令并实时打印输出。"""
                # 启动进程
                process = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, universal_newlines=True, bufsize=1)
                # 实时打印输出
                while True:
                    output = process.stdout.readline()
                    if not output and process.poll() is not None:
                        break
                    if output:
                        print(output.strip())

        threads = []
        for cmd in commands:
            thread = threading.Thread(target=execute_command, args=(cmd,))
            threads.append(thread)
            thread.start()

        for thread in threads:
            thread.join()

        sourcefiles = []
        for ijob in range(max_threads):
            sourcefiles.append(sourcefile_dir+'/_'+str(ijob)+'.root')
        
        haddcmd = ['hadd', '-f', event_data_dir+'/'+filename]+sourcefiles
        hadd = subprocess.Popen(haddcmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, universal_newlines=True, bufsize=1)
        while True:
            output = hadd.stdout.readline()
            if not output and hadd.poll() is not None:
                break
            if output:
                print(output.strip())
        print("所有命令执行完毕。")      
        # 记录循环结束的时间，并计算持续时间
        end_time = time.time()
        duration = end_time - start_time

        # 将循环次数和持续时间写入日志文件
        end_time = datetime.now()
        formatted_endtime = end_time.strftime("%Y-%m-%d %H:%M:%S")
        log_file.write(f"{formatted_datetime}-{formatted_endtime}: script {iscript}: {a}/{total} 执行时间:{duration} 秒\n;WavetDataDir/{filename};{partition_input},max_threads={max_threads}:")
        print(f"{formatted_datetime}-{formatted_endtime}: script {iscript}: {a}/{total}")
        rmcmd = ['rm', '-r', sourcefile_dir]
        rm = subprocess.Popen(rmcmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, universal_newlines=True, bufsize=1)