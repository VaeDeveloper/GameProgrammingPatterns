/**
 * pattern Prototype
 * @brief The Prototype Pattern is a creational design pattern that allows you to create new objects by cloning an existing object, called a prototype.
 * 
 * This pattern is particularly useful when object creation is complex, costly, or when you need multiple objects with similar state. Instead of creating new instances from scratch, the prototype pattern allows you to create new objects by copying an existing one. This can save time and resources, particularly when creating many instances that only differ slightly.
 * 
 * The Prototype pattern is typically implemented by having an interface with a method, such as `clone()`, which is used to create copies of the object. The objects being cloned can be of any type that implements this interface.
 * 
 * @note It is important to note that the cloned object might need to have certain properties adjusted (e.g., different coordinates, different configurations) when it is being created, as shown in the example where the `Bullet` object’s position is customized on cloning.
 * 
 * @example
 * In the context of a game, suppose you have a `Bullet` object that has a starting position and speed. Instead of creating a new `Bullet` from scratch every time, you can clone an existing `Bullet`, adjust its position or other properties, and use the new object without needing to re-initialize its properties.
 *
 * @see https://refactoring.guru/design-patterns/prototype
 * @see https://www.dofactory.com/design-patterns/prototype
 */

#include "SDL2/SDL.h"
#include <iostream>

/**
 * @class GameObject
 * @brief Abstract base class for all game objects in the prototype pattern.
 * 
 * Defines the common interface for all game objects that can be cloned and rendered.
 */
class GameObject
{
public:
    /**
     * @brief Clones the current object to create a new instance.
     * 
     * @param newX The new x-coordinate for the cloned object.
     * @param newY The new y-coordinate for the cloned object.
     * @return A pointer to the cloned GameObject.
     */
    virtual GameObject* clone(int newX, int newY) const = 0;

    /**
     * @brief Renders the object on the screen.
     * 
     * @param renderer The SDL renderer to draw the object.
     */
    virtual void render(SDL_Renderer* renderer) const = 0;

    /**
     * @brief Virtual destructor for proper cleanup of derived classes.
     */
    virtual ~GameObject() = default;
};

/**
 * @class Bullet
 * @brief Represents a Bullet in the game, inherits from GameObject.
 * 
 * This class provides functionality to clone the bullet, render it on the screen, 
 * and update its position based on speed.
 */
class Bullet : public GameObject
{
private:
    int x , y; ///< The x and y coordinates of the Bullet.
    int speed; ///< The speed at which the Bullet moves vertically.

public:
    /**
     * @brief Constructs a Bullet object with a starting position and speed.
     * 
     * @param startX The initial x-coordinate of the Bullet (default is 0).
     * @param startY The initial y-coordinate of the Bullet (default is 0).
     * @param speed The speed of the Bullet (default is 0).
     */
    Bullet(int startX = 0, int startY = 0, int speed = 0)
    : x(startX), y(startY), speed(speed) 
    {
        std::cout << "Bullet constructor called" << std::endl;
    }

    /**
     * @brief Clones the Bullet object and returns a new instance with updated position.
     * 
     * @param newX The new x-coordinate for the cloned Bullet.
     * @param newY The new y-coordinate for the cloned Bullet.
     * @return A pointer to the new Bullet object.
     */
    GameObject* clone(int newX, int newY) const override
    {
        return new Bullet(newX, newY, speed);
    }

    /**
     * @brief Renders the Bullet on the screen using SDL.
     * 
     * @param renderer The SDL renderer used to draw the Bullet.
     */
    void render(SDL_Renderer* renderer) const override
    {
        SDL_Rect rect = {x, y, 10, 20};
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); 
        SDL_RenderFillRect(renderer, &rect);
    }

    /**
     * @brief Updates the position of the Bullet by moving it vertically.
     */
    void update() 
    {
        y -= speed; 
    }
};


/**
 * @brief Main function that initializes SDL, 
 * creates a Bullet object, and runs the game loop.
 */
int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Prototype Pattern with SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
    if (!window) 
    {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) 
    {
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    Bullet originalBullet(320, 480, 5); 
    Bullet* clonedBullet = static_cast<Bullet*>(originalBullet.clone(100, 480));

    bool running = true;
    SDL_Event event;

    while (running) 
    {
        while (SDL_PollEvent(&event)) 
        {
            if (event.type == SDL_QUIT) 
            {
                running = false;
            }
        }

        originalBullet.update();
        clonedBullet->update();

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Черный фон
        SDL_RenderClear(renderer);

        originalBullet.render(renderer);
        clonedBullet->render(renderer);

        SDL_RenderPresent(renderer);

        SDL_Delay(16); 
    }
    
    delete clonedBullet;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

