// example.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>

class MyClass
{
public:
	MyClass();
	~MyClass();

private:

};

class MyClass2 : MyClass
{
public:

private:

};

class MyClass3 {
public:
	MyClass3();
	~MyClass3();

private:

};

MyClass3::MyClass3()
{
}

MyClass3::~MyClass3()
{
}
MyClass2: MyClass::MyClass2 : MyClass()
{
}

MyClass2: MyClass::~MyClass2 : MyClass()
{
}

int main()
{
	std::cout << "Hello World!\n";
}
