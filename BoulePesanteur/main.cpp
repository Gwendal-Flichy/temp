#include <SFML/Graphics.hpp>

struct Vec2
{
    Vec2(float x_, float y_);

    float x;
    float y;

    Vec2& operator+=(const Vec2& op2);
};

Vec2 operator+(const Vec2& op1, const Vec2& op2);
Vec2 operator-(const Vec2& op1, const Vec2& op2);
Vec2 operator/(const Vec2& op1, const float& op2);
Vec2 operator*(const Vec2& op1, const float& op2);

Vec2::Vec2(float x_, float y_) : x(x_), y(y_)
{
}

Vec2 operator+(const Vec2& op1, const Vec2& op2)
{
    return { op1.x + op2.x, op1.y + op2.y };
}

Vec2 operator-(const Vec2& op1, const Vec2& op2)
{
    return { op1.x - op2.x, op1.y - op2.y };
}

Vec2 operator/(const Vec2& op1, const float& op2)
{
    return { op1.x / op2, op1.y / op2 };
}

Vec2 operator*(const Vec2& op1, const float& op2)
{
    return { op1.x * op2, op1.y * op2 };
}

Vec2& Vec2::operator+=(const Vec2& op2)
{
    x += op2.x;
    y += op2.y;
    return *this;
}







struct Boule
{
    Vec2 position;
    Vec2 vitesse;
};

constexpr float g() { return -9.81f; }

void updateBallWithNumericalIntegration(Boule& boule, float totalTime, float deltaTime)
{
    boule.position += boule.vitesse * deltaTime;
    Vec2 acceleration{ 0.f, -g() };
    boule.vitesse += acceleration * deltaTime;
}

void updateBallWithAnalyticalSolution(Boule& boule, float totalTime, float deltaTime)
{
    boule.position.y = -g() * totalTime * totalTime * 0.5f;
    boule.vitesse.x = 0;
    boule.vitesse.y = -g() * totalTime;
}

int main()
{
    const int fps = 240;

    sf::RenderWindow window(sf::VideoMode(800, 800), "SFML works!");
    window.setFramerateLimit(fps);

    Boule boule1{ {100, 0}, {0, 0} };
    Boule boule2{ {300, 0}, {0, 0} };

    float totalTime = 0.f;
    const float deltaTime = 1.f / static_cast<float>(fps);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        updateBallWithNumericalIntegration(boule1, totalTime, deltaTime);
        updateBallWithAnalyticalSolution(boule2, totalTime, deltaTime);

        sf::CircleShape shape1(50.f);
        shape1.setPosition(boule1.position.x, boule1.position.y);
        shape1.setFillColor(sf::Color::Green);
        window.draw(shape1);

        sf::CircleShape shape2(50.f);
        shape2.setPosition(boule2.position.x, boule2.position.y);
        shape2.setFillColor(sf::Color::Blue);
        window.draw(shape2);

        window.display();

        totalTime += deltaTime;
    }

    return 0;
}