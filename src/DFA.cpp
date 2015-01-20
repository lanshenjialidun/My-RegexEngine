#include "DFA.h"
#include <string>
#include <stack>
#include <iostream>
using namespace std;
template <class T>
class LinkStack;

//������ʽջ�����
template <class T>
class StackNode
{
	friend class LinkStack<T>;
private:
	T data;
	StackNode<T> *next;
	StackNode(T item = 0, StackNode<T> *p = NULL)
	{
		date = item;
		next = p;
	}
};

// ������ʽջ��
template <class T>
class LinkStack
{
private:
	StackNode<T> *top;
public:
	LinkStack();
	~LinkStack();
	bool IsEmpty();
	int Length();
	void Push(const T &item);
	T Pop();
	T GetTop();
	void Clear();
};
//���幹�캯��
template <class T>
LinkStack<T>::LinkStack()
{
	top = NULL;
}

//������������
template <class T>
LinkStack<T>::~LinkStack()
{
	Clear();
}
//�ж�ջ�Ƿ�Ϊ��
template <class T>
bool LinkStack<T>::IsEmpty()
{
	return(!top);
}
//ջ�ĳ���
template <class T>
int LinkStack<T>::Length()
{
	StackNode<T> *temp = new StackNode<T>();
	temp = top;
	int length = 0;
	while (temp)
	{
		temp = temp->next;
		length++;
	}
	return length;
}
//ѹջ
template <class T>
void LinkStack<T>::Push(const T &item)
{
	top = new StackNode<T>(item, top);
}
// �������(��ջ)
template <class T>
T LinkStack<T>::Pop()
{
	if (!IsEmpty())
	{
		StackNode<T> *temp = top;
		top = top->next;
		T value = temp->data;
		delete temp;
		return value;
	}
	else
	{
		cout << "Stack is Empty" << endl;
		exit(1);
	}
}
//��ȡջ������
template<class T>
T LinkStack<T>::GetTop()
{
	if (!IsEmpty())
		return top->data;
	else
	{
		cout << "The Stack is Empty" << endl;
		exit(1);
	}
}
//���ջ
template<class T>
void LinkStack<T>::Clear()
{
	StackNode<T> *temp = new StackNode<T>();
	while (top)
	{
		temp = top;
		top = top->next; 
		delete temp;
	}
}
/*
		DFA
*/
//DFA���캯��
DFA::DFA()
{
	Regex = new char[128];
	RegexPost = new char[128]; 
	Edge = new char[128];
	EdgeNumber = 0;
	DFAStatesNumber = 0;
	NFAStatesNumber = 0;
	DtranNumber = 0;
	NFATable = new AdjacentTable();
}
//DFA����������
DFA::~DFA()
{
	delete[] Regex;
	delete[] RegexPost;
	delete[] Edge;
	delete[] AcceptStates;
	NFATable->Clear();
	DFATable->Clear();
}
//��ȡ��������
void DFA::InputRegex()
{
	cout << " �������������ʽ" << endl;
	cin >> Regex;
}
//���������
void DFA::InsertNode()
{
	int i = 0,j, len = strlen(Regex);
	int length = len;
	while (len--&&i<=length-1)
	{
		if (((Regex[i] != '(' && Regex[i] != '.' && Regex[i] != '|')
			|| Regex[i] == ')'
			|| Regex[i] == '*')
			&& (Regex[i + 1] != ')' && Regex[i + 1] != '.' && Regex[i + 1] != '|' && Regex[i + 1] != '*'))
		{
			for (j = len; j>i + 1; j--)
			{
				Regex[j] = Regex[j - 1];
			}
			Regex[i + 1] = '.';
			len++;
			Regex[len] = '\0';
			i++;
		}
		i++;
	}
	cout << "\n��һ��: ���������\n"
		<< Regex << "\n"
		<< "�ַ�������: " << len
		<< "\n\n------------------------" << endl;
	cout << "\n��һ�������������\n" << Regex << endl;
	cout << "�ַ������ȣ�" <<length<<endl;
}
//�ж���������ȼ�
int DFA::Precedence(char symbol)
{
	int priority;
	switch (symbol)
	{
	case '|':priority = 1; break;
	case '.':priority = 2; break;
	case '*':priority = 3; break;
	default:priority = 0; break;
	}
	return priority;
}
//������ʽתΪ�沨��ʽ
void DFA::RegextoPost()
{
	int i = 0, j = 0;
	char ch, cl;
	strcpy(RegexPost, "\0");
	LinkStack<char> *ls = new LinkStack<char>();
	ls->Clear();
	ls->Push('#');
	ch = Regex[i];
	while (ch != '\0')
	{
		if (ch == '(')
		{
			ls->Push(ch);
			ch = Regex[++i];
		}
		else if (ch = ')')
		{
			while (ls->GetTop() != '(')
				RegexPost[j++] = ls->Pop();
			ls->Pop();
			ch = Regex[++i];
		}
		else if ((ch == '|') || (ch = '*') || (ch == '.'))
		{
			cl = ls->GetTop();
			while (Precedence(cl) >= Precedence(ch))
			{
				RegexPost[j++] = cl;
				ls->Pop();
				cl = ls->GetTop();
			}
			ls->Push(ch);
			ch = Regex[++i];
		}
		else{
			RegexPost[j++] = ch;
			ch = Regex[++i];
		}
	}
	ch = ls->Pop();
	while ((ch == '|') || (ch == '*') || (ch == '.'))
	{
		RegexPost[j++] = ch;
		ch = ls->Pop();
	}
	RegexPost[j] = '\0';
	ls->Clear();
	cout << "תΪ��׺ʽ" << endl;
	cout << RegexPost << endl;
	cout << "�ַ������ȣ�" << strlen(RegexPost) << endl;
}
//ɨ���沨��ʽ�г������������ַ�����Ŀ
void DFA::GetEdgeNumber()
{
	int i = 0, j;
	EdgeNumber = 0;
	while (RegexPost[i] != '\0')
	{
		if (RegexPost[i] == '.' || RegexPost[i] == '|' || RegexPost[i] == '*')
		{
			i++;
			continue;
		}
		for (j = 0; j < EdgeNumber; j++)
		{
			if (RegexPost[i] == Edge[j])
				break;
		}
		if (j == EdgeNumber)
		{
			Edge[EdgeNumber] = RegexPost[i];
			EdgeNumber++;
		}
		i++;
	}
	Edge[EdgeNumber] = '\0';
	cout << "\n������: ��ȡ�ַ���\n";
	for (i = 0; i<EdgeNumber; i++)
	{
		cout << Edge[i] << ' ';
	}
	cout << "\n�ַ�����: " << EdgeNumber
		<< "\n\n------------------------" << endl;
}
//��Thompson���취����NFA
void DFA::Thompson()
{ 
	int i, j;
	char ch;
	int s1, s2;
	LinkStack<int >*States = new LinkStack<int >();
	States->Clear();
	if (strlen(RegexPost) < 1)
	{
		cout << "No Regex Expression Find" << endl;
		exit(1);
	}
	NFATable->SetValue(0, 0);
	i = 1; j = 0;
	ch = RegexPost[i];
	while (ch != '\0')
	{
		if (ch = '.')
		{
			s2 = States->Pop();
		}
	}
	
}