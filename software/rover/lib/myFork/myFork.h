#ifndef _MY_FORK_HPP_
#define _MY_FORK_HPP_

#define P_MAX 10 //プロセス数

class MY_FORK
{
private:
	int pid[P_MAX], status, pidElement, p_max;
	bool flgPrint = true;
	char temp_str[50];

	void start();
	void forkPrint(const char str[]);
	int getElement() { return pidElement; }
	int getPid(const int i) { return pid[i]; };

public:
	MY_FORK(const bool _flgPrint = true, const int = 2);
	~MY_FORK();
	void parent_wait_child();
	int ifMyFork(const bool waitOnPatent);
	int ifMyFork(void (*const pf[])(void), const int, const bool waitOnPatent = true);
	int getP_Max() { return p_max; }
	void myWait(const int i);
	void myKill(const int i);
};

#endif
