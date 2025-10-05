#include <SFML/Graphics.hpp>
#include <conio.h>
#include <string>
#include <SFML/OpenGL.hpp>

using namespace sf;

void PlayerMovementP(Sprite& player, Keyboard::Key key, float x, float y)
{
    if (Keyboard::isKeyPressed(key)) player.move(sf::Vector2f(x, y));
}

int WinMain()
{
    // Создание окна 750 на 750
    RenderWindow window(VideoMode({ 750, 750 }), "2DGame C++ (V 0.01)", Style::Close);

    // Игрок
    const Texture Playertexture("Graphics\\textures\\player.png");
    Sprite player(Playertexture);
    player.setScale(Vector2f(1.0, 1.0));
    player.setOrigin(Vector2f(27.0f, 64.0f));
    player.setPosition(Vector2f(375.0f, 375.0f));

	// Оружие1 - может быть будет больше
	const Texture gun1Texture("Graphics\\textures\\gun1.png");
    Sprite gun1(gun1Texture);
	gun1.setScale(Vector2f(1.0, 1.0));
	gun1.setOrigin(Vector2f(24.5f, 13.5f));
	gun1.setPosition(Vector2f(900.0f, 450.0f));

    // Ширифт
    const Font font("Graphics\\fonts\\arial.ttf");  
    
	// Текст с управлением
    Text upra(font, "\n\n\n\n\n\n\n\nManagement:\nAD - walking.", 20);
	
    // Создание камеры
    View camera;
	camera.setSize(Vector2f(1000.0f, 1000.0f));

	// Платформы
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

		// Ходьба по экрану на AD
        PlayerMovementP(player, Keyboard::Key::A, -0.2f, 0.0f);
        PlayerMovementP(player, Keyboard::Key::D, 0.2f, 0.0f);

        // Физика
		FloatRect playerBounds = player.getGlobalBounds();
		FloatRect rectangleBounds = rectangle.getGlobalBounds();
		FloatRect rectangle1Bounds = rectangle1.getGlobalBounds();
        FloatRect rectangle2Bounds = rectangle2.getGlobalBounds();
        FloatRect gun1Bounds = gun1.getGlobalBounds();

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


		// Оружия
        if (playerBounds.findIntersection(gun1Bounds))
        {
            gun1.setPosition(Vector2f(playerX + 30, playerY - 7));
			gun1.setScale(player.getScale());
		}
        else {
            if (gun1Bounds.findIntersection(rectangleBounds) ||
                gun1Bounds.findIntersection(rectangle1Bounds) ||
                gun1Bounds.findIntersection(rectangle2Bounds))
            {
                gun1.setPosition(Vector2f(gun1.getPosition().x, gun1.getPosition().y - 0.01f));
		    }
            else {
				gun1.move(Vector2f(0.0f, 0.05f));
            }
        }
		if (gun1.getPosition().y >= 750.0f) gun1.setPosition(Vector2f(900.0f, 450.0f));

		// Обновление камеры
        Vector2f camPos = camera.getCenter();
        camPos.x += (playerX - camPos.x) * 0.0005f;
        camPos.y += (playerY - camPos.y) * 0.0005f;
        camera.setCenter(camPos);

		// Текст с информацией
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

        // Рисуем всё
        window.draw(textInfo);
        window.draw(player);
		window.draw(gun1);
		window.draw(rectangle);
		window.draw(rectangle1);
		window.draw(rectangle2);
        window.draw(upra);

		// Обновляем экран
        window.display();
    }

}
