

/**
 * @file CommandPatternExample.cpp
 * 
 * @brief This code demonstrates the **Command Design Pattern**, 
 * which is a behavioral design pattern used to encapsulate requests or operations 
 * as objects, allowing them to be parameterized, stored, or executed at a later time. 
 * 
 * ### Pattern Explanation
 * The Command Pattern decouples the object that invokes the operation (e.g., an InputHandler)
 * from the object that performs the operation (e.g., Player). 
 * Each operation is represented as a command object, providing flexibility 
 * in associating commands with actions dynamically at runtime.
 * 
 * ### Benefits of the Command Pattern
 * - **Encapsulation**: Commands encapsulate method calls and parameters.
 * - **Decoupling**: The sender of a command doesn't need to know about the receiver's details.
 * - **Flexibility**: Commands can be easily parameterized, queued, or logged for undo/redo operations.
 * - **Open/Closed Principle**: Adding new commands does not affect existing code, 
 * as you simply implement new `Command` subclasses.
 * 
 * ### Implementation Overview
 * - **Command Interface**: An abstract base class with an `execute` method 
 *    that all concrete commands must implement.
 * - **Concrete Commands**: Specific commands (e.g., `JumpCommand`, `MoveLeftCommand`) 
 *    that encapsulate the logic for performing specific actions.
 * - **Receiver**: The `Player` class, which performs the actual operations when a command is executed.
 * - **Invoker**: The `InputHandler` class, which binds commands to keys 
 *    and triggers their execution based on user input.
 * 
 * ### SDL Integration
 * In this example, the SDL library is used for handling keyboard input events (`SDL_KEYDOWN`). 
 * The `InputHandler` binds SDL keycodes to specific command objects, 
 * allowing the player to perform actions (e.g., jump, move left) by pressing the corresponding keys.
 * 
 * ### Example Key Bindings:
 * - `SPACE` → Jump
 * - `LEFT` → Move Left
 * - `RIGHT` → Move Right
 * - `UP` → Move Forward
 * - `DOWN` → Move Backward
 * 
 * ### How to Extend
 * To add a new action:
 * 1. Create a new class that inherits from `Command` and implement the `execute` method.
 * 2. Bind the new command to a key in the `InputHandler` class.
 * 3. Optionally, extend the `Player` class to include a new method corresponding to the action.
 * 
 * ### Example Use Cases
 * - Game controls, as shown here.
 * - Command queues in multithreaded applications.
 * - Undo/redo systems for text editors or drawing tools.
 * - Macro recording and playback in applications.
 * 
 * @note This example focuses on keyboard input with SDL, 
 * but the pattern is adaptable to other types of input or actions.
 */



#include "SDL2/SDL.h"
#include <iostream>
#include <unordered_map>
#include <memory>




// Command interface
class Command {
public:
    virtual ~Command() {}
    virtual void execute() = 0;
};

// Receiver class
class Player 
{
public:
    void jump() 
    {
        std::cout << "Player jumps!" << std::endl;
    }
    void moveLeft() 
    {
        std::cout << "Player moves left!" << std::endl;
    }
    void moveRight() 
    {
        std::cout << "Player moves right!" << std::endl;
    }
};

// Concrete Commands
class JumpCommand : public Command 
{
    Player& player;
public:
    explicit JumpCommand(Player& p) : player(p) {}
    void execute() override 
    {
        player.jump();
    }
};

class MoveForwardCommand : public Command {
private:
    Player& player;

public:
    MoveForwardCommand(Player& player) : player(player) {}
    void execute() override {
        std::cout << "Player is moving forward!\n";
    }
};

class MoveBackwardCommand : public Command {
private:
    Player& player;

public:
    MoveBackwardCommand(Player& player) : player(player) {}
    void execute() override {
        std::cout << "Player is moving backward!\n";
    }
};


class MoveLeftCommand : public Command {
    Player& player;
public:
    explicit MoveLeftCommand(Player& p) : player(p) {}
    void execute() override {
        player.moveLeft();
    }
};

class MoveRightCommand : public Command {
    Player& player;
public:
    explicit MoveRightCommand(Player& p) : player(p) {}
    void execute() override {
        player.moveRight();
    }
};

// Invoker class
class InputHandler {
    std::unordered_map<SDL_Keycode, std::unique_ptr<Command>> commandMap;
public:
    void bindCommand(SDL_Keycode key, std::unique_ptr<Command> command) {
        commandMap[key] = std::move(command);
    }
    void handleInput(const SDL_Event& event) {
        if (event.type == SDL_KEYDOWN) {
            SDL_Keycode key = event.key.keysym.sym;
            if (commandMap.find(key) != commandMap.end())
            {
                commandMap[key]->execute();
            }
        }
    }
};

// Main function
int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("Command Pattern Example",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          800, 600, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    Player player;
    InputHandler inputHandler;

    inputHandler.bindCommand(SDLK_SPACE , std::make_unique<JumpCommand>(player));
    inputHandler.bindCommand(SDLK_LEFT, std::make_unique<MoveLeftCommand>(player));
    inputHandler.bindCommand(SDLK_RIGHT, std::make_unique<MoveRightCommand>(player));
    inputHandler.bindCommand(SDLK_UP, std::make_unique<MoveForwardCommand>(player));
    inputHandler.bindCommand(SDLK_DOWN, std::make_unique<MoveBackwardCommand>(player)); 

    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;

                case SDL_KEYDOWN:
                    inputHandler.handleInput(event);
                    break;
            }
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
