为了简单起见, 在这个demo中, 我们用 tini 解决僵尸进程的收割问题和信号转发问题.

### 模拟僵尸进程无法收割问题
```
CONTAINER_ID=`docker run --rm -d --entrypoint "zombie-reaping-problem" infmaze/pid1-problems`
watch -n 1 docker exec -it $CONTAINER_ID ps jxf
```
可以看到系统中最后残留了5个zombie进程.

-------

使用tini 收割僵尸进程:
```
CONTAINER_ID=`docker run --rm -d --entrypoint "/tini" infmaze/pid1-problems -- zombie-reaping-problem`
watch -n 1 docker exec -it $CONTAINER_ID ps jxf
```
可以看到僵尸进程已经不存在了.

### 模拟信号转发失败问题

```
# make signal-forwarding-problem subprocess of bash
CONTAINER_ID=`docker run --rm -d --entrypoint "signal-forwarding-problem-entrypoint.sh" infmaze/pid1-problems`
docker exec -it $CONTAINER_ID ps jxf
docker stop --time=30 $CONTAINER_ID # wait 30 seconds before kill container
```

可以看到，执行 `docker stop` 时，容器不会立即停止，main 进程也没有收到任何 signal, 30 秒之后 docker 把容器强制杀死。

----------

用tini 转发signal：
```
# make signal-forwarding-problem subprocess of tini
CONTAINER_ID=`docker run --rm -d --entrypoint "/tini" infmaze/pid1-problems -- signal-forwarding-problem`
docker exec -it $CONTAINER_ID ps jxf
docker stop --time=30 $CONTAINER_ID # wait 30 seconds before kill container
```

可以看到执行 docker stop 之后，容器中的main 进程接收到 SIGTERM 信号，执行了cleanup 函数，并正常退出了.

> 1. 不需要手动stop 实验用的容器，容器会在300s后自动退出并删除。
> 1. 导致容器不能优雅退出，有多种原因，此处只实验了bash 作为 PID1 进程的情况.

### Reference
- [Docker and the PID 1 zombie reaping problem](https://blog.phusion.nl/2015/01/20/docker-and-the-pid-1-zombie-reaping-problem/)
- [Why Your Dockerized Application Isn’t Receiving Signals](https://hynek.me/articles/docker-signals/)
