/// Code written by Bordeanu Calin

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <windows.h>

#define BLOCKS 90
#define blockSize 7

const int WIDTH = BLOCKS * blockSize;
const int HEIGHT = BLOCKS * blockSize;

sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Snake");

bool gameOver = false;

int redScore = 0;
int blueScore = 0;

class Player
{
public:
    std::vector<sf::RectangleShape> body;

    sf::Color defaultColor;
    sf::Vector2f dir = {0, -1};

    void Move()
    {
        sf::RectangleShape &head = body[0];

        int x = head.getPosition().x/blockSize;
        int y = head.getPosition().y/blockSize;

        if(dir.x==0 && dir.y==-1){
            head.setPosition({x*blockSize, (y-1)*blockSize});
        }
        if(dir.x==0 && dir.y==1){
            head.setPosition({x*blockSize, (y+1)*blockSize});
        }
        if(dir.x==1 && dir.y==0){
            head.setPosition({(x+1)*blockSize, y*blockSize});
        }
        if(dir.x==-1 && dir.y==0){
            head.setPosition({(x-1)*blockSize, y*blockSize});
        }
    }

    void AddTail()
    {
        sf::RectangleShape newPart;
        newPart.setFillColor(defaultColor);
        newPart.setSize({blockSize, blockSize});

        sf::RectangleShape head = body[0];

        int x = head.getPosition().x;
        int y = head.getPosition().y;

        newPart.setPosition({x, y});

        body.push_back(newPart);
    }

    void CheckBounds()
    {
        sf::RectangleShape &head = body[0];

        int x = head.getPosition().x/blockSize;
        int y = head.getPosition().y/blockSize;

        if(x>=BLOCKS) x = 0;
        if(x<0) x = BLOCKS;
        if(y>=BLOCKS) y = 0;
        if(y<0) y = BLOCKS;

        x*=blockSize;
        y*=blockSize;

        head.setPosition({x, y});
    }

    void CheckSelfCollision(int &enemyScore)
    {
        for(int i = 1; i<body.size(); ++i){
            int x1 = body[0].getPosition().x;
            int x2 = body[i].getPosition().x;

            int y1 = body[0].getPosition().y;
            int y2 = body[i].getPosition().y;

            if(x1==x2 && y1==y2){
                gameOver = true;
                ++enemyScore;
                std::cout << "Press 'R' to go to the next round";
            }
        }
    }

    void CheckPlayerCollision(Player p, int &enemyScore)
    {
        for(int i = 0; i<p.body.size(); ++i){
            int x1 = body[0].getPosition().x;
            int x2 = p.body[i].getPosition().x;

            int y1 = body[0].getPosition().y;
            int y2 = p.body[i].getPosition().y;

            if(x1==x2 && y1==y2){
                gameOver = true;
                ++enemyScore;
                std::cout << "Press 'R' to go to the next round";
            }
        }
    }

    void ChangeDir(bool useWASD)
    {
        if(!useWASD)
        {
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && dir.y!=1)     dir = {0, -1};
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && dir.y!=-1)  dir = {0, 1};
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && dir.x!=1)   dir = {-1, 0};
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && dir.x!=-1) dir = {1, 0};
        }
        else
        {
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::W) && dir.y!=1)  dir = {0, -1};
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::S) && dir.y!=-1) dir = {0, 1};
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::A) && dir.x!=1)  dir = {-1, 0};
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::D) && dir.x!=-1) dir = {1, 0};
        }
    }

    void Draw()
    {
        for(int i = 0; i<body.size(); ++i)
            window.draw(body[i]);
    }

    void Update(Player enemy, int &enemyScore)
    {
        Move();
        CheckSelfCollision(enemyScore);
        CheckPlayerCollision(enemy, enemyScore);
        AddTail();
        CheckBounds();
    }

    Player(sf::Color Color, sf::Vector2f Position)
    {
        sf::RectangleShape head;

        defaultColor = Color;
        head.setFillColor(Color);
        head.setSize({blockSize, blockSize});
        head.setPosition(Position);
        body.push_back(head);
    }
};

int main()
{
    gameOver = false;
    system("cls");

    Player p1(sf::Color::Red, {WIDTH/4, HEIGHT/2});
    Player p2(sf::Color::Blue, {WIDTH-WIDTH/4, HEIGHT/2});

    sf::Clock clock;
    float t = 0;

    std::cout << "Red:  " << redScore  << '\n';
    std::cout << "Blue: " << blueScore << '\n';

    while(window.isOpen())
    {
        sf::Event e;
        while(window.pollEvent(e)){
            if(e.type == sf::Event::Closed) window.close();
            if(gameOver && sf::Keyboard::isKeyPressed(sf::Keyboard::R)) main();
        }

        sf::Time time = clock.restart();
        t+=time.asSeconds();

        if(!gameOver)
        {
            p1.ChangeDir(true);
            p2.ChangeDir(false);
            if(t>0.03){
                t = 0;
                p1.Update(p2, blueScore);
                p2.Update(p1, redScore);
            }
        }

        window.clear(sf::Color(0, 0, 0));
        p1.Draw();
        p2.Draw();
        window.display();
    }
}
