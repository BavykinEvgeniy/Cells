#include <vector>
#include <iostream>
#include <time.h>
using namespace std;

struct cell
{
	int i;
	int j;
};

enum direction {UP, DOWN, LEFT, RIGHT};

class State;

vector <State> history;

class State{
private:
	int** cells;
	cell freeCell;

	void alloc();
	void copy(const State& s);
	void free();
	
	void move(int x, int y);
public:
	State();
	State(const State& rhs);
	State& operator=(const State & rhs);
	~State();

	void Randomize();
	bool const operator==(const State & rhs);
	State* movePart(direction move);
	bool const isCorrect();
	bool const isAnswer();
	void const Print();
};

void State::alloc(){
	cells = new int*[3];
	for (int i=0; i<3; i++)
		cells[i] = new int[3];
}

void State::copy(const State& s){
	for (int i=0; i<3; i++)
		for (int j=0; j<3; j++)
			cells[i][j] = s.cells[i][j];
	freeCell = s.freeCell;
}

void State::free(){
	for (int i=0; i<3; i++)
		delete [] cells[i];
	delete [] cells;
}

State::State()
{
	alloc();
	int count = 1;
	for (int i=0; i<3; i++)
		for (int j=0; j<3; j++)
			cells[i][j] = count++;
	freeCell.i = 2;
	freeCell.j = 2;
	cells[freeCell.i][freeCell.j] = 0;
}

State::State(const State& rhs)
{
	alloc();
	copy(rhs);
}

State::~State()
{
	free();
}

State& State::operator=(const State& rhs)
{
	if(this == &rhs)
		return *this;

	copy(rhs);
	return *this;
}

void State::move(int x, int y)
{
	if( freeCell.i + x > 2 || freeCell.i + x < 0 || freeCell.j + y > 2 || freeCell.j + y < 0){
		throw std::exception("invalid_move");
		return;
	}

	int tmp = cells[freeCell.i][freeCell.j];
	cells[freeCell.i][freeCell.j] = cells[freeCell.i + x][freeCell.j + y];
	cells[freeCell.i + x][freeCell.j + y] = tmp;
	freeCell.i = freeCell.i + x;
	freeCell.j = freeCell.j + y;
}

bool const State::operator==(const State& s) 
{
	for (int i=0; i<3; i++)
		for (int j=0; j<3; j++)
			if (cells[i][j] != s.cells[i][j])
				return false;
	return true;
}

void const State::Print()
{
	for (int i=0; i<3; i++)
	{
		for (int j=0; j<3; j++)
			if (cells[i][j])
				cout << cells[i][j] << " ";
			else
				cout << "  ";
		cout << endl;
	}
	cout << endl;
}

void State::Randomize()
{
	int mixes = 10;
	for (int k = 0; k < mixes; k++)
	{
		int i1 = rand()%3;
		int j1 = rand()%3;
		int i2 = rand()%3;
		int j2 = rand()%3;
		int tmp = cells[i1][j1];
		cells[i1][j1] = cells[i2][j2];
		cells[i2][j2] = tmp;
	}
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			if (cells[i][j] == 0)
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



State* State::movePart(direction d)//движение относительно свободной клетки
{
	State* res = new State;
	*res = *this;

	switch(d){
		case(UP)://двигаем снизу	
			res -> move(1, 0);
			break;
		case(DOWN)://двигаем сверху	
			res -> move(-1, 0);
			break;
		case(LEFT)://двигаем справа	
			res -> move(0, 1);
			break;
		case(RIGHT)://двигаем слева	
			res -> move(1, 0);
			break;
	}
	return res;
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
			nextStates[i] = tmp.s.movePart( (direction)i);
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
	cout<<"Hello my VERY BIG world"<<endl;
	start.Print();
	//Solve(&start);
	return 0;
}