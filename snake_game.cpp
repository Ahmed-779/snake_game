#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <vector>

using namespace std;

const int windowWidth = 350;
const int windowHeight = 350;
const int blockSize = 10;

// Snake direction enumeration
enum Direction { Up, Down, Left, Right };

struct SnakeSegment {
    int x, y;
    SnakeSegment(int x, int y) : x(x), y(y) {}
};

vector<SnakeSegment> snake = {SnakeSegment(30, 170)};
Direction direction = Right;
bool playing = true;

int fruitX, fruitY;
bool eaten = true; // If the fruit is eaten
int score = 0;

sf::Font font;
sf::Text scoreText;

void spawnFruit() {
    fruitX = (rand() % (windowWidth / blockSize)) * blockSize;
    fruitY = (rand() % (windowHeight / blockSize)) * blockSize;
    eaten = false;
}

void update(sf::RenderWindow &window) {
    // Move the snake
    int newX = snake[0].x, newY = snake[0].y;

    switch (direction) {
    case Up:
        newY -= blockSize;
        break;
    case Down:
        newY += blockSize;
        break;
    case Left:
        newX -= blockSize;
        break;
    case Right:
        newX += blockSize;
        break;
    }

    // Check for collisions with walls
    if (newX < 0 || newY < 0 || newX >= windowWidth || newY >= windowHeight) {
        playing = false;
        return;
    }

    // Check for collisions with itself
    for (const auto &segment : snake) {
        if (segment.x == newX && segment.y == newY) {
            playing = false;
            return;
        }
    }

    // Add new head to the snake
    snake.insert(snake.begin(), SnakeSegment(newX, newY));

    // Check if fruit is eaten
    if (newX == fruitX && newY == fruitY) {
        eaten = true;
        score++;
    } else {
        snake.pop_back(); // Remove the tail if no fruit eaten
    }

    // Spawn a new fruit if eaten
    if (eaten) {
        spawnFruit();
    }

    // Update score text
    scoreText.setString("Score: " + to_string(score));
}

void draw(sf::RenderWindow &window) {
    window.clear(sf::Color::Black);

    // Draw fruit
    sf::RectangleShape fruit(sf::Vector2f(blockSize, blockSize));
    fruit.setFillColor(sf::Color::Red);
    fruit.setPosition(fruitX, fruitY);
    window.draw(fruit);

    // Draw snake
    for (const auto &segment : snake) {
        sf::RectangleShape block(sf::Vector2f(blockSize, blockSize));
        block.setFillColor(sf::Color::Green);
        block.setPosition(segment.x, segment.y);
        window.draw(block);
    }

    // Draw score
    window.draw(scoreText);

    window.display();
}

int main() {
    // Initialize random seed
    srand(time(0));

    // Create window
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Snake Game");

    // Load font
    if (!font.loadFromFile("OpenSans-VariableFont_wdth,wght.ttf")) {
        printf("Failed to load font\n");
        return 1;
    }

    // Set up score text
    scoreText.setFont(font);
    scoreText.setCharacterSize(20);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10, 10);
    scoreText.setString("Score: 0");

    spawnFruit();

    sf::Clock clock;
    while (window.isOpen() && playing) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Handle input
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && direction != Down) {
            direction = Up;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && direction != Up) {
            direction = Down;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && direction != Right) {
            direction = Left;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && direction != Left) {
            direction = Right;
        }

        // Update game logic every 0.15 seconds
        if (clock.getElapsedTime().asSeconds() > 0.15f) {
            update(window);
            clock.restart();
        }

        draw(window);
    }

    // Game over screen
    sf::Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(25); // Reduce font size for better fit
    gameOverText.setFillColor(sf::Color::White);
    gameOverText.setString("Game Over!\nFinal Score: " + to_string(score));
    gameOverText.setPosition(windowWidth / 2 - 100, windowHeight / 2 - 50); // Center the text

    window.clear(sf::Color::Black);
    window.draw(gameOverText);
    window.display();

    sf::sleep(sf::seconds(3)); // Display game over screen for 3 seconds
    return 0;
}
