/**
 * @class Observer Pattern
 * @brief Behavioral design pattern that allows an object (the Subject) to notify other objects (Observers) about changes in its state.
 *
 * The Observer pattern defines a one-to-many dependency between objects, such that when the state of one object (the Subject) changes,
 * all of its dependent objects (Observers) are automatically notified and updated. This pattern is particularly useful when an object
 * needs to notify other objects about changes without knowing who or what those objects are.
 *
 * In this implementation:
 * - The `Subject` class maintains a list of observers and notifies them when the state changes.
 * - The `Observer` class defines an abstract interface for all concrete observers.
 * - Concrete observers, such as `HealthUI`, `ScoreUI`, and `EventLogger`, implement the `onNotify` method to respond to state changes.
 * 
 * @usage
 * The Observer pattern is typically used in situations where multiple components or modules need to react to state changes
 * in a central object (like user interfaces, event handling systems, and logging systems).
 * It decouples the subject from the observers, making it easier to add or remove observers without modifying the subject.
 * 
 * @example
 * The `Subject` class maintains the current state, and the `Observer` classes react accordingly. When a state change occurs in the subject,
 * all observers are notified and updated with the new state.
 * 
 * - `addObserver(Observer* observer)` adds an observer to the list.
 * - `removeObserver(Observer* observer)` removes an observer from the list.
 * - `notifyObservers()` notifies all observers about the state change.
 * 
 * The observer pattern is often used in event-driven systems, GUI applications, or any system where changes need to be propagated
 * to multiple components automatically.
 * 
 * @see http://designpatterns.sourceforge.net/patterns/Observer.html
 */

#include <SDL2/SDL.h>
#include <iostream>

/**
 * @class Observer
 * @brief Abstract base class for all observers in the observer pattern.
 *
 * Defines the interface for objects that need to be notified when the state of the subject changes.
 */
class Observer 
{
    friend class Subject;

public:
    /**
     * @brief Constructor initializes the next pointer to nullptr.
     */
    Observer() : next_(nullptr) {}

    /**
     * @brief Destructor.
     */
    virtual ~Observer() = default;

    /**
     * @brief Virtual function to handle state notifications.
     * @param value The updated state value.
     */
    virtual void onNotify(int value) = 0;

private:
    /**< Pointer to the next observer in the chain */
    Observer* next_;
};

/**
 * @class Subject
 * @brief Represents the subject in the observer pattern.
 *
 * Manages a list of observers and notifies them of state changes.
 */
class Subject 
{
private:
    Observer* head_; /**< Head of the linked list of observers */
    int state_;      /**< Current state value of the subject */

public:
    /**
     * @brief Constructor initializes the subject with no observers and a state of 0.
     */
    Subject() : head_(nullptr), state_(0) {}

    /**
     * @brief Adds a new observer to the subject.
     * @param observer Pointer to the observer to be added.
     */
    void addObserver(Observer* observer) 
    {
        observer->next_ = head_;
        head_ = observer;
    }

    /**
     * @brief Removes an observer from the subject.
     * @param observer Pointer to the observer to be removed.
     */
    void removeObserver(Observer* observer)
    {
        if (head_ == observer)
        {
            head_ = observer->next_;
            observer->next_ = nullptr;
            // code to fix a memory leak, 
            // the implementation above assumes 
            // that we have a garbage collector !!!
            // delete observer->next_
            return;
        }

        Observer* current = head_;

        while (current != nullptr) 
        {
            if (current->next_ == observer)
            {
                current->next_ = observer->next_;
                observer->next_ = nullptr;
                
                // code to fix a memory leak, 
                // the implementation above assumes 
                // that we have a garbage collector !!!
                // delete observer->next_
            
                return;
            }
            current = current->next_;
        }
    }

    /**
     * @brief Sets the state of the subject and notifies all observers.
     * @param newState The new state value.
     */
    void setState(int newState) 
    {
        state_ = newState;
        notifyObservers();
    }

    /**
     * @brief Retrieves the current state of the subject.
     * @return The current state value.
     */
    int getState() const { return state_; }

    /**
     * @brief Notifies all observers of the current state.
     */
    void notifyObservers() 
    {
        Observer* current = head_;

        while (current) 
        {
            current->onNotify(state_);
            current = current->next_;
        }
    }
};

/**
 * @class HealthUI
 * @brief Concrete observer for updating health-related UI.
 */
class HealthUI : public Observer 
{
public:
    void onNotify(int value) override 
    {
        std::cout << "[Health UI] Player health updated to: " << value << std::endl;
    }
};

/**
 * @class ScoreUI
 * @brief Concrete observer for updating score-related UI.
 */
class ScoreUI : public Observer 
{
public:
    /**
     * @brief Handles notifications with the new state value.
     * @param value The new state value (player health).
     */
    void onNotify(int value) override 
    {
        std::cout << "[Score UI] Player score updated to: " << value * 10 << " points" << std::endl;
    }
};

/**
 * @class EventLogger
 * @brief Concrete observer for logging events.
 */
class EventLogger : public Observer 
{
public:
    /**
     * @brief Handles notifications with the new state value.
     * @param value The new state value (event data).
     */
    void onNotify(int value) override 
    {
        std::cout << "[Logger] Event logged with state: " << value << std::endl;
    }
};

/**
 * @brief Main function to initialize SDL and run the observer pattern example.
 *
 * Sets up the SDL window and handles key events to update the state of the subject.
 * Notifies observers of changes in state.
 *
 * @param argc Command line argument count.
 * @param argv Command line arguments.
 * @return Returns 0 on successful execution.
 */
int main(int argc, char* argv[]) 
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) 
    {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Observer Pattern with Linked List", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
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

    // Create Subject and Observers
    Subject subject;
    HealthUI healthUI;
    ScoreUI scoreUI;
    EventLogger logger;

    subject.addObserver(&healthUI);
    subject.addObserver(&scoreUI);
    subject.addObserver(&logger);

    bool running = true;
    SDL_Event event;

    int counter = 100;

    while (running) 
    {
        while (SDL_PollEvent(&event)) 
        {
            if (event.type == SDL_QUIT) 
            {
                running = false;
            }

            if (event.type == SDL_KEYDOWN) 
            {
                switch (event.key.keysym.sym) 
                {
                    case SDLK_SPACE:
                        subject.setState(++counter);
                        break;
                    case SDLK_r:
                        counter = 0;
                        subject.setState(counter);
                        break;
                    case SDLK_h:
                        subject.setState(--counter);
                        break;
                    default:
                        break;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
