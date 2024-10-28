#include <SFML/Graphics.hpp>
#include <iostream>
#include <filesystem>
#include <chrono>
#include <deque>

// methods
void renderLoop(sf::RenderWindow &window, sf::View &view);
void updateLoop();
sf::View getLetterboxView(sf::View view, int windowWidth, int windowHeight);
void resizeSprite(sf::Sprite &sprite, sf::Vector2f size);
//

const unsigned int resX = 320, resY = 180;

long deltaTime = 1;
long lastTime = -1;

sf::Sprite bgSprite;
sf::Sprite plrSprite;

float momentum = 0;
bool clicking = false, fall = false;

std::deque<sf::Sprite> walls;

int main()
{
    
    sf::RenderWindow window({resX * 3, resY * 3}, "Flappy Blocky");
    window.setFramerateLimit(144);

    sf::View view;
    view.setSize( resX, resY );
    view.setCenter( view.getSize().x / 2, view.getSize().y / 2 );
    view = getLetterboxView( view, resX, resY ); 

    // Load textures
    sf::Texture bgTex;
    if(!bgTex.loadFromFile("data/images/balls 2.jpg")) return 1;
    bgSprite = sf::Sprite(bgTex);
    resizeSprite(bgSprite, sf::Vector2f(resX, resY));
    sf::Texture plrTex;
    if(!plrTex.loadFromFile("data/images/plr.png")) return 1;
    plrSprite = sf::Sprite(plrTex);
    resizeSprite(plrSprite, sf::Vector2f(32, 32));
    plrSprite.setPosition(30, 100);
    plrSprite.setOrigin(sf::Vector2f(16.f, 16.f));

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

void render(sf::RenderTexture &rend){
    
    rend.draw(bgSprite);
    rend.draw(plrSprite);

}
void renderLoop(sf::RenderWindow &window, sf::View &view){
    window.clear();

    window.setView(view);
    sf::RenderTexture rend;
    if(!rend.create(resX, resY)){
        return;
    }
    render(rend);
    sf::Sprite rendSprite(rend.getTexture());
    resizeSprite(rendSprite, sf::Vector2f(resX, resY));
    rendSprite.setScale(rendSprite.getScale().x, rendSprite.getScale().y * -1);
    rendSprite.move(0, resY);
    window.draw(rendSprite);

    window.display();
}

void update(){

    for(sf::Sprite sprite: walls){
        sprite.move(sf::Vector2f(-0.1f * deltaTime, 0));
    }
    bool loop = true;
    while(loop){
        loop = false;
        
    }
    
    momentum += (0.005f * deltaTime);
    if(momentum > 1.5f) momentum = 1.5f;

    if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
        if(!clicking){
            clicking = true;
            momentum = -1.5f;
            fall = true;
        }
    }else{
        clicking = false;
    }

    if(fall){
        plrSprite.move(0, momentum);
        float percent = (momentum / 1.5f);
        plrSprite.setRotation(30 * percent);

        if(plrSprite.getPosition().y < 0){
            plrSprite.setPosition(30, 0);
            momentum = 0;
        } 
        if(plrSprite.getPosition().y > resY){
            // death o.o
        }
    }else{
        plrSprite.setRotation(0);
    }

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

void resizeSprite(sf::Sprite &sprite, sf::Vector2f size){
    sf::Texture texture = *(sprite.getTexture());
    sf::Vector2u texSize = texture.getSize();

    float scaleX = size.x / (float)texSize.x;
    float scaleY = size.y / (float)texSize.y;
    sprite.setScale(sf::Vector2f(scaleX, scaleY));
}
