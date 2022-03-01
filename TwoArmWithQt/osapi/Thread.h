
#ifndef _OSAPI_THREAD_H

class OS_Thread
{
public:
	OS_Thread();
	virtual ~OS_Thread();

	// ����������
	virtual int Run();

	// �ȴ����ջ���Դ
	static void Join(OS_Thread* thrd);

	// Sleep����
	static void Msleep(int ms);
	static void Sleep(int s);

public:
	virtual int Routine() = 0;

private:
	void* m_Priv;
};


#endif

