/**
 * 不动笔墨不读书系列
 * 合理控制优先级 根据职责赋予权利
 * linux是一个多用户多任务的操作系统，即使是在单核心CPU的系统上，用户也感觉系统中的多个进程在同时进行
 * 实际上，内核使用的是一套固定的调度策略，多个进程轮流使用CPU，并快速地轮流切换，但是对于不同功能的进程
 * 它们的任务优先级和对实时性的要求是不一样的，有什么办法可以控制进程这一行为
 * 
 * 控制进程
 * 
 * 普通进程优先级
 * 内核以时间片轮转的方式被依次调度
 * 
 * 内核暂时调度出CPU，转而调入其他符合运行条件的进程继续执行
 * 进程自动休眠(sleep)
 * 进程因为执行了阻塞的IO操作，而需要等待慢速IO完成
 * 进程已经用光了当前分配给自己的时间片
 * 默认情况下，所有进程都会被分配相同长度的时间片
 * 
 * 如果一个进程执行的计算工作特别重要 需要特别多的时间
 * 进程系统闲下来的时候才去执行 提供内部数据审计功能的验证进程
 * Nice值 top命令的输出，包含进程的优先级信息
 * PR代表优先级 0-39
 * NI代表Nice值 -20-19
 * 数值越小，要求获得的CPU时间越多
 * PR = 20+NI
 * Nice就是和善，不抢
 * 控制优先级
 * **/
int setpriority(int which,id_t who,int prio);
int getpriority(int which,id_t who);

/**
 * which 和 who参数联合指定要操作的进程和进程组
 * 
 * 使用getpriority()系统调用之前需要先把errno显式设置为0
 * 
 * 调整优先级的限制
 * 施加一定的限制条件，防止进程被滥用
 * 
 * 实时进程优先级
 * 只能影响内核进行进程调度的权重因子，能影响内核选择让哪个进程获得CPU的偏向程度
 * 但是不能完全保证调度的延迟时间，更不能精确控制调度的顺序
 * 但有一些实时应用却可能对这些有更严格的要求
 * 系统调用，控制实时进程的调度策略和优先级
 * SCHED_RR
 * SECHD_FIFO
 * 1-99 优先维护一个任务队列
 * SCHED_RR调度策略，为每个进程都分配一个固定长度的时间片，如果有优先级相同的进程，会以循环时间分享的形式交替执行
 * 交替执行 时间片用完就排后面
 * 
 * fifo就不是，一旦进程获得运行权，跑完再说
 * 
 * 实时进程调度的参数控制
 * 
 * 设置或修改实时进程调度策略和优先级的系统调用是
 * **/

int sched_setscheduler(pid_t pid,int policy,const struct sched_param *param);
 /**
  policy表示指定进程采用的调度策略。
 * 如果决定采用实时调度策略，则可以取值 SCHED_RR、SCHED_FIFO
 * 如果希望采用非实时的普通调度策略，则也可设置为 SCHED_OTHER（普通时间片轮转调度）、SCHED_BATCH（适用于批处理的进程）、SCHED_IDLE（比最大的 Nice 值的运行优先级还要低的后台进程）。
 * sched_param结构定义了一个包含调度优先级的参数
 * 
 * **/
struct sched_param{
    int sched_priority;
};
/**
 * 如果只需要修改实时进程的优先级，而不需要修改使用的调度策略，还可以使用一个更简单的系统调用
 * 
 * **/
int sched_setparam(pid_t pid,const struct sched_param *param);
/**
 * linux还提供了一个让调用进程能主动放弃CPU的系统调用
 * **/
int sched_yield(void);
/**
 * 该系统调用会把调用进程移动到同优先级队列的队尾，从而主动让出CPU，让同优先级的其他进程有机会运行
 * 
 * 实时策略的调度限制
 * 对实时进程来说，优先级较高的可运行进程总是会优先于优先级较低的进程
 * 而且采用实时策略的进程的优先级要高于采用时间片轮转调度策略的普通进程
 * 
 * 只有系统中对时效性有特殊要求的服务进程才会采用实时调度策略
 * rt表示实时调度策略 NI值无效
 * 
 * 进程在CPU之间的歉意时产生的性能影响主要来源于对CPU的高速缓存中数据的处理
 * 
 * **/
