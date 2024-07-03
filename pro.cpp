#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;

const int WORLD_WIDTH = 10;
const int WORLD_HEIGHT = 10;

class GameObject {
protected:
    int positionX, positionY;

public:
    GameObject(int x, int y) : positionX(x), positionY(y) {}

    virtual void move(int dx, int dy) {
        positionX += dx;
        positionY += dy;
    }

    virtual void display() const = 0;

    int getPositionX() const {
        return positionX;
    }

    int getPositionY() const {
        return positionY;
    }

    
    bool isValidPosition(int x, int y) const {
        if (x >= 1 && x < WORLD_WIDTH - 1 && y >= 1 && y < WORLD_HEIGHT - 1) {
            return true;
        }
        return false;
    }
};

class PlayerObject : public GameObject {
public:
    PlayerObject(int x, int y) : GameObject(x, y) {}

    void display() const override {
        cout << "P ";
    }

    void move(int dx, int dy) override {
        int newPosX = positionX + dx;
        int newPosY = positionY + dy;
        if (isValidPosition(newPosX, newPosY)) {
            positionX = newPosX;
            positionY = newPosY;
        }
    }
};

class EnemyObject : public GameObject {
public:
    EnemyObject(int x, int y) : GameObject(x, y) {}

    void display() const override {
        cout << "E ";
    }

    void move(int dx, int dy) override {
        int newPosX = positionX + dx;
        int newPosY = positionY + dy;
        if (isValidPosition(newPosX, newPosY)) {
            positionX = newPosX;
            positionY = newPosY;
        }
    }
};

class ObstacleObject : public GameObject {
public:
    ObstacleObject(int x, int y) : GameObject(x, y) {}

    void display() const override {
        cout << "# ";
    }

    // Obstacles are immobile, so they don't move
    void move(int dx, int dy) override {}
};


bool isObstaclePosition(int x, int y, GameObject* obstacles[], int obstacleCount) {
    for (int i = 0; i < obstacleCount; ++i) {
        if (x == obstacles[i]->getPositionX() && y == obstacles[i]->getPositionY()) {
            return true;
        }
    }
    return false;
}

int main() {
    srand(time(nullptr));

    
    GameObject *obstacles[3];
    obstacles[0] = new ObstacleObject(2, 3);
    obstacles[1] = new ObstacleObject(5, 5);
    obstacles[2] = new ObstacleObject(7, 8);

    int playerX, playerY;
    do {
        playerX = rand() % (WORLD_WIDTH - 2) + 1;
        playerY = rand() % (WORLD_HEIGHT - 2) + 1;
    } while (isObstaclePosition(playerX, playerY, obstacles, 3));

    GameObject *player = new PlayerObject(playerX, playerY);
    GameObject *enemy = new EnemyObject(rand() % (WORLD_WIDTH - 2) + 1, rand() % (WORLD_HEIGHT - 2) + 1);

    cout<<"GAME RULES:\n1)p-be the player(you)\n2)E-be the Enemy (automatically it move)\n3)The Task is player need to catch the enemy\n\n";


    int userInput;
    bool gameEnd = false;
    do {
        // Clear screen
        system("cls || clear");

        for (int y = 0; y < WORLD_HEIGHT; ++y) {
            for (int x = 0; x < WORLD_WIDTH; ++x) {
                
                bool isObstacle = false;
                for (int i = 0; i < 3; ++i) {
                    if (x == obstacles[i]->getPositionX() && y == obstacles[i]->getPositionY()) {
                        obstacles[i]->display();
                        isObstacle = true;
                        break;
                    }
                }
                if (isObstacle) {
                    continue; 
                }

                if (x == 0 || x == WORLD_WIDTH - 1 || y == 0 || y == WORLD_HEIGHT - 1) {
                    cout << "* ";
                } else if (x == player->getPositionX() && y == player->getPositionY()) {
                    player->display();
                } else if (x == enemy->getPositionX() && y == enemy->getPositionY()) {
                    enemy->display();
                } else {
                    cout << ". ";
                }
            }
            cout << endl;
        }

        cout << "\nEnter movement direction (1: left, 2: up, 3: down, 4: right): ";
        cin >> userInput;
        int dx = 0, dy = 0;
        switch (userInput) {
            case 1: // left
                dx = -1;
                break;
            case 2: // up
                dy = -1;
                break;
            case 3: // down
                dy = 1;
                break;
            case 4: // right
                dx = 1;
                break;
            default:
                cout << "Invalid input!" << endl;
                continue; 
        }
        player->move(dx, dy);

       
        if (isObstaclePosition(player->getPositionX(), player->getPositionY(), obstacles, 3)) {
            cout << "Cannot move! Obstacle in the way." << endl;
            player->move(-dx, -dy); 
        }

        if (player->getPositionX() == enemy->getPositionX() && player->getPositionY() == enemy->getPositionY()) {
            cout << "Game Over! You got caught by the enemy." << endl;
            gameEnd = true;
        }

        enemy->move(rand() % 3 - 1, rand() % 3 - 1);

    } while (!gameEnd);

    delete player;
    delete enemy;

    
    for (int i = 0; i < 3; ++i) {
        delete obstacles[i];
    }

    return 0;
}
