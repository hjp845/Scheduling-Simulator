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
	int work_on=0;  // 다른 cpu에서 일을 하고 있는지 확인하기 위한 변수일뿐이다.
	int worked=1; //일을 해내었는가? start 때문에 기본값은 1로 한다.
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
		for (int i = 0; i < this->speed; i++)  //---------------------------------------------------------------------speed만큼 하다가 하나가 완료되면 다른걸로 갈아타서 일한다.
		{
			sort(tasks, tasks + many_task);  //---------------------------------------------정렬 시간복잡도 최소화의 핵심
			if (tasks[0].worked == 1)
			{
				cout << "cpu" << this->number + 1 << " 휴식 상태" << endl;
				return;
			}
			if ((tasks[0].work_on != 0) && (tasks[0].work_on != this->number+1))
			{
				cout << "cpu" << this->number + 1 << " 휴식 상태" << endl;
				return;
			}
			this->whatwork = tasks[0].number;
			tasks[0].work_on = this->number+1;

			tasks[0].work_time++;
			cout << "cpu" << this->number + 1 << "에서 task" << this->whatwork + 1 << "실행됨" << endl;
			
		}
	}
};
cpu *cpus = new cpu[5];

void check() {
	for (int i = 0; i < many_task; i++)
	{
		if ((tasks[i].start<=time) && ((time-tasks[i].start) % tasks[i].period == 0))  //---------------------------각 task에 새로운 job이 부여되면
		{
			tasks[i].ddl = tasks[i].d;	//--------------------ddl변수 값을 period값으로 초기화한다.
			tasks[i].work_time = 0;  //-------------------------work_time 값도 0으로 초기화한다.
			tasks[i].zl = 0;
			tasks[i].worked = 0;
		}

		if (tasks[i].work_time >= tasks[i].execution)  //-------------------------------만약 일한시간이 실행시간을 충족한다면
		{
			tasks[i].worked = 1;  //---------------------------------그 task의 ddl값을 큰 값으로 설정한다. ddl 값이 크다는 것은 구간내에 일이 완료된 의미로 받아들이자.
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
		tasks[i].ddl--;  //-------------------------------------------------------------------------------한 타임이 지날때마다 ddl은 하나씩 줄어들기 마련이다.
		if ((tasks[i].start <= time) && ((time-tasks[i].start) % tasks[i].period== tasks[i].d) && (tasks[i].work_time < tasks[i].execution))  //--------------------새로운 job이 부여되는 시점직전에 일한시간이 실행시간보다 낮다면
		{
			cout << "task" << i + 1 << "에서" << "e" << "발생$$$$$$$" << endl;  //-----------------------------오류가 발생했다고 뜨게한다.
		}
	}
}

void set() {
	cout << "------------------------------------------------" << endl;
	cout << "     원하시는 task의 총 개수를 입력해주세요." << endl;
	cout << "------------------------------------------------" << endl;
	cin >> many_task;
	for (int i = 0; i < many_task; i++)
	{
		cout << endl << "task" << i + 1 << "의 주기시간을 입력해주세요.";
		cin >> tasks[i].period;
		cout << "task" << i + 1 << "의 마감시한을 입력해주세요.";
		cin >> tasks[i].d;
		cout << "task" << i + 1 << "의 실행시간을 입력해주세요.";
		cin >> tasks[i].execution;
		cout << "task" << i + 1 << " 언제 시작할까요? (기본값=0)";
		cin >> tasks[i].start;
	
		if (tasks[i].execution > tasks[i].d || tasks[i].d > tasks[i].period)
		{
			cout << "잘못된 값이 입력된 것으로 추정됩니다. 시스템이 정상적으로 작동하지 않을 수 있습니다." << endl;
		}

		tasks[i].number = i;
	}
	cout << "------------------------------------------------" << endl;
	cout << "     원하시는 cpu의 총 개수를 입력해주세요." << endl;
	cout << "------------------------------------------------" << endl;
	cin >> many_cpu;
	for (int i = 0; i < many_cpu; i++)
	{
		cout << endl << "cpu" << i + 1 << "의 속도를 입력해주세요.(기본값 = 1)";
		cin >> cpus[i].speed;
		cpus[i].todo = cpus[i].speed;
		cpus[i].number = i;
	}
	
}


int main() {
	cout << "------------------------------------------------" << endl;
	cout << "       안녕하세요. 모의 스케쥴링 공간입니다." << endl;
	cout << "------------------------------------------------" << endl;
	cout << "------------------------------------------------" << endl;
	cout << " EDF-ZL 시스템 기반, hetero 멀티코어 환경입니다." << endl;
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
		cout << "<-------------------------------" << time << "초 됨" << endl;
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
int MAX = 200000000; // 2억


class task {
public:
	int number;
	int period;
	int execution;
	int ddl;
	int work_time;
	int zl = 0;
	int work_on = 0;  // 다른 cpu에서 일을 하고 있는지 확인하기 위한 변수일뿐이다.
	int worked = 0; //일을 해내었는가?
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
			cout << "cpu" << this->number + 1 << " 휴식 상태" << endl;
			return;
		}

		this->whatwork = tasks[0].number;
		tasks[0].work_on = 1;
		tasks[0].work_time++;

		cout << "cpu" << this->number + 1 << "에서 task" << this->whatwork + 1 << "실행됨" << endl;
	}
};
cpu *cpus = new cpu[5];










void check() {
	for (int i = 0; i < many_task; i++)
	{
		if (time % tasks[i].period == 0)  //---------------------------각 task에 새로운 job이 부여되면
		{
			tasks[i].ddl = tasks[i].period;	//--------------------ddl변수 값을 period값으로 초기화한다.
			tasks[i].work_time = 0;  //-------------------------work_time 값도 0으로 초기화한다.
			tasks[i].zl = 0;
			tasks[i].worked = 0;
		}

		if (tasks[i].work_time >= tasks[i].execution)  //-------------------------------만약 일한시간이 실행시간을 충족한다면
		{
			tasks[i].worked = 1;  //---------------------------------그 task의 ddl값을 큰 값으로 설정한다. ddl 값이 크다는 것은 구간내에 일이 완료된 의미로 받아들이자.
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
		tasks[i].ddl--;  //-------------------------------------------------------------------------------한 타임이 지날때마다 ddl은 하나씩 줄어들기 마련이다.
		if ((time % tasks[i].period == 0) && (tasks[i].work_time < tasks[i].execution))  //--------------------새로운 job이 부여되는 시점직전에 일한시간이 실행시간보다 낮다면
		{
			cout << "task" << i + 1 << "에서" << "e" << "발생$$$$$$$" << endl;  //-----------------------------오류가 발생했다고 뜨게한다.
		}
	}
}

void set() {
	cout << "--------------------------------------" << endl;
	cout << "원하시는 task의 총 개수를 입력해주세요." << endl;
	cout << "--------------------------------------" << endl;
	cin >> many_task;
	for (int i = 0; i < many_task; i++)
	{
		cout << endl << "task" << i + 1 << "의 주기시간을 입력해주세요.";
		cin >> tasks[i].period;
		cout << "task" << i + 1 << "의 실행시간을 입력해주세요.";
		cin >> tasks[i].execution;
		cout << "task" << i + 1 << "의 마감시한을 입력해주세요.";
		cin >> tasks[i].d;
		tasks[i].number = i;
	}
	cout << "--------------------------------------" << endl;
	cout << "원하시는 cpu의 총 개수를 입력해주세요." << endl;
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
		cout << "<-------------------------------" << time << "초 됨" << endl;
		if (time == 100) { break; }
	}

	return 0;
}
*/