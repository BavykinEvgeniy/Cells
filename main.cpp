#include <vector>
#include <iostream>
#include <time.h>
using namespace std;

struct cell
{
	int x;
	int y;
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
	void move(direction dir);
public:
	State();
	State(const State& rhs);
	State& operator=(const State & rhs);
	~State();

	bool const operator==(const State & rhs);	
	bool const isCorrect();
	bool const isAnswer();
	void const Print();

	State* movePart(direction move);
	void shuffle(int countOfmoving);
};

void State::alloc(){
	cells = new int*[3];
	for (int i = 0; i < 3; i++)
		cells[i] = new int[3];
}

void State::copy(const State& s){
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
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
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			cells[i][j] = count++;

	freeCell.x = 2;
	freeCell.y = 2;
	cells[freeCell.x][freeCell.y] = 0;
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
	if(freeCell.x + x > 2 || freeCell.x + x < 0 || freeCell.y + y > 2 || freeCell.y + y < 0){
		throw std::exception("invalid_move");
		return;
	}

	int tmp = cells[freeCell.x][freeCell.y];
	cells[freeCell.x][freeCell.y] = cells[freeCell.x + x][freeCell.y + y];
	cells[freeCell.x + x][freeCell.y + y] = tmp;

	freeCell.x = freeCell.x + x;
	freeCell.y = freeCell.y + y;
}

void State::move(direction dir)
{
	switch(dir){
		case(UP)://двигаем снизу	
			move(1, 0);
			break;
		case(DOWN)://двигаем сверху	
			move(-1, 0);
			break;
		case(LEFT)://двигаем справа	
			move(0, 1);
			break;
		case(RIGHT)://двигаем слева	
			move(0, -1);
			break;
	}
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
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
			if (cells[i][j])
				cout << cells[i][j] << " ";
			else
				cout << "  ";
		cout << endl;
	}
	cout << endl;
}

void State::shuffle(int countOfmoving = 10)
{
	for (int i = 0; i < countOfmoving; i++){
		try{
			move((direction)(rand() % 4));
		}catch(std::exception ex){
			//std::cout << ex.what() << std::endl;
		}
	}
}

bool const State::isAnswer()
{
	State etalon;
	return *this == etalon;
}

bool const State::isCorrect()
{
	for (int i = 0; i < history.size(); i++)
		if (history[i] == *this)
			return false;
	return true; 
}

State* State::movePart(direction dir)//УЖАСНЫЙ МЕТОД ФУФУФУ
{
	State* res = new State(*this); //*res = *this;
	res-> move(dir);
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
		for (int i = 0; i < 4; i++)
			nextStates[i] = tmp.s.movePart( (direction)i);
		for (int i = 0; i < 4; i++)
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
		for (int i = 0; i < 4; i++)
			delete nextStates[i];
	}
	vector <State> path;
	int count = stack.size() -1;
	while (count >= 0)
	{
		path.push_back(stack[count].s);
		count = stack[count].prev;
	}

	for (int i = path.size()-1; i >= 0; i--)
		path[i].Print();

	/*for (int i=0; i<stack.size(); i++)
		stack[i].s.Print();*/
	/****/
}


int main()
{
	srand (time(NULL));
	State start;
	start.shuffle(40);
	cout<<"BIG START:"<<endl;
	start.Print();
	//Solve(&start);
	return 0;
}