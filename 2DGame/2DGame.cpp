#include <SFML/Graphics.hpp>
#include <conio.h>
#include <string>

using namespace sf;

int WinMain()
{
	setlocale(LC_ALL, "RU");
    // Create the main window
    RenderWindow window(VideoMode({ 750, 750 }), "2DGame C++", Style::Close);
    const Image icon("Graphics\\textures\\sprite.png");
    auto size = Vector2u{ 512, 512 };
    window.setIcon(size, icon.getPixelsPtr());


    const Texture texture("Graphics\\textures\\sprite.png");
    Sprite player(texture);
    player.setScale(Vector2f(0.3, 0.3));
    player.setOrigin(Vector2f(170.0f, 256.0f));
    player.setPosition(Vector2f(375.0f, 375.0f));

    // Create a graphical text to display
    const Font font("Graphics\\fonts\\arial.ttf");  

    Text upra(font, "\n\n\n\n\n\n\n\nManagement:\nAD - walking.\nQE - changing the size.\nR - reset size", 20);

	View camera;
	camera.setSize(Vector2f(1000.0f, 1000.0f));

    const Texture grass("Graphics\\textures\\grass.png");
    RectangleShape rectangle;
    rectangle.setSize(Vector2f(500.0f, 100.0f));
    rectangle.setPosition(Vector2f(375.0f, 525.0f));
	rectangle.setOrigin(Vector2f(250.0f, 50.0f));
	rectangle.setTexture(&grass);

    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        while (const std::optional event = window.pollEvent()) { if (event->is<sf::Event::Closed>()) window.close(); }
        float playerX = player.getPosition().x, playerY = player.getPosition().y;
        // Изменение размера игрока на Q и E
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E)){
         player.setScale(sf::Vector2f(player.getScale().x + 0.001, player.getScale().y + 0.001));
         camera.setSize(sf::Vector2f(camera.getSize().x + 1, camera.getSize().y + 1));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
         player.setScale(sf::Vector2f(player.getScale().x - 0.001, player.getScale().y - 0.001));
         camera.setSize(sf::Vector2f(camera.getSize().x - 1, camera.getSize().y - 1));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)) {
            player.setScale(Vector2f(0.3, 0.3));
            camera.setSize(Vector2f(1000.0f, 1000.0f));
        }

        if (player.getScale().x < 0.1f && player.getScale().y < 0.1f) player.setScale(sf::Vector2f(0.1f, 0.1f));
        if (player.getScale().x > 1.0f && player.getScale().y > 1.0f) player.setScale(sf::Vector2f(1.0f, 1.0f));

        if (camera.getSize().y < 250.0f && camera.getSize().x < 250.0f) camera.setSize(Vector2f(250.0f, 250.0f));
        if (camera.getSize().y > 1100.0f && camera.getSize().x > 1100.0f) camera.setSize(Vector2f(1100.0f, 1100.0f));

		// Ходьба по экрану на WASD
        //if (Keyboard::isKeyPressed(Keyboard::Key::W)) player.move(sf::Vector2f(0.0f, -0.2f));
        //if (Keyboard::isKeyPressed(Keyboard::Key::S)) player.move(sf::Vector2f(0.0f, 0.2f));
        if (Keyboard::isKeyPressed(Keyboard::Key::A)) player.move(sf::Vector2f(-0.2f, 0.0f));
        if (Keyboard::isKeyPressed(Keyboard::Key::D)) player.move(sf::Vector2f(0.2f, 0.0f));
        bool canJump;
        if (Keyboard::isKeyPressed(Keyboard::Key::F)) player.rotate(degrees(0.2f));

        // физика
		player.move(Vector2f(0.0f, 0.05f));

        if (player.getGlobalBounds().findIntersection(rectangle.getGlobalBounds())) {
			player.setPosition(Vector2f(playerX, playerY - 0.01f));
            canJump = true;
        }
        else
        {
            canJump = false;
        }
        if (canJump == true && Keyboard::isKeyPressed(Keyboard::Key::Space)){
            player.move(sf::Vector2f(0.0f, -200.0f));
        }
        else if (canJump == false && Keyboard::isKeyPressed(Keyboard::Key::Space)){
            player.move(sf::Vector2f(0.0f, 0.0f));
        }

		// Обновление камеры
        camera.setCenter(Vector2f(playerX, playerY));


        Text win(font, "Window:", 15);
        Text winPos(font, "\nPosition: " + std::to_string(window.getPosition().x) + " x " + std::to_string(window.getPosition().y), 15);
        Text winSize(font, "\nSize: " + std::to_string(window.getSize().x) + " x " + std::to_string(window.getSize().y), 15);
        Text playerText(font, "\n\nPlayer:", 15);
        Text scaleText(font, "\n\n\nScale: " + std::to_string(player.getScale().x) + " x " + std::to_string(player.getScale().y), 15);
        Text PosText(font, "\n\n\n\nPosition: " + std::to_string(playerX) + " x " + std::to_string(playerY), 15);
        Text RotText(font, "\n\n\n\n\nRotation: " + std::to_string(player.getRotation().asDegrees()), 15);
        Text cameraText(font, "\n\n\n\n\n\nCamera:", 15);
        Text camPosText(font, "\n\n\n\n\n\n\nPosition: " + std::to_string(camera.getCenter().x) + " x " + std::to_string(camera.getCenter().y), 15);
        Text camSizeText(font, "\n\n\n\n\n\n\n\nSize: " + std::to_string(camera.getSize().x) + " x " + std::to_string(camera.getSize().y), 15);

        win.setFillColor(sf::Color::Black);
        playerText.setFillColor(sf::Color::Black);
        cameraText.setFillColor(sf::Color::Black);
		window.setView(camera);
		// Очищяем экран
        window.clear(sf::Color(0, 145, 255));

        // Draw the string
        window.draw(player);
        window.draw(playerText);
		window.draw(rectangle);
        window.draw(win);   
        window.draw(winSize);
        window.draw(scaleText);
        window.draw(PosText);
        window.draw(RotText);
		window.draw(cameraText);
        window.draw(camPosText);
        window.draw(camSizeText);
		window.draw(upra);

        // Update the window
        window.display();
    }

}