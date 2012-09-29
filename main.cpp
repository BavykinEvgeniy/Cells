#include <vector>
#include <set>
#include <iostream>
#include <time.h>
using namespace std;

struct cell
{
	int x;
	int y;
};

enum direction {UP, DOWN, LEFT, RIGHT};

class State{
private:
	int** cells;
	cell freeCell;

	void alloc();
	void copy(const State& rhs);
	void free();
	
	bool move(int x, int y);
	bool move(direction dir);
public:
	State();
	State(const State& rhs);
	State& operator=(const State& rhs);
	~State();

	bool operator==(const State& rhs) const;
	bool operator<(const State& rhs) const;
	bool isCorrect() const;
	bool isAnswer() const;
	void Print() const;

	State* movePart(direction move);
	void shuffle(int countOfmoving);
	//friend bool operator<(const State& lhs, const State& rhs);
};

set<State> history;

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
	for (int i = 0; i < 3; i++)
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

State& State::operator=(const State& rhs)
{
	if(this == &rhs)
		return *this;

	copy(rhs);
	return *this;
}

State::~State()
{
	free();
}

bool State::move(int x, int y)
{
	if(freeCell.x + x > 2 || freeCell.x + x < 0 || freeCell.y + y > 2 || freeCell.y + y < 0){
		return false;
	}

	int tmp = cells[freeCell.x][freeCell.y];
	cells[freeCell.x][freeCell.y] = cells[freeCell.x + x][freeCell.y + y];
	cells[freeCell.x + x][freeCell.y + y] = tmp;

	freeCell.x = freeCell.x + x;
	freeCell.y = freeCell.y + y;
	return true;
}

bool State::move(direction dir) 
{
	switch(dir){
		case(UP)://двигаем снизу	
			return move(1, 0);
			break;
		case(DOWN)://двигаем сверху	
			return move(-1, 0);
			break;
		case(LEFT)://двигаем справа	
			return move(0, 1);
			break;
		case(RIGHT)://двигаем слева	
			return move(0, -1);
			break;
		default:
			return false;
	}
}

int countOfSteps = 0;

State* State::movePart(direction dir)//УЖАСНЫЙ МЕТОД ФУФУФУ
{
	countOfSteps++;
	State* res = new State(*this); //*res = *this;
	if(res->move(dir) && res->isCorrect())
		return res;
	return NULL;
}

void State::shuffle(int countOfmoving = 10)
{
	for (int i = 0; i < countOfmoving; i++){
		move((direction)(rand() % 4));		
	}
}

bool State::operator==(const State& rhs) const 
{
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			if (cells[i][j] != rhs.cells[i][j])
				return false;
	return true;
}

bool State::operator<(const State& rhs) const
{
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			if (cells[i][j] != rhs.cells[i][j])
				return cells[i][j] < rhs.cells[i][j];
	return false;
}

void State::Print() const
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

bool State::isAnswer() const
{
	State etalon;
	return *this == etalon;
}

bool State::isCorrect() const
{
	/*for (unsigned int i = 0; i < history.size(); i++)
		if (history[i] == *this)
			return false;
	return true;*/ 
	return history.find(*this) == history.end();
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
	vector<Link> stack;
	stack.push_back(tmp);
	//history.push_back(*start);
	history.insert(*start);
	unsigned int head = 0;
	/****/

	while (!tmp.s.isAnswer())
	{
		Link toStack;
		State** nextStates = new State*[4];
		for (int i = 0; i < 4; i++)
			nextStates[i] = tmp.s.movePart((direction)i);
		for (int i = 0; i < 4; i++)
			if (nextStates[i] != NULL)
			{
				//nextStates[i]->Print();
				toStack.prev = head;
				toStack.s = *nextStates[i];
				stack.push_back(toStack);
				//history.push_back(toStack.s);
				history.insert(toStack.s);
			}
		head++;
		if (head < stack.size())
			tmp = stack[head];
		for (int i = 0; i < 4; i++)
			delete nextStates[i];
	}
	vector <State> path;

	int count = head;
	while (count >= 0)
	{
		path.push_back(stack[count].s);
		count = stack[count].prev;
	}

	for (int i = path.size()-1; i >= 0; i--)
		path[i].Print();
	cout<<"Length: "<<path.size()-1<<endl;
		//stack[i].s.Print();
		
	/****/
}


int main()
{
	srand((unsigned int)time(NULL));
	State s1, s2, s3, start;

	/*s1.shuffle(400);
	s2.shuffle(400);
	s3.shuffle(400);*/
	start.shuffle(100000);
	cout<<"Start:"<<endl;
	start.Print();
	/*s1.Print();
	s2.Print();
	s3.Print();

	if(s1 < s2)
		std::cout << "s1 < s2" << std::endl;

	if(s2 < s3)
		std::cout << "s2 < s3" << std::endl;
	
	if(s1 < s3)
		std::cout << "s1 < s3" << std::endl;*/

	Solve(&start);
	std::cout << countOfSteps << std::endl;
	return 0;
}