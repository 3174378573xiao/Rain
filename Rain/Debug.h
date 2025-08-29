
//rain���� �������з�����Եĺ���
//���ߣ�xiao
//github:3174378573xiao
//qq:3174378573#pragma once
#pragma once
#include <iostream>
#include <ostream>
using namespace std;

//debug_log
//��δ����DEBUG��ʱ��debug_log��䲻����������ڵ���
//�÷���cout��ȫ��ͬ
#ifdef DEBUG
// ���� DEBUG ʱ��dout �ض��� std::cerr���� std::debug_log��
#define debug_log std::cerr
#else
// δ���� DEBUG ʱ��ʹ�ÿ�������
class NullStream {
public:
	// �����������͵�������޲�����
	template<typename T>
	NullStream& operator<<(const T&) {
		return *this;
	}

	// ���������������� std::endl��std::hex �ȣ�
	NullStream& operator<<(std::ostream& (*)(std::ostream&)) {
		return *this;
	}
};

// ȫ�ֿ���ʵ��
inline NullStream null_stream;
#define debug_log null_stream
#endif



//��ӡ�����������Ϣ
//argc ���������������argv�������������
void argPrint(int argc, char* argv[]) {
	debug_log << "argc���������������:" << argc << endl;
	//debug_log <<  << argv[0] << endl;
	debug_log << "argv(�������)��" << endl;
	for (int i = 0; i < argc; i++)
	{
		debug_log << i << "|";
		if (i == 0) debug_log << "��������·��:";
		debug_log << argv[i] << endl;
	}
}
