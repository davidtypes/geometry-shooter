//
// Created by David Valenzuela on 12/28/23.
//

#include "../header/Game.h"

#include <cmath>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>

Game::Game(const std::string& config)
{
	//TODO add back the config into the init call
	std::cout << config << std::endl;
	init(config);
}

void Game::init(const std::string& s)
{
	//TODO .. add config parameter and read in config file..
	std::ifstream configFile;
	configFile.open(s);

	size_t windowWidth;
	size_t windowHeight;
	size_t framerate;

	if (!configFile.is_open())
	{
		std::cout << "Config file was unable to be opened. Ending program...\n";
		m_running = false;
	}
	else
	{
		std::cout << "Config file opened successfully.\n";
		std::string temp;
		configFile >> temp >> windowWidth >> windowHeight >> framerate >> temp;
		configFile >> temp >> temp >> temp >> temp >> temp >> temp;
		configFile >> temp >> m_playerConfig.SR >> m_playerConfig.CR >> m_playerConfig.FR >> m_playerConfig.FG
				   >> m_playerConfig.FB >> m_playerConfig.OR >> m_playerConfig.OG >> m_playerConfig.OB
				   >> m_playerConfig.OT
				   >> m_playerConfig.V >> m_playerConfig.S;

		//would be -> const std::string & config
		m_window.create(sf::VideoMode(windowWidth, windowHeight), "Assignment 2");
		m_window.setFramerateLimit(framerate);

		spawnPlayer();
	}
}

void Game::run()
{
	while (m_running)
	{
		m_entities.update();

		if (!m_paused)
		{
			sEnemySpawner();
			sMovement();
			sCollision();
			sLifespan();
		}

		sUserInput();
		sRender();

		if (!m_paused)
		{
			m_currentFrame++;
		}
	}
}

void Game::sMovement()
{
	m_player->cTransform->velocity = { 0, 0 };

	float leftWidth = m_player->cTransform->pos.x - m_player->cCollision->radius;
	float topHeight = m_player->cTransform->pos.y - m_player->cCollision->radius;
	float rightWidth = m_player->cTransform->pos.x + m_player->cCollision->radius;
	float bottomHeight = m_player->cTransform->pos.y + m_player->cCollision->radius;

	if (m_player->cInput->up && (topHeight > 0))
		m_player->cTransform->velocity.y = -m_playerConfig.S;
	if (m_player->cInput->left && (leftWidth > 0))
		m_player->cTransform->velocity.x = -m_playerConfig.S;
	if (m_player->cInput->down && (bottomHeight < m_window.getSize().y))
		m_player->cTransform->velocity.y = m_playerConfig.S;
	if (m_player->cInput->right && (rightWidth < m_window.getSize().x))
		m_player->cTransform->velocity.x = m_playerConfig.S;

	for (auto e : m_entities.getEntities())
	{
		e->cTransform->pos.x += e->cTransform->velocity.x;
		e->cTransform->pos.y += e->cTransform->velocity.y;
	}
}

void Game::sUserInput()
{
//no movement logic, just input
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			m_running = false;
		}

		if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
		{
			bool isKeyPressed = (event.type == sf::Event::KeyPressed);

			switch (event.key.code)
			{
			case sf::Keyboard::W:
				m_player->cInput->up = isKeyPressed;
				break;
			case sf::Keyboard::A:
				m_player->cInput->left = isKeyPressed;
				break;
			case sf::Keyboard::S:
				m_player->cInput->down = isKeyPressed;
				break;
			case sf::Keyboard::D:
				m_player->cInput->right = isKeyPressed;
				break;
			case sf::Keyboard::Escape:
				if (isKeyPressed)
				{
					setPaused(!m_paused);
				}
			default:
				break;
			}
		}

		if (!m_paused)
		{
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					spawnBullet(m_player, Vec2(event.mouseButton.x, event.mouseButton.y));
				}
				if (event.mouseButton.button == sf::Mouse::Right)
				{
					//call special
				}
			}
		}
	}
}

void Game::sLifespan()
{
	for (auto& b : m_entities.getEntities("bullet"))
	{
		b->cLifespan->remaining--;
		if (b->cLifespan->remaining == 0)
			b->destroy();
	}
	for (auto& b : m_entities.getEntities("small-enemy"))
	{
		b->cLifespan->remaining--;
		if (b->cLifespan->remaining == 0)
			b->destroy();
	}
}

void Game::sRender()
{
	m_window.clear();

	for (auto e : m_entities.getEntities())
	{
		e->cShape->circle.setPosition(e->cTransform->pos.x, e->cTransform->pos.y);
		e->cTransform->angle += 1.0f;
		e->cShape->circle.setRotation(e->cTransform->angle);

		m_window.draw(e->cShape->circle);
	}

	m_window.display();
}

void Game::sEnemySpawner()
{
	if (m_currentFrame - m_last_EnemySpawnTime > 90)
	{
		spawnEnemy();
	}
}

void Game::sCollision()
{
	for (auto& b : m_entities.getEntities("bullet"))
	{
		for (auto& e : m_entities.getEntities("enemy"))
		{
			if (b->cTransform->pos.dist(e->cTransform->pos) < (b->cCollision->radius + e->cCollision->radius))
			{
				spawnSmallEnemies(e);
				b->destroy();
				e->destroy();
			}
		}
		for (auto& e : m_entities.getEntities("small-enemy"))
		{
			if (b->cTransform->pos.dist(e->cTransform->pos) < (b->cCollision->radius + e->cCollision->radius))
			{
				b->destroy();
				e->destroy();
			}
		}
	}

	for (auto& e : m_entities.getEntities("enemy"))
	{
		float leftWidth = e->cTransform->pos.x - e->cCollision->radius;
		float topHeight = e->cTransform->pos.y - e->cCollision->radius;
		float rightWidth = e->cTransform->pos.x + e->cCollision->radius;
		float bottomHeight = e->cTransform->pos.y + e->cCollision->radius;
		if (leftWidth < 0 && e->cTransform->velocity.x < 0)
			e->cTransform->velocity.x *= -1;
		if (rightWidth > m_window.getSize().x && e->cTransform->velocity.x > 0)
			e->cTransform->velocity.x *= -1;
		if (topHeight < 0 && e->cTransform->velocity.y < 0)
			e->cTransform->velocity.y *= -1;
		if (bottomHeight > m_window.getSize().y && e->cTransform->velocity.y > 0)
			e->cTransform->velocity.y *= -1;

		if (e->cTransform->pos.dist(m_player->cTransform->pos) < (e->cCollision->radius + m_player->cCollision->radius))
		{
			m_player->destroy();
			spawnPlayer();
		}
	}

	for (auto& e : m_entities.getEntities("small-enemy"))
	{
		if (e->cTransform->pos.dist(m_player->cTransform->pos) < (e->cCollision->radius + m_player->cCollision->radius))
		{
			m_player->destroy();
			spawnPlayer();
		}
	}
}

void Game::setPaused(bool paused)
{
	m_paused = paused;
}

void Game::spawnPlayer()
{
	float cx = m_window.getSize().x / 2;
	float cy = m_window.getSize().y / 2;

	auto entity = m_entities.addEntity("player");
	entity->cTransform = std::make_shared<CTransform>(Vec2(cx, cy),
		Vec2(0, 0),
		0);
	entity->cShape = std::make_shared<CShape>(m_playerConfig.SR,
		8,
		sf::Color(m_playerConfig.FR, m_playerConfig.FG, m_playerConfig.FB),
		sf::Color(255, 0, 0),
		4.0f);
//    entity->cSprite = std::make_shared<CSprite>("./katarapixel.png");
	entity->cInput = std::make_shared<CInput>();
	entity->cCollision = std::make_shared<CCollision>(m_playerConfig.CR);
	m_player = entity;
}

void Game::spawnEnemy()
{
	srand(time(0));
	Vec2 randPos = Vec2(rand() % m_window.getSize().x + 1, rand() % m_window.getSize().y + 1);
	Vec2 randDir = Vec2(rand() % 20 - 10, rand() % 20 - 10);
	randDir.normalize();

	auto entity = m_entities.addEntity("enemy");
	entity->cTransform = std::make_shared<CTransform>(randPos, randDir * 3, 0);
	entity->cShape = std::make_shared<CShape>(32.0f,
		rand() % 6 + 3,
		sf::Color(10, 10, 10),
		sf::Color(0, 255, 0),
		4.0f);
	entity->cCollision = std::make_shared<CCollision>(32.0f);
	m_last_EnemySpawnTime = m_currentFrame;
}

void Game::spawnSmallEnemies(std::shared_ptr<Entity> entity)
{
	sf::CircleShape circle = entity->cShape->circle;
	for (int i = 0; i < circle.getPointCount(); i++)
	{
		float interval = 360 / circle.getPointCount();
		auto e = m_entities.addEntity("small-enemy");
		e->cTransform = std::make_shared<CTransform>(entity->cTransform->pos,
			Vec2(cosf(interval * i * M_PI / 180), sinf(interval * i * M_PI / 180))
				* 3,
			0);
		e->cShape = std::make_shared<CShape>(circle.getRadius() / 2,
			circle.getPointCount(),
			sf::Color(10, 10, 10),
			sf::Color(0, 255, 0),
			4.0f);
		e->cLifespan = std::make_shared<CLifespan>(60);
		e->cCollision = std::make_shared<CCollision>(entity->cCollision->radius);
	}
}

void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2& mousePos)
{
	Vec2 dir = (mousePos - entity->cTransform->pos);
	dir.normalize();

	auto bullet = m_entities.addEntity("bullet");
	bullet->cTransform = std::make_shared<CTransform>(entity->cTransform->pos,
		dir * 20.0f,
		0);
	bullet->cShape = std::make_shared<CShape>(10.0f, 20, sf::Color::White, sf::Color::White, 2.0f);
	bullet->cLifespan = std::make_shared<CLifespan>(50);
	bullet->cCollision = std::make_shared<CCollision>(10.0f);
}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity)
{

}
