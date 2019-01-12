可通过 lock --help 查看帮助
可通过命令行实现加密
format： lock hello.exe hello2.exe

lock.exe会打开 shell.bin 文件生成shelldat.bin（shelldat.bin 为 shell.bin + 原 exe$完整的文件头），加密并修改重定位

