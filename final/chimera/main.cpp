#include <iostream>

// You will need to create some classes.
class Feline
{
    public:
        virtual void vocalize() const = 0;
};

class Canine
{
    public:
        virtual void vocalize() const = 0;
};

class Dog : public Canine
{
    public:
        virtual void vocalize() const;
};

class Cat : public Feline
{
    public:
        virtual void vocalize() const;
};

// Must not change this class.
class Chimera : public Dog, public Cat
{
};

class Lion : public Feline
{
	public:
    	void vocalize() const;
};

// Must not modify this function.
void Dog::vocalize() const
{
    std::cout << "Woof!" << std::endl;
}

// Must not modify this function.
void Cat::vocalize() const
{
    std::cout << "Meow!" << std::endl;
}

void Lion::vocalize() const
{
    std::cout << "Roar!" << std::endl;
}

int main()
{
    auto cp = new Chimera;
    // cp->vocalize(); // Should give compile-time, ambiguity error.
    // Executes Dog::vocalize().
    static_cast<Canine *>(cp)->vocalize();
    // Executes Cat::vocalize().
    static_cast<Feline *>(cp)->vocalize();

    auto lp = new Lion;
    // Executes Lion::vocalize().
    static_cast<Feline *>(lp)->vocalize();
}