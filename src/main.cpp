#include <SFML/Graphics.hpp>
#include <iostream>
#include <filesystem>
#include <chrono>

// methods
void renderLoop(sf::RenderWindow &window, sf::View &view);
void updateLoop();
sf::View getLetterboxView(sf::View view, int windowWidth, int windowHeight);
//

long deltaTime = 1;
long lastTime = -1;

sf::Sprite bgSprite;

int main()
{
    unsigned int resX = 700, resY = 500;
    sf::RenderWindow window({resX, resY}, "Flappy Blocky");
    window.setFramerateLimit(144);

    sf::View view;
    view.setSize( resX, resY );
    view.setCenter( view.getSize().x / 2, view.getSize().y / 2 );
    view = getLetterboxView( view, resX, resY ); 

    // Load textures
    sf::Texture bgTex;
    if(!bgTex.loadFromFile("data/images/ryo.png")) return 1;
    bgSprite = sf::Sprite(bgTex);
    bgSprite.setScale(sf::Vector2f(3.f, 2.f));

    while (window.isOpen())
    {
        for (auto event = sf::Event(); window.pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (event.type == sf::Event::Resized){
                unsigned int width = event.size.width;
                unsigned int height = event.size.height;
                if(event.size.width < resX){
                    window.setSize(sf::Vector2u(resX, height));
                    width = resX;
                }
                if(event.size.height < resY){
                    window.setSize(sf::Vector2u(width, resY));
                    height = resY;
                }
                view = getLetterboxView( view, width, height);
            }
        }

        updateLoop();
        renderLoop(window, view);

    }
}

void render(sf::RenderWindow &window){
    
    window.draw(bgSprite);

}
void renderLoop(sf::RenderWindow &window, sf::View &view){
    window.clear();

    window.setView(view);
    render(window);

    window.display();
}

void update(){
    
    

}
void updateLoop(){
    // Calculate deltaTime
    long now = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
    deltaTime = lastTime != -1 ? now - lastTime : 0;
    lastTime = now;
    
    update();
}


sf::View getLetterboxView(sf::View view, int windowWidth, int windowHeight) {

    // Compares the aspect ratio of the window to the aspect ratio of the view,
    // and sets the view's viewport accordingly in order to achieve a letterbox effect.
    // A new view (with a new viewport set) is returned.

    float windowRatio = (float) windowWidth / (float) windowHeight;
    float viewRatio = view.getSize().x / (float) view.getSize().y;
    float sizeX = 1;
    float sizeY = 1;
    float posX = 0;
    float posY = 0;

    bool horizontalSpacing = true;
    if (windowRatio < viewRatio)
        horizontalSpacing = false;

    // If horizontalSpacing is true, the black bars will appear on the left and right side.
    // Otherwise, the black bars will appear on the top and bottom.

    if (horizontalSpacing) {
        sizeX = viewRatio / windowRatio;
        posX = (1 - sizeX) / 2.f;
    }

    else {
        sizeY = windowRatio / viewRatio;
        posY = (1 - sizeY) / 2.f;
    }

    view.setViewport( sf::FloatRect(posX, posY, sizeX, sizeY) );

    return view;
}
