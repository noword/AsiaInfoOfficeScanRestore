# 亚信 OfficeScan 本地隔离文件恢复工具

SB亚信提供的本地隔离文件恢复工具只有 GUI 界面，不能批量恢复。

这个工具可以解决这个问题。

* 注意：需要把 `VSAPI32.DLL`复制到与 `asia_restore.exe` 同一目录，在命令行下运行 `asia_restore.exe`。
* 使用风险自负

## 用法


```
Usage: asia_restore [qtn file] | --all

Example:
Restore a signle file:
    asia_restore "C:\Program Files (x86)\AsiaInfo\OfficeScan Client\SUSPECT\Backup\somefile.qtn"

Restore all:
    asia_restore --all
```