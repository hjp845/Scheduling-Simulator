#include<iostream>
#include<algorithm>
using namespace std;

int many_task;
int many_cpu;
int time = 0;


class task {
public:
	int number;
	int period;
	int execution;
	int ddl=100;
	int work_time=0;
	int zl=0;
	int work_on=0;  // �ٸ� cpu���� ���� �ϰ� �ִ��� Ȯ���ϱ� ���� �����ϻ��̴�.
	int worked=1; //���� �س����°�? start ������ �⺻���� 1�� �Ѵ�.
	int d;
	int start=0;
	

	bool operator <(task &t) {
		if (this->worked == t.worked){
			if (this->work_on == t.work_on)	{
				if (this->zl == t.zl){
					if (this->ddl == t.ddl)	{return this->number < t.number;}
					else{return this->ddl < t.ddl;}
				}else{return this->zl > t.zl;}
			}else{return this->work_on < t.work_on;}
		}else{return this->worked < t.worked;}
	}
};
task* tasks = new task[10];



class cpu {
public:
	int number;
	int whatwork;
	int speed = 1;
	int todo = 1;

	void work() {
		for (int i = 0; i < this->speed; i++)  //---------------------------------------------------------------------speed��ŭ �ϴٰ� �ϳ��� �Ϸ�Ǹ� �ٸ��ɷ� ����Ÿ�� ���Ѵ�.
		{
			sort(tasks, tasks + many_task);  //---------------------------------------------���� �ð����⵵ �ּ�ȭ�� �ٽ�
			if (tasks[0].worked == 1)
			{
				cout << "cpu" << this->number + 1 << " �޽� ����" << endl;
				return;
			}
			if ((tasks[0].work_on != 0) && (tasks[0].work_on != this->number+1))
			{
				cout << "cpu" << this->number + 1 << " �޽� ����" << endl;
				return;
			}
			this->whatwork = tasks[0].number;
			tasks[0].work_on = this->number+1;

			tasks[0].work_time++;
			cout << "cpu" << this->number + 1 << "���� task" << this->whatwork + 1 << "�����" << endl;
			
		}
	}
};
cpu *cpus = new cpu[5];

void check() {
	for (int i = 0; i < many_task; i++)
	{
		if ((tasks[i].start<=time) && ((time-tasks[i].start) % tasks[i].period == 0))  //---------------------------�� task�� ���ο� job�� �ο��Ǹ�
		{
			tasks[i].ddl = tasks[i].d;	//--------------------ddl���� ���� period������ �ʱ�ȭ�Ѵ�.
			tasks[i].work_time = 0;  //-------------------------work_time ���� 0���� �ʱ�ȭ�Ѵ�.
			tasks[i].zl = 0;
			tasks[i].worked = 0;
		}

		if (tasks[i].work_time >= tasks[i].execution)  //-------------------------------���� ���ѽð��� ����ð��� �����Ѵٸ�
		{
			tasks[i].worked = 1;  //---------------------------------�� task�� ddl���� ū ������ �����Ѵ�. ddl ���� ũ�ٴ� ���� �������� ���� �Ϸ�� �ǹ̷� �޾Ƶ�����.
		}
		if (tasks[i].ddl == (tasks[i].execution - tasks[i].work_time))
		{
			tasks[i].zl = 1;
		}
	}
	for (int i = 0; i < many_cpu; i++)
	{
		cpus[i].todo = cpus[i].speed;
	}
}

void check_error() {
	for (int i = 0; i < many_task; i++)
	{
		tasks[i].work_on = 0;
		tasks[i].ddl--;  //-------------------------------------------------------------------------------�� Ÿ���� ���������� ddl�� �ϳ��� �پ��� �����̴�.
		if ((tasks[i].start <= time) && ((time-tasks[i].start) % tasks[i].period== tasks[i].d) && (tasks[i].work_time < tasks[i].execution))  //--------------------���ο� job�� �ο��Ǵ� ���������� ���ѽð��� ����ð����� ���ٸ�
		{
			cout << "task" << i + 1 << "����" << "e" << "�߻�$$$$$$$" << endl;  //-----------------------------������ �߻��ߴٰ� �߰��Ѵ�.
		}
	}
}

void set() {
	cout << "------------------------------------------------" << endl;
	cout << "     ���Ͻô� task�� �� ������ �Է����ּ���." << endl;
	cout << "------------------------------------------------" << endl;
	cin >> many_task;
	for (int i = 0; i < many_task; i++)
	{
		cout << endl << "task" << i + 1 << "�� �ֱ�ð��� �Է����ּ���.";
		cin >> tasks[i].period;
		cout << "task" << i + 1 << "�� ���������� �Է����ּ���.";
		cin >> tasks[i].d;
		cout << "task" << i + 1 << "�� ����ð��� �Է����ּ���.";
		cin >> tasks[i].execution;
		cout << "task" << i + 1 << " ���� �����ұ��? (�⺻��=0)";
		cin >> tasks[i].start;
	
		if (tasks[i].execution > tasks[i].d || tasks[i].d > tasks[i].period)
		{
			cout << "�߸��� ���� �Էµ� ������ �����˴ϴ�. �ý����� ���������� �۵����� ���� �� �ֽ��ϴ�." << endl;
		}

		tasks[i].number = i;
	}
	cout << "------------------------------------------------" << endl;
	cout << "     ���Ͻô� cpu�� �� ������ �Է����ּ���." << endl;
	cout << "------------------------------------------------" << endl;
	cin >> many_cpu;
	for (int i = 0; i < many_cpu; i++)
	{
		cout << endl << "cpu" << i + 1 << "�� �ӵ��� �Է����ּ���.(�⺻�� = 1)";
		cin >> cpus[i].speed;
		cpus[i].todo = cpus[i].speed;
		cpus[i].number = i;
	}
	
}


int main() {
	cout << "------------------------------------------------" << endl;
	cout << "       �ȳ��ϼ���. ���� �����층 �����Դϴ�." << endl;
	cout << "------------------------------------------------" << endl;
	cout << "------------------------------------------------" << endl;
	cout << " EDF-ZL �ý��� ���, hetero ��Ƽ�ھ� ȯ���Դϴ�." << endl;
	cout << "------------------------------------------------" << endl;
	set();
	while (true)
	{
		check();
		for (int i = 0; i < many_cpu; i++)
		{
			cpus[i].work();
		}
		time++;
		check_error();
		cout << "<-------------------------------" << time << "�� ��" << endl;
		if (time == 100) { break; }
	}
	//delete[] tasks;
	return 0;
}

















/*
#include<iostream>
#include<algorithm>
using namespace std;

int many_task;
int many_cpu;
int time = 0;
int MAX = 200000000; // 2��


class task {
public:
	int number;
	int period;
	int execution;
	int ddl;
	int work_time;
	int zl = 0;
	int work_on = 0;  // �ٸ� cpu���� ���� �ϰ� �ִ��� Ȯ���ϱ� ���� �����ϻ��̴�.
	int worked = 0; //���� �س����°�?
	int d;


	bool operator <(task &t) {
		if (this->worked == t.worked)
		{
			if (this->work_on == t.work_on)
			{
				if (this->zl == t.zl)
				{
					if (this->ddl == t.ddl)
					{
						return this->number < t.number;
					}
					else
					{
						return this->ddl < t.ddl;
					}
				}
				else
				{
					return this->zl > t.zl;
				}
			}
			else
			{
				return this->work_on < t.work_on;
			}
		}
		else
		{
			return this->worked < t.worked;
		}
	}
};
task *tasks = new task[10];

class cpu {
public:
	int number;
	int whatwork;

	void work() {
		if (tasks[0].worked == 1 || tasks[0].work_on == 1)
		{
			cout << "cpu" << this->number + 1 << " �޽� ����" << endl;
			return;
		}

		this->whatwork = tasks[0].number;
		tasks[0].work_on = 1;
		tasks[0].work_time++;

		cout << "cpu" << this->number + 1 << "���� task" << this->whatwork + 1 << "�����" << endl;
	}
};
cpu *cpus = new cpu[5];










void check() {
	for (int i = 0; i < many_task; i++)
	{
		if (time % tasks[i].period == 0)  //---------------------------�� task�� ���ο� job�� �ο��Ǹ�
		{
			tasks[i].ddl = tasks[i].period;	//--------------------ddl���� ���� period������ �ʱ�ȭ�Ѵ�.
			tasks[i].work_time = 0;  //-------------------------work_time ���� 0���� �ʱ�ȭ�Ѵ�.
			tasks[i].zl = 0;
			tasks[i].worked = 0;
		}

		if (tasks[i].work_time >= tasks[i].execution)  //-------------------------------���� ���ѽð��� ����ð��� �����Ѵٸ�
		{
			tasks[i].worked = 1;  //---------------------------------�� task�� ddl���� ū ������ �����Ѵ�. ddl ���� ũ�ٴ� ���� �������� ���� �Ϸ�� �ǹ̷� �޾Ƶ�����.
		}
		if (tasks[i].ddl == (tasks[i].execution - tasks[i].work_time))
		{
			tasks[i].zl = 1;
		}

	}
}

void check_error() {
	for (int i = 0; i < many_task; i++)
	{
		tasks[i].work_on = 0;
		tasks[i].ddl--;  //-------------------------------------------------------------------------------�� Ÿ���� ���������� ddl�� �ϳ��� �پ��� �����̴�.
		if ((time % tasks[i].period == 0) && (tasks[i].work_time < tasks[i].execution))  //--------------------���ο� job�� �ο��Ǵ� ���������� ���ѽð��� ����ð����� ���ٸ�
		{
			cout << "task" << i + 1 << "����" << "e" << "�߻�$$$$$$$" << endl;  //-----------------------------������ �߻��ߴٰ� �߰��Ѵ�.
		}
	}
}

void set() {
	cout << "--------------------------------------" << endl;
	cout << "���Ͻô� task�� �� ������ �Է����ּ���." << endl;
	cout << "--------------------------------------" << endl;
	cin >> many_task;
	for (int i = 0; i < many_task; i++)
	{
		cout << endl << "task" << i + 1 << "�� �ֱ�ð��� �Է����ּ���.";
		cin >> tasks[i].period;
		cout << "task" << i + 1 << "�� ����ð��� �Է����ּ���.";
		cin >> tasks[i].execution;
		cout << "task" << i + 1 << "�� ���������� �Է����ּ���.";
		cin >> tasks[i].d;
		tasks[i].number = i;
	}
	cout << "--------------------------------------" << endl;
	cout << "���Ͻô� cpu�� �� ������ �Է����ּ���." << endl;
	cout << "--------------------------------------" << endl;
	cin >> many_cpu;
	for (int i = 0; i < many_cpu; i++)
	{
		cpus[i].number = i;
	}
}


int main() {
	set();
	while (true)
	{
		check();
		for (int i = 0; i < many_cpu; i++)
		{
			sort(tasks, tasks + many_task);
			cpus[i].work();
		}
		time++;
		check_error();
		cout << "<-------------------------------" << time << "�� ��" << endl;
		if (time == 100) { break; }
	}

	return 0;
}
*/