#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

/* procdump() */
#include "spinlock.h"


int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;

  // For lazy page allocation
  myproc()->sz += n;
  //if(growproc(n) < 0)
    //return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int
sys_perf(void)
{
  char *flag;
  if(argstr(0, &flag) < 0){
    return -1;
  }
  perf(flag);
  return 0;
}

struct {
  struct spinlock lock;
  struct proc proc[NPROC];
} ptable;

int
getTicks(void)
{
  uint xticks;

  xticks = ticks;
  return xticks;
}

int
sys_psched(void)
{
  // process struct pointer
  struct proc *p;
  // ticks
  int tick = 0;

  static char *states[] = {
  [UNUSED]    "unused",
  [EMBRYO]    "embryo",
  [SLEEPING]  "sleep ",
  [RUNNABLE]  "runble",
  [RUNNING]   "run   ",
  [ZOMBIE]    "zombie"
  };  
  char *state;

  //cprintf("Task\t|\tRuntime ms\t|\tSwitches\t|\tAverage delay ms\t|\tMaximum delay ms\n");
  cprintf("PID\t|\tProcess State\t|\tTicks\t|\tName\t\n");  

  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++)
  {
    //break condition
    if(p->pid == 0) break;
    
    if(p->state == UNUSED)
      continue;
    if(p->state >= 0 && p->state < NELEM(states) && states[p->state])
      state = states[p->state];
    else
      state = "???";
    
    //print process information
    cprintf("%d", p->pid);
    cprintf("\t|\t");
    cprintf("%s", state);
    cprintf("\t|\t");
    cprintf("%d", tick = getTicks());
    cprintf("\t|\t");
    cprintf("%s", p->name);
    cprintf("\n");
    
  }
  //procdump();
  return 1;
}
/*
void
procdump(void)
{
  static char *states[] = {
  [UNUSED]    "unused",
  [EMBRYO]    "embryo",
  [SLEEPING]  "sleep ",
  [RUNNABLE]  "runble",
  [RUNNING]   "run   ",
  [ZOMBIE]    "zombie"
  };
  int i;
  struct proc *p;
  char *state;
  uint pc[10];

  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
    if(p->state == UNUSED)
      continue;
    if(p->state >= 0 && p->state < NELEM(states) && states[p->state])
      state = states[p->state];
    else
      state = "???";
    cprintf("%d %s %s", p->pid, state, p->name);
    if(p->state == SLEEPING){
      getcallerpcs((uint*)p->context->ebp+2, pc);
      for(i=0; i<10 && pc[i] != 0; i++)
        cprintf(" %p", pc[i]);
    }
    cprintf("\n");
  }
}
*/