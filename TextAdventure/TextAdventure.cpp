// TextAdventure.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <algorithm> // count_if
#include <ctime> // time
#include <iostream>
#include <string>
#include <random> // mt19937
#include <vector>

namespace Random
{
    // Initialize our mersenne twister with a random seed based on the clock (once at system startup)
    std::mt19937 mersenne{ static_cast<std::mt19937::result_type>(std::time(nullptr)) };
}

class RandomName
{
private:
    static const inline std::vector<std::string> firstPartNames{ "Bun", "Paper", "Joke", "Disappointment" };
    static const inline std::vector<std::string> lastPartNames{ "Stuffer", "Pusher", "Master", "Lover"};

public:

    static std::string getRandomName()
    {
        std::uniform_int_distribution dist{ 0, 3 };

        int firstNameIndex{ dist(Random::mersenne) };
        int lastNameIndex{ dist(Random::mersenne) };

        return firstPartNames[firstNameIndex] + lastPartNames[lastNameIndex];
    }
};

enum class Race
{
    Human, // 0
    Fox, // 1
    Golem, // ...
    Elf,

    race_count // 4
};

enum class Type
{
    Gladiator,
    Bowman,
    Caster,
    Assassin,

    type_count
};

class Entity
{

private:
    int m_health{};
    int m_maxHealth{};
    int m_resource{};
    int m_maxResource{};
    bool m_isDead{ false };
    std::string m_name{};
    Race m_race{};
    Type m_type{};

public:
    // Constructors

    Entity() {}
    Entity(std::string name, Race race, Type type) : m_name{ name }, m_race{ race }, m_type{ type }
    {
        switch (race)
        {
            case Race::Fox:
                m_maxHealth = 60;
                break;
            case Race::Golem:
                m_maxHealth = 120;
                break;
            case Race::Elf:
                m_maxHealth = 80;
                break;
            case Race::Human:
            default:
                m_maxHealth = 100;
        }

        switch (type)
        {
        case Type::Gladiator:
        case Type::Assassin:
            m_maxResource = 0; // Rage and Adrenaline increases as combat continues
            break;
        case Type::Bowman:
        case Type::Caster:
        default:
            m_maxResource = 100; // Stamina and Willpower decreases as combat continues
            
        }

        m_health = m_maxHealth;
        m_resource = m_maxResource;
    }

    // Getters
    int getHealth() const { return m_health; }
    int getResource() const { return m_resource; }
    bool getIsDead() const { return m_isDead; }
    std::string getName() const { return m_name; }
    Race getRace() const { return m_race; }
    Type getType() const { return m_type; }

    // Setters
    void HealEntity(int value)
    {
        m_health += value;

        if (m_health > m_maxHealth) m_health = m_maxHealth;
    }

    void DamageEntity(int value)
    {
        m_health -= value;

        if (m_health < 0)
        {
            m_health = 0;
            m_isDead = true;
        }
    }

    void UseResource(int value)
    {
        m_resource -= value;

        if (m_resource < 0) m_resource = 0;
    }

    void ReplenishResource(int value)
    {
        m_resource += value;

        if (m_resource > m_maxResource) m_resource = m_maxResource;
    }

    void setRace(Race race) { m_race = race; }
    void setType(Type type) { m_type = type; }
};

class Player : public Entity
{
private:
public:
    // Constructors
    Player() {};
    Player(std::string name, Race race, Type type) : Entity(name, race, type)
    {}
};

void greeting()
{
    std::cout << "Greetings Adventurer and welcome to 'Text Escape'!\n";
    std::cout << "I hope you enjoy my first end to end game!\n";
    std::cout << "If you have any feedback, please visit https://michael-geffre.com/text-escape-feedback\n";
    std::cout << "and share your thoughts with me!\n\n";
}

std::string askForName()
{
    std::string input{ };
    int nameSetAttempt{ 0 };
    int nonAlphaChars{ 0 };

    while (true)
    {
        std::cout << "Please enter a name with only alphabetical characters (no spaces either): ";
        std::getline(std::cin, input);

        nonAlphaChars = std::count_if(input.begin(), input.end(), [](char c) -> int { return !std::isalpha(c); });

        if (nonAlphaChars > 0)
        {
            switch (nameSetAttempt)
            {
            case 0:
                std::cout << "Please try again, you entered some non-alphabetical characters (includes spaces).\n";
                break;
            case 1:
                std::cout << "Haha, very funny. I said not to enter any NON-alphabetical characters...Try again.\n";
                break;
            case 2:
                std::cout << "This is your last chance to choose a name you enjoy. If you mess up again, I will choose one for you.\n";
                break;
            case 3:
                std::string name{ RandomName::getRandomName() };
                std::cout << "You asked for it! Your name is now " << name << ". Enjoy your randomly generated name.\n";
                return name;
            }

            ++nameSetAttempt;

            std::cout << '\n';
            std::cin.clear();
        }
        else
        {
            return input;
        }
    }
}

std::string_view printRace(Race race)
{
    switch (race)
    {
    case Race::Fox: return "Fox";
    case Race::Golem: return "Golem";
    case Race::Elf: return "Elf";
    case Race::Human: return "Human";
    default: return "???";
    }
}

std::string_view printType(Type type)
{
    switch (type)
    {
    case Type::Gladiator: return "Gladiator";
    case Type::Assassin: return "Assassin";
    case Type::Bowman: return "Bowman";
    case Type::Caster: return "Caster";
    default: return "???";
    }
}

Race askForRace()
{
    std::cout << "Enter in the number of your race from the below options:\n";

    for (int i{ 0 }; i < static_cast<int>(Race::race_count); ++i)
    {
        std::cout << i + 1 << ". " << printRace(static_cast<Race>(i)) << '\n';
    }

    int selection{};
    while (true)
    {
        
        std::cin >> selection;

        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(32767, '\n');
        }
        else
        {
            if (selection > 0 && selection < static_cast<int>(Race::race_count) + 1)
            {
                break;
            }
            else
            {
                std::cout << "You enter in an invalid number, please try again.\n";
            }
        }
    }

    return static_cast<Race>(selection - 1);
}

Type askForType()
{
    std::cout << "Choose your type from the below options:\n";

    for (int i{ 0 }; i < static_cast<int>(Type::type_count); ++i)
    {
        std::cout << i + 1 << ". " << printType(static_cast<Type>(i)) << '\n';
    }

    int selection{};
    while (true)
    {

        std::cin >> selection;

        if (std::cin.fail())
        {
            std::cout << "Invalid input, please try again.\n";
            std::cin.clear();
            std::cin.ignore(32767, '\n');
        }
        else
        {
            std::cout << "Selection: " << selection << '\n';
            if (selection > 0 && selection < static_cast<int>(Type::type_count) + 1)
            {
                break;
            }
            else
            {
                std::cout << "You enter in an invalid number, please try again.\n";
            }
        }
    }

    return static_cast<Type>(selection - 1);
}

Player getUserDetails()
{
    while (true)
    {
        std::string name{ askForName() };
        Race race{ askForRace() };
        Type type{ askForType() };

        while (true)
        {
            std::cout << "Is the below information correct? (Y/N)\n";
            std::cout << "Name: " << name << '\n';
            std::cout << "Race: " << printRace(race) << '\n';
            std::cout << "Type: " << printType(type) << '\n';

            char c{};
            std::cin >> c;

            if (std::cin.fail())
            {
                std::cout << "Please enter in a 'Y' or 'N'.\n";
                std::cin.clear();
                std::cin.ignore(32767, '\n');
            }
            else
            {
                if (c == 'Y')
                {
                    return Player{name, race, type};
                }
                else
                {
                    std::cin.clear();
                    std::cin.ignore(32767, '\n');
                    break; // Ask player to enter in information again
                }
            }
        }
    }
    
    return {};
}

void startStory()
{
    std::cout << "Print out the objective of the game and then describe the environment they are in.\n";
}
int main()
{

    // Player Object
    greeting();
    Player player{ getUserDetails() };
    startStory();

}