#include "apue.h"
#include <signal.h>

static volatile sig_atomic_t sigflag;
static sigset_t newmask, oldmask, zeromask;

/*one signal handler for SIGUSR1 and SIGUSR2*/
static void sig_usr(int signo)
{
	sigflag = 1;
}

void TELL_WAIT(void)
{
	if (SIG_ERR == signal(SIGUSR1, sig_usr))
		err_sys("signal (SIGUSR1) error");
	if (SIG_ERR == signal(SIGUSR2, sig_usr))
		err_sys("signal (SIGUSR2) error");
	sigemptyset(&zeromask);
	sigemptyset(&newmask);
	sigaddset(&newmask, SIGUSR1);
	sigaddset(&newmask, SIGUSR2);
	
	/*Block SIGUSR1 and SIGUSR2, and save current signal mask*/
	if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
		err_sys("SIG_BLOCK error");
}

void TELL_PARENT(pid_t pid)
{
	kill(pid, SIGUSR2);
}

void WAIT_PARENT(void)
{
	while (0 == sigflag)
		sigsuspend(&zeromask);
	sigflag = 0;
	
	if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
		err_sys("SIG_SETMASK error");
}

void TELL_CHILD(pid_t pid)
{
	kill(pid, SIGUSR1);
}

void WAIT_CHILD(void)
{
	while (0 == sigflag)
		sigsuspend(&zeromask);
	sigflag = 0;
	
	if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
		err_sys("SIG_SETMASK error");
}
