#include "Function.h"

//ժҪ��fstreamͬʱ��д��Ҫ�ڶ�д֮����ձ�־λ����д�ļ�֮ǰҪ�ѹ��ŵ���ͷ��
//�ļ�����ȷʵ�ж���int�ĺ��������Ǳ���Ŀ��time����࣬�����и���int����time�࣬
//��time���������ļ������>>����������¶����ԣ�����ȫ��ת����string���룬��ת����int��
//DVDstock���ڸ�ֵ��ʱ�����Ĭ�ϸ�ֵ�������Ȼ��������Ҫ�������������Ϊstring���Ѿ������ˣ�
//����time��Ҳ�������أ���Ϊ��Ա����ֻ��3��int

int main()
{
	if (Init())
	{
		while (true)
		{
			RetryWindows(OptionBegin);
		}
	}
	else
	{
		cout << "�ļ���ʧ�ܣ�" << endl;
		exit(0);
	}
	return 0;
}