#include <SFML/Graphics.hpp>
#include <conio.h>
#include <string>

using namespace sf;

void PlayerMovementP(Sprite& player, Keyboard::Key key, float x, float y)
{
    if (Keyboard::isKeyPressed(key)) player.move(sf::Vector2f(x, y));
}

int WinMain()
{
    // Локализация
    setlocale(LC_ALL, "RU");
    // Создание окна 750 на 750
    RenderWindow window(VideoMode({ 750, 750 }), "2DGame C++ (V 0.01)", Style::Close);


    const Texture texture("Graphics\\textures\\player.png");
    Sprite player(texture);
    player.setScale(Vector2f(1.0, 1.0));
    player.setOrigin(Vector2f(27.0f, 64.0f));
    player.setPosition(Vector2f(375.0f, 375.0f));

    // Create a graphical text to display
    const Font font("Graphics\\fonts\\arial.ttf");  

    Text upra(font, "\n\n\n\n\n\n\n\nManagement:\nAD - walking.\nQE - changing the size.\nR - reset size.\nF - rotate player.", 20);
	View camera;
	camera.setSize(Vector2f(1000.0f, 1000.0f));

    const Texture grass("Graphics\\textures\\grass.png");
    RectangleShape rectangle;
    rectangle.setTexture(&grass);
    rectangle.setSize(Vector2f(500.0f, 100.0f));
    rectangle.setPosition(Vector2f(375.0f, 525.0f));
	rectangle.setOrigin(Vector2f(250.0f, 50.0f));

    RectangleShape rectangle1;
    rectangle1.setTexture(&grass);
    rectangle1.setSize(Vector2f(500.0f, 100.0f));
    rectangle1.setPosition(Vector2f(1000.0f, 525.0f));
    rectangle1.setOrigin(Vector2f(250.0f, 50.0f));

    RectangleShape rectangle2;
    rectangle2.setTexture(&grass);
    rectangle2.setSize(Vector2f(500.0f, 100.0f));
    rectangle2.setPosition(Vector2f(1625.0f, 525.0f));
    rectangle2.setOrigin(Vector2f(250.0f, 50.0f));


    while (window.isOpen())
    {

        while (const std::optional event = window.pollEvent()) { if (event->is<sf::Event::Closed>()) window.close(); }
        float playerX = player.getPosition().x, playerY = player.getPosition().y;
        // Изменение размера игрока на Q и E
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E)){
         player.setScale(Vector2f(player.getScale().x + 0.001, player.getScale().y + 0.001));
         camera.setSize(Vector2f(camera.getSize().x + 1, camera.getSize().y + 1));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
         player.setScale(Vector2f(player.getScale().x - 0.001, player.getScale().y - 0.001));
         camera.setSize(Vector2f(camera.getSize().x - 1, camera.getSize().y - 1));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)) {
            player.setScale(Vector2f(1.0, 1.0));
            camera.setSize(Vector2f(1000.0f, 1000.0f));
        }

        if (player.getScale().x < 0.1f && player.getScale().y < 0.1f) player.setScale(sf::Vector2f(0.1f, 0.1f));
        if (player.getScale().x > 2.0f && player.getScale().y > 2.0f) player.setScale(sf::Vector2f(2.0f, 2.0f));

        if (camera.getSize().y < 250.0f && camera.getSize().x < 250.0f) camera.setSize(Vector2f(250.0f, 250.0f));
        if (camera.getSize().y > 2500.0f && camera.getSize().x > 2500.0f) camera.setSize(Vector2f(2500.0f, 2500.0f));

		// Ходьба по экрану на WASD
        PlayerMovementP(player, Keyboard::Key::A, -0.2f, 0.0f);
        PlayerMovementP(player, Keyboard::Key::D, 0.2f, 0.0f);
        if (Keyboard::isKeyPressed(Keyboard::Key::F)) player.rotate(degrees(0.2f));

        // физика
		FloatRect playerBounds = player.getGlobalBounds();
		FloatRect rectangleBounds = rectangle.getGlobalBounds();
		FloatRect rectangle1Bounds = rectangle1.getGlobalBounds();
        FloatRect rectangle2Bounds = rectangle2.getGlobalBounds();

		player.move(Vector2f(0.0f, 0.05f));
        bool canJump;
        if (playerBounds.findIntersection(rectangleBounds) ||
            playerBounds.findIntersection(rectangle1Bounds) ||
			playerBounds.findIntersection(rectangle2Bounds))
        {
			player.setPosition(Vector2f(playerX, playerY - 0.01f));
            canJump = true;
        }
        else canJump = false;

        if (canJump == true && Keyboard::isKeyPressed(Keyboard::Key::Space)){
            player.move(Vector2f(0.0f, -150.0f));
        }
        else if (canJump == false && Keyboard::isKeyPressed(Keyboard::Key::Space)){
            player.move(Vector2f(0.0f, 0.0f));
        }

        if (playerY >= 800.0f)
        {
            player.setPosition(Vector2f(375.0f, 375.0f));
        }

		// Обновление камеры
        camera.setCenter(Vector2f(playerX, playerY));

        Text textInfo(font,
            "Window:"
            "\nPosition: " + std::to_string(window.getPosition().x) + " x " + std::to_string(window.getPosition().y) +
            "\nSize: " + std::to_string(window.getSize().x) + " x " + std::to_string(window.getSize().y) +
            "\nPlayer:" +
            "\nScale: " + std::to_string(player.getScale().x) + " x " + std::to_string(player.getScale().y) +
            "\nPosition: " + std::to_string(playerX) + " x " + std::to_string(playerY) +
            "\nRotation: " + std::to_string(player.getRotation().asDegrees()) +
            "\nCamera:" +
            "\nPosition: " + std::to_string(camera.getCenter().x) + " x " + std::to_string(camera.getCenter().y) +
            "\nSize: " + std::to_string(camera.getSize().x) + " x " + std::to_string(camera.getSize().y), 15
            );

		window.setView(camera);
		// Очищяем экран
        window.clear(Color(0, 145, 255));

        // Draw the string
        window.draw(textInfo);
        window.draw(player);
		window.draw(rectangle);
		window.draw(rectangle1);
		window.draw(rectangle2);
        window.draw(upra);

        // Update the window
        window.display();
    }

}
