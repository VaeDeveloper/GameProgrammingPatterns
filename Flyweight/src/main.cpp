
/**
 * @file flyweight_example.cpp
 * 
 * @brief Demonstration of the Flyweight Design Pattern using SDL.
 * 
 * This code implements the Flyweight Design Pattern to efficiently manage and reuse shared texture resources.
 * 
 * @details
 * The Flyweight pattern is a structural design pattern that is used to minimize memory usage or computational
 * cost by sharing as much data as possible with other similar objects. This is achieved by separating intrinsic
 * (shared) state from extrinsic (context-specific) state. In this example:
 * 
 * - **Intrinsic state**: The loaded texture data, shared across all instances that use the same texture.
 * - **Extrinsic state**: The rendering position and other properties specific to each rendering instance.
 * 
 * ### Key Components:
 * - **Flyweight Interface**: 
 *   - Defined by the `Flyweight` class.
 *   - Declares an interface (`Draw`) for performing operations using the shared state.
 * - **Concrete Flyweight**: 
 *   - Implemented by the `TextureFlyweight` class.
 *   - Handles the loading and rendering of textures using SDL.
 * - **Flyweight Factory**: 
 *   - Implemented by the `FlyweightFactory` class.
 *   - Ensures shared flyweight objects are created only once and reused.
 * - **Client**:
 *   - The `main` function demonstrates the use of the factory to retrieve shared flyweight objects
 *     and render them efficiently.
 * 
 * ### Benefits:
 * 1. Reduces memory usage by sharing resources (e.g., textures).
 * 2. Improves performance when rendering multiple objects with the same intrinsic state.
 * 
 * ### Example Usage:
 * In this code:
 * - `crateTexture` and `metalTexture` are flyweights shared across multiple rendering calls.
 * - The factory ensures only one instance of each texture is created, even if used in multiple places.
 * - Extrinsic properties like rendering positions (`x` and `y`) are provided during each rendering call.
 * 
 * ### SDL Integration:
 * - Uses `SDL_Renderer` for rendering and `SDL_Texture` for managing image data.
 * - SDL's `IMG_Load` is used to load textures from image files.
 * - Demonstrates resource management and rendering in an SDL application.
 * 
 * ### Example Output:
 * - The program displays multiple crates and metal textures on the screen.
 * - Outputs logs indicating whether a texture was created or reused.
 */


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <map>
#include <memory>
#include <string>

/**
 * @brief Abstract base class representing the Flyweight interface.
 * 
 * Defines the common interface for all flyweight objects. 
 * Flyweights provide shared resources and operations that depend on unique parameters.
 */
class Flyweight 
{
public:
    /**
     * @brief Virtual destructor for Flyweight.
     */
    virtual void Draw(SDL_Renderer* renderer, int x, int y) const = 0;
    
    /**
     * @brief Renders the flyweight on the screen at the specified position.
     * 
     * @param renderer The SDL_Renderer used for rendering.
     * @param x The x-coordinate for the rendering position.
     * @param y The y-coordinate for the rendering position.
     */    
    virtual ~Flyweight() = default;
};

/**
 * @brief Concrete implementation of the Flyweight interface for textures.
 * 
 * This class represents a texture that is shared between multiple instances.
 * It handles the loading and rendering of an SDL_Texture.
 */
class TextureFlyweight : public Flyweight {
private:
    /**
     * 
     */
    SDL_Texture* texture;

    int width, height; 

public:
    /**
     * @brief Constructs a TextureFlyweight and loads the texture from a file.
     * 
     * @param renderer The SDL_Renderer used to create the texture.
     * @param filePath The file path of the texture to load.
     * @throws std::runtime_error If the texture cannot be loaded.
     */
    TextureFlyweight(SDL_Renderer* renderer, const std::string& filePath) 
    {
        SDL_Surface* surface = IMG_Load(filePath.c_str());
        if (!surface) {
            throw std::runtime_error("Failed to load image: " + filePath);
        }
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        width = surface->w / 4;
        height = surface->h / 4;
        SDL_FreeSurface(surface);
    }

    /**
     * @brief Destructor that releases the texture resource.
     */
    ~TextureFlyweight() override 
    {
        SDL_DestroyTexture(texture);
    }

    /**
     * @brief Renders the texture at the specified position.
     * 
     * @param renderer The SDL_Renderer used for rendering.
     * @param x The x-coordinate for the rendering position.
     * @param y The y-coordinate for the rendering position.
     */
    void Draw(SDL_Renderer* renderer, int x, int y) const override 
    {
        SDL_Rect dstRect = {x, y, width, height};
        SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
    }
};

/**
 * @brief Factory class for creating and managing Flyweight objects.
 * 
 * This class maintains a cache of flyweight objects to ensure that shared resources are reused
 * and avoids redundant creation of the same flyweights.
 */
class FlyweightFactory 
{
private:
    /**< Cache of flyweights mapped by file paths. */
    std::map<std::string, std::shared_ptr<Flyweight>> flyweights;

public:
    /**
     * @brief Retrieves a Flyweight object for the given file path.
     * 
     * If the flyweight for the file path does not exist, it creates a new one. Otherwise,
     * it reuses the existing flyweight from the cache.
     * 
     * @param renderer The SDL_Renderer used to create new textures if needed.
     * @param filePath The file path of the texture.
     * @return A shared pointer to the Flyweight object.
     */
    std::shared_ptr<Flyweight> GetFlyweight(SDL_Renderer* renderer, const std::string& filePath) 
    {
        if (flyweights.find(filePath) == flyweights.end()) 
        {
            std::cout << "Creating new Flyweight for: " << filePath << std::endl;
            flyweights[filePath] = std::make_shared<TextureFlyweight>(renderer, filePath);
        } 
        else 
        {
            std::cout << "Reusing existing Flyweight for: " << filePath << std::endl;
        }

        return flyweights[filePath];
    }
};


int main(int argc, char* argv[]) 
{
    // fix compilation warning 
    (void)argc;
    (void)argv;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    if (IMG_Init(IMG_INIT_PNG) == 0) 
    {
        std::cerr << "IMG_Init Error: " << IMG_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Flyweight Pattern with SDL", 100, 100, 900, 800, SDL_WINDOW_SHOWN);
    if (!window) 
    {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) 
    {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    FlyweightFactory factory;
    auto crateTexture = factory.GetFlyweight(renderer, "assets/crate.png");
    auto metalTexture = factory.GetFlyweight(renderer, "assets/metal.png");

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

        SDL_SetRenderDrawColor(renderer, 135, 206, 250, 255); 
        SDL_RenderClear(renderer);

        
        for (int i = 0; i < 6; ++i) 
        {
            crateTexture->Draw(renderer, 5 + i * 150, 10 + i);  
            metalTexture->Draw(renderer, 5 + i * 150, 200 + i);  
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}

