#include <vector>
#include <iostream>
#include <time.h>
using namespace std;

struct cell
{
	int i;
	int j;
};

class State;

vector <State> history;

class State{
private:
	int** state;
	cell freeCell;
public:
	State();
	State(const State& s);
	void Randomize();
	State& operator=(const State & s);
	bool operator==(const State & s);
	State* movePart(int option);
	bool const isCorrect();
	bool const isAnswer();
	void const Print();
	~State();
};

State::State()
{
	state = new int*[3];
	for (int i=0; i<3; i++)
		state[i] = new int[3];
	int count = 1;
	for (int i=0; i<3; i++)
		for (int j=0; j<3; j++)
			state[i][j] = count++;
	freeCell.i = 2;
	freeCell.j = 2;
	state[freeCell.i][freeCell.j] = 0;
}

State::State(const State& s)
{
	state = new int*[3];
	for (int i=0; i<3; i++)
		state[i] = new int[3];
	for (int i=0; i<3; i++)
		for (int j=0; j<3; j++)
			state[i][j] = s.state[i][j];
	freeCell.i = s.freeCell.i;
	freeCell.j = s.freeCell.j;
}

State& State::operator =(const State& s)
{
	if(this == &s)
		return *this;

	for (int i=0; i<3; i++)
		for (int j=0; j<3; j++)
			state[i][j] = s.state[i][j];
	freeCell.i = s.freeCell.i;
	freeCell.j = s.freeCell.j;
	return *this;
}

bool State::operator ==(const State& s) 
{
	for (int i=0; i<3; i++)
		for (int j=0; j<3; j++)
			if (state[i][j] != s.state[i][j])
				return false;
	return true;
}

void const State::Print()
{
	for (int i=0; i<3; i++)
	{
		for (int j=0; j<3; j++)
			if (state[i][j])
				cout<<state[i][j]<<" ";
			else
				cout<<"  ";
		cout<<endl;
	}
	cout<<endl;
}

void State::Randomize()
{
	int mixes = 10;
	for (int k=0; k<mixes; k++)
	{
		int i1 = rand()%3;
		int j1 = rand()%3;
		int i2 = rand()%3;
		int j2 = rand()%3;
		int tmp = state[i1][j1];
		state[i1][j1] = state[i2][j2];
		state[i2][j2] = tmp;
	}
	for (int i=0; i<3; i++)
		for (int j=0; j<3; j++)
			if (state[i][j] == 0)
			{
				freeCell.i = i;
				freeCell.j = j;
			}
}

bool const State::isAnswer()
{
	State etalon;
	return *this == etalon;
}

bool const State::isCorrect()
{
	for (int i=0; i<history.size(); i++)
		if (history[i] == *this)
			return false;
	return true; 
}

State* State::movePart(int option)//�������� ������������ ��������� ������
{
	State* res = new State;;
	*res = *this;
	if (option == 0)//������� �����
	{
		if (res->freeCell.i < 2)
		{
			int tmp = res->state[freeCell.i][freeCell.j];
			res->state[freeCell.i][freeCell.j] = res->state[freeCell.i+1][freeCell.j];
			res->state[freeCell.i+1][freeCell.j] = tmp;
			res->freeCell.i = freeCell.i+1;
			res->freeCell.j = freeCell.j;
		}
		else
			return NULL;
	}
	if (option == 1)//������� ������
	{
		if (res->freeCell.i>0)
		{
			int tmp = res->state[freeCell.i][freeCell.j];
			res->state[freeCell.i][freeCell.j] = res->state[freeCell.i-1][freeCell.j];
			res->state[freeCell.i-1][freeCell.j] = tmp;
			res->freeCell.i = freeCell.i-1;
			res->freeCell.j = freeCell.j;
		}
		else
			return NULL;
	}
	if (option == 2)//������� ������
	{
		if (res->freeCell.j<2)
		{
			int tmp = res->state[freeCell.i][freeCell.j];
			res->state[freeCell.i][freeCell.j] = res->state[freeCell.i][freeCell.j+1];
			res->state[freeCell.i][freeCell.j+1] = tmp;
			res->freeCell.i = freeCell.i;
			res->freeCell.j = freeCell.j+1;
		}
		else
			return NULL;
	}
	if (option == 3)//������� �����
	{
		if (res->freeCell.j>0)
		{
			int tmp = res->state[freeCell.i][freeCell.j];
			res->state[freeCell.i][freeCell.j] = res->state[freeCell.i][freeCell.j-1];
			res->state[freeCell.i][freeCell.j-1] = tmp;
			res->freeCell.i = freeCell.i;
			res->freeCell.j = freeCell.j-1;
		}
		else
			return NULL;
	}
	if (!res->isCorrect())
		return NULL;

	return res;
}

State::~State()
{
	for (int i=0; i<3; i++)
		delete [] state[i];
	delete [] state;
}

struct Link{
	State s;
	int prev;
};

void Solve(State* start)
{
	Link tmp;
	tmp.s = *start;
	tmp.prev = -1;
	vector <Link> stack;
	stack.push_back(tmp);
	history.push_back(*start);
	int head = 0;
	/****/

	while (!tmp.s.isAnswer())
	{
		Link toStack;
		State** nextStates = new State*[4];
		for (int i=0; i<4; i++)
			nextStates[i] = tmp.s.movePart(i);
		for (int i=0; i<4; i++)
			if (nextStates[i] != NULL)
			{
				//nextStates[i]->Print();
				toStack.prev = head;
				toStack.s = *nextStates[i];
				stack.push_back(toStack);
				history.push_back(toStack.s);
			}
		head++;
		if (head<stack.size())
			tmp = stack[head];
		for (int i=0; i<4; i++)
			delete nextStates[i];
	}
	vector <State> path;
	int count = stack.size() -1;
	while (count>=0)
	{
		path.push_back(stack[count].s);
		count = stack[count].prev;
	}

	for (int i=path.size()-1; i>=0; i--)
		path[i].Print();

	/*for (int i=0; i<stack.size(); i++)
		stack[i].s.Print();*/
	/****/
}


int main()
{
	srand ( time(NULL) );
	State start;
	start.Randomize();
	cout<<"START:"<<endl;
	start.Print();
	//Solve(&start);
	return 0;
}