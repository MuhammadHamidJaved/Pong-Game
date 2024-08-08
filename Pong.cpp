#include<iostream>
#include<SFML/Graphics.hpp>
#include<pthread.h>
#include<unistd.h>
#include<string>
#include<X11/Xlib.h>

using namespace std;

const int Width = 600;
const int Height = 400;
const float Paddle_Speed = 5.0f;
const float Ball_Speed = -3.0f;
const float scale=1.5f;
int player1score=0;
int player2score=0;


sf::RectangleShape paddle1(sf::Vector2f(20, 100));
sf::RectangleShape paddle2(sf::Vector2f(20, 100));
sf::CircleShape ball(10);

sf::Vector2f ballVelocity(Ball_Speed,Ball_Speed);
pthread_mutex_t paddleMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t ballMutex = PTHREAD_MUTEX_INITIALIZER;

void* MovePaddle1(void* arg)
{
while(true)
{
pthread_mutex_lock(&paddleMutex);
if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && paddle1.getPosition().y > 0)
{
paddle1.move(0,-Paddle_Speed);
}
if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && paddle1.getPosition().y < Height - paddle1.getSize().y)
{
paddle1.move(0,Paddle_Speed);
}
pthread_mutex_unlock(&paddleMutex);
usleep(10000);
}

pthread_exit(NULL);

}

void* MovePaddle2(void* arg)
{
while(true)
{
pthread_mutex_lock(&paddleMutex);
if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && paddle2.getPosition().y > 0)
{
paddle2.move(0,-Paddle_Speed);
}
if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && paddle2.getPosition().y < Height - paddle2.getSize().y)
{
paddle2.move(0,Paddle_Speed);
}
pthread_mutex_unlock(&paddleMutex);
usleep(10000);
}

pthread_exit(NULL);

}

void* MoveBall(void* arg)
{
while(true)
{
pthread_mutex_lock(&ballMutex);
ball.move(ballVelocity);
pthread_mutex_unlock(&ballMutex);

//getGlobalBounds and intersect are functions of sfml library
if (ball.getGlobalBounds().intersects(paddle1.getGlobalBounds()) || ball.getGlobalBounds().intersects(paddle2.getGlobalBounds())) 
{
ballVelocity.x = -ballVelocity.x;
}

if(ball.getPosition().y<0||ball.getPosition().y > Height-ball.getRadius()*2)
ballVelocity.y = -ballVelocity.y;

if(ball.getPosition().x<0)
{
ballVelocity.x = -ballVelocity.x;
player2score++;
}

if(ball.getPosition().x > Width - ball.getRadius() * 2)
{
ballVelocity.x = -ballVelocity.x;
player1score++;
}
sf::sleep(sf::milliseconds(10));
}
pthread_exit(NULL);
}

int main()
{
XInitThreads();

sf::RenderWindow window(sf::VideoMode(Width, Height), "Pong");
window.setFramerateLimit(144);

paddle1.setPosition(10,Height/2-paddle1.getSize().y/2);
paddle2.setPosition(Width-30,Height/2-paddle2.getSize().y/2);
ball.setPosition(Width / 2 - ball.getRadius(), Height / 2 - ball.getRadius());

sf::RectangleShape background(sf::Vector2f(Width, Height));
background.setFillColor(sf::Color::Blue);

sf::RectangleShape line(sf::Vector2f(8, Height));
line.setFillColor(sf::Color::Red);
line.setPosition(300,0);

sf::Font font;
if (!font.loadFromFile("ArialTh.ttf")) {
std::cerr << "Error loading font file!" << std::endl;
return 1;
}

sf::Text text;
text.setFont(font);
text.setCharacterSize(60);
text.setFillColor(sf::Color::White);
text.setPosition(240,20);
text.setScale(sf::Vector2f(scale, scale));

sf::Text text1;
text1.setFont(font);
text1.setCharacterSize(60);
text1.setFillColor(sf::Color::White);
text1.setPosition(320,20);
text1.setScale(sf::Vector2f(scale, scale));

sf::Text text2;
text2.setFont(font);
text2.setCharacterSize(30);
text2.setFillColor(sf::Color::White);
text2.setPosition(50,200);
text2.setScale(sf::Vector2f(scale, scale));
text2.setString("Congratulations Player 1!!");

sf::Text text3;
text3.setFont(font);
text3.setCharacterSize(30);
text3.setFillColor(sf::Color::White);
text3.setPosition(50,200);
text3.setScale(sf::Vector2f(scale, scale));
text3.setString("Congratulations Player 2!!");

pthread_t id[3];
pthread_create(&id[0],NULL,&MovePaddle1,NULL);
pthread_create(&id[1],NULL,&MovePaddle2,NULL);
pthread_create(&id[2],NULL,&MoveBall,NULL);

while(window.isOpen())
{
sf::Event event;
while(window.pollEvent(event))
{
if (event.type == sf::Event::Closed)
window.close();
}
window.clear();

window.draw(background);
window.draw(line);
window.draw(paddle1);
window.draw(paddle2);
window.draw(ball);

text.setString(std::to_string(player1score));
window.draw(text);
text1.setString(std::to_string(player2score));
window.draw(text1);

if(player1score==10||player2score==10)
{
window.clear();
if(player1score==10)
{
window.draw(text2);
window.display();
sf::sleep(sf::seconds(5));
window.close();
}

if(player2score==10)
{
window.draw(text3);
window.display();
sf::sleep(sf::seconds(5));
window.close();

}
}

window.display();

}



pthread_join(id[0],NULL);
pthread_join(id[1],NULL);
pthread_join(id[2],NULL);


return 0;
}






