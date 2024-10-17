#include <SFML/Graphics.hpp>
#include <iostream>
#include <filesystem>

// methods
void renderLoop(sf::RenderWindow &window);
//

int main()
{

    sf::RenderWindow window({700u, 500u}, "Balls.");
    window.setFramerateLimit(144);

    while (window.isOpen())
    {
        for (auto event = sf::Event(); window.pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        renderLoop(window);

    }
}

void render(sf::RenderTexture &render){
    
    // sf::CircleShape circol(50.f);

    // circol.setFillColor(sf::Color(0, 255, 0));
    // circol.setOutlineThickness(3.f);
    // circol.setOutlineColor(sf::Color::White);
    // render.draw(circol);

    sf::Texture texture;
    if(!texture.loadFromFile("data/images/ryo.png")){
        return;
    }
    sf::Sprite sprite(texture);
    sprite.setPosition(sf::Vector2f(30.f, 50.f));
    sprite.setRotation(50.f);
    render.draw(sprite);


}
void renderLoop(sf::RenderWindow &window){
    sf::RenderTexture tex;
    if(!tex.create(700, 500)){
        exit(3);
    }

    tex.clear();
    render(tex);
    tex.display();
    
    window.clear();

    sf::Sprite texSprite(tex.getTexture());
    window.draw(texSprite);

    window.display();
}
