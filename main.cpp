#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

#include <string>
#include <vector>
#include <sstream>
#include <thread>

#ifndef unix
#include <windows.h>
#endif

#include "NetworkClient.hpp"
#include "GenerationManager.hpp"
#include "PlayerManager.hpp"
#include "AssetManager.hpp"
#include "TileManager.hpp"
#include "ShopManager.hpp"
#include "icon.h"

//variables
const int balance_to_win = 101;
const short side = 10;
TileManager TileList[side * side];

void HideConsole()
{
#ifndef unix
    HWND Hide;
    AllocConsole();
    Hide = FindWindowA("ConsoleWindowClass", NULL);
    ShowWindow(Hide, 1);
#endif
}

//Game Funcs
void Gather(TileManager &tile, sf::Texture &tile_t, PlayerManager &pl, sf::Text &balance_txt, sf::Texture &rot_t, int &cost, int &growtime, sf::Sound& snd) {
    pl.money_p += cost;
    balance_txt.setString(std::to_string(pl.money_p) + "$");

    tile.tile.setTexture(tile_t);
    tile.growtime = growtime;
    tile.lives -= 1;
    snd.play();

    if (tile.lives == 0) {
        tile.growtime = -1;
        tile.tile.setTexture(rot_t);
    }

}

void Plant(TileManager &tile, sf::Texture &tile_t, sf::Text &balance_txt, PlayerManager &pl, int &cost, int &growtime, sf::Sound &snd) {
    if (pl.money_p >= cost) {
        pl.money_p -= cost;
        balance_txt.setString(std::to_string(pl.money_p) + "$");

        tile.tile.setTexture(tile_t);
        tile.growtime = growtime;
        tile.lives = 3;
        snd.play();
    }
}

void setCursor(sf::RenderWindow &window, sf::Sprite &cur_s, float &x, float &y, bool isOn) {
    if (isOn) {
        window.setMouseCursorVisible(false);
        cur_s.setPosition(x, y);
    }
    else {
        window.setMouseCursorVisible(true);
        cur_s.setPosition(-100, -100);
    }

}

//Network
void receiveMessages(sf::UdpSocket& socket, AssetManager &as) {
    sf::Packet packet;
    std::size_t received;
    sf::IpAddress sender;
    unsigned short port;

    while (true) {
        if (socket.receive(packet, sender, port) != sf::Socket::Done) {
            std::cerr << "Error receiving data" << std::endl;
            continue;
        }
        float x;
        float y;
        string t;
        string c;
        string n;
        sf::Uint16 m;
        packet >> x >> y >> t >> c >> n >> m;

        if (n != "") {

            if (m >= balance_to_win) {
                as.win_txt.setString(n + "Wins!");
                as.back_s.setPosition(0, 0);
            }

            as.op_nickname_txt.setString(n);
            as.op_balance_txt.setString(std::to_string(m));

            if (c == "r") {
                as.op_nickname_txt.setFillColor(sf::Color::Red);
                as.op_balance_txt.setFillColor(sf::Color::Red);
            }

            if (c == "b") {
                as.op_nickname_txt.setFillColor(sf::Color::Blue);
                as.op_balance_txt.setFillColor(sf::Color::Blue);
            }
        }

        for (int i = 0; i < std::size(TileList); i++) {
            if (TileList[i].tile.getGlobalBounds().contains(x, y)) {
                if (c == "r") {
                    TileList[i].tile.setColor(sf::Color::Red);
                    TileList[i].color = "r";
                }

                if (c == "b") {
                    TileList[i].tile.setColor(sf::Color::Blue);
                    TileList[i].color = "b";
                }

                if(t == "grass_clock_t")
                    TileList[i].tile.setTexture(as.grass_clock_t); {}
                if (t == "tomato_bed_1_t")
                    TileList[i].tile.setTexture(as.tomato_bed_1_t);
                if (t == "tomato_bed_2_t")
                    TileList[i].tile.setTexture(as.tomato_bed_2_t);
                if (t == "tomato_bed_3_t")
                    TileList[i].tile.setTexture(as.tomato_bed_3_t);
                if (t == "tomato_bed_rot_t")
                    TileList[i].tile.setTexture(as.tomato_bed_rot_t);
                if (t == "grass_house_t")
                    TileList[i].tile.setTexture(as.grass_house_t);
                if (t == "dirt_t")
                    TileList[i].tile.setTexture(as.dirt_t);
            }
        }
    }
}

int main()
{
    //Set managers
    GenerationManager ge;
    PlayerManager pl{};
    AssetManager as;
    TileManager ti;
    ShopManager sh;

    //Window
    sf::RenderWindow window(sf::VideoMode(width, height), "Open Farm Simulator", sf::Style::Close);
    window.setFramerateLimit(60);
    window.setIcon(32, 32, MagickImage);
    HideConsole();

    //Set player values
    pl.NewGameSettings();

    //Load Game Assets
    as.setGameUI(pl.money_p, pl.max_worker);
    as.setMenuUi();
    as.setAudio();
    as.setTiles();

    //Map generating
    int level[side*side];
    for (int i = 0; i < std::size(level); i++) {
        int start = 0;
        int end = side;
        int x = rand() % (end - start + 1) + start;

        if (x < 3)
            level[i] = 1;
        else
            level[i] = 0;
    }

    unsigned int n = 0;
    for (unsigned int i = 0; i < side; i++)
        for (unsigned int j = 0; j < side; j++)
        {
            ti.tile.setScale(sf::Vector2f(1.f, 1.f));
            ti.tile.setOrigin(50, 50);
            ti.tile.setTexture(as.grass_t);
            ti.color = "";

            //Set tile texture
            int tileNumber = level[i + j * 10];
            if (tileNumber == 0) {
                ti.tile.setTexture(as.grass_t);
                ti.isWater = false;
            }

            else {
                ti.tile.setTexture(as.water_0_t);
                ti.isWater = true;
            }

            ti.tile.setPosition((width-side*100)/2 + i * 100, j * 100);

            //Values of tile
            ti.growtime = -1;
            ti.digtime = -1;
            ti.planttime = -1;
            ti.gathertime = -1;
            TileList[n] = ti;

            n += 1;
        }

    //Network
    sf::UdpSocket socket;

    // Устанавливаем серверный IP и порт
    sf::IpAddress serverIp;
    std::cout << "Write server IP";
    std::cin >> serverIp;

    unsigned short serverPort = 54000;

    // Сообщаем серверу о своем подключении
    const std::string connectMsg = "CONNECT";
    if (socket.send(connectMsg.c_str(), connectMsg.size() + 1, serverIp, serverPort) != sf::Socket::Done) {
        std::cerr << "Error sending connect message" << std::endl;
        return -1;
    }

    std::cout << "Connected to server " << serverIp << ":" << serverPort << std::endl;

    // Запуск потока для получения сообщений
    std::thread receiverThread(receiveMessages, std::ref(socket), std::ref(as));

    std::cout << "Enter nickname";
    std::string nickname;
    std::cin >> nickname;

    std::cout << "Enter team (r or b)";
    std::string team;
    std::cin >> team;

    sf::Color teamColor;

    if (team == "r") {
        teamColor = sf::Color::Red;
    }

    else if (team == "b") {
        teamColor = sf::Color::Blue;
    }

    else
        window.close();

    //Update
    bool pr = false;
    bool isReady = false;

    bool isTomato = false;
    bool isShovel = false;
    bool isBasket = false;
    bool isBuild = false;

    sf::Sprite GameUiButtons[4]{as.shovel_s, as.basket_s, as.seed_s, as.build_s};

    static sf::Clock clockGrow;
    static sf::Clock clockAnim;
    static sf::Clock clockBuild;

    while (window.isOpen())
    {
        if (pl.money_p >= balance_to_win) {
            as.win_txt.setString("You win!");
            as.back_s.setPosition(0, 0);
        }

        //GrowSystem
        if (clockGrow.getElapsedTime().asSeconds() > 1)
        {
            for (unsigned int i = 0; i < std::size(TileList); i++) {
                if (TileList[i].growtime != -1) {
                    TileList[i].growtime -= 1;
                }

                if (TileList[i].growtime == 5) {
                    TileList[i].tile.setTexture(as.tomato_bed_2_t);

                    float x = TileList[i].tile.getPosition().x;
                    float y = TileList[i].tile.getPosition().y;
                    string t = "tomato_bed_2_t";
                    sf::Packet packet;
                    packet << x << y << t;

                    if (socket.send(packet, serverIp, serverPort) != sf::Socket::Done) {
                        std::cerr << "Error sending message" << std::endl;
                    }
                }

                if (TileList[i].growtime == 0) {
                    TileList[i].tile.setTexture(as.tomato_bed_3_t);

                    float x = TileList[i].tile.getPosition().x;
                    float y = TileList[i].tile.getPosition().y;
                    string t = "tomato_bed_3_t";
                    sf::Packet packet;
                    packet << x << y << t;

                    if (socket.send(packet, serverIp, serverPort) != sf::Socket::Done) {
                        std::cerr << "Error sending message" << std::endl;
                    }

                    TileList[i].growtime = -1;
                }
            }

            clockGrow.restart();
        }

        //AnimSystem
        if (clockAnim.getElapsedTime().asSeconds() > 1)
        {
            for (unsigned int i = 0; i < std::size(TileList); i++)
                if (TileList[i].isWater) TileList[i].tile.setTexture(as.water_1_t);
        }

        if (clockAnim.getElapsedTime().asSeconds() > 2)
        {
            for (unsigned int i = 0; i < std::size(TileList); i++)
                if(TileList[i].isWater) TileList[i].tile.setTexture(as.water_0_t);

            clockAnim.restart();
        } 

        //BuildSystem
        if (clockBuild.getElapsedTime().asSeconds() > 1)
        {
            for (unsigned int i = 0; i < std::size(TileList); i++) {
                if (TileList[i].digtime != -1) {
                    TileList[i].digtime -= 1;
                }

                if (TileList[i].digtime == 0) {
                    TileList[i].tile.setTexture(as.dirt_t);
                    TileList[i].digtime = -1;

                    float x = TileList[i].tile.getPosition().x;
                    float y = TileList[i].tile.getPosition().y;
                    string t = "dirt_t";
                    sf::Packet packet;
                    packet << x << y << t;

                    if (socket.send(packet, serverIp, serverPort) != sf::Socket::Done) {
                        std::cerr << "Error sending message" << std::endl;
                    }

                    pl.cur_worker += 1;
                    as.worker_txt.setString(std::to_string(pl.cur_worker) + "/" + std::to_string(pl.max_worker));
                }
            }

            clockBuild.restart();
        }


        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            
            sf::Vector2f mousePositionFloat = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));

            //Cursors
            if (isTomato)
                setCursor(window, as.tomato_cursor_s, mousePositionFloat.x, mousePositionFloat.y, true);
            else 
                setCursor(window, as.tomato_cursor_s, mousePositionFloat.x, mousePositionFloat.y, false);

            if (isShovel)
                setCursor(window, as.shovel_cursor_s, mousePositionFloat.x, mousePositionFloat.y, true);
            else
                setCursor(window, as.shovel_cursor_s, mousePositionFloat.x, mousePositionFloat.y, false);

            if (isBasket)
                setCursor(window, as.basket_cursor_s, mousePositionFloat.x, mousePositionFloat.y, true);
            else
                setCursor(window, as.basket_cursor_s, mousePositionFloat.x, mousePositionFloat.y, false);

            if (isBuild)
                setCursor(window, as.build_cursor_s, mousePositionFloat.x, mousePositionFloat.y, true);
            else
                setCursor(window, as.build_cursor_s, mousePositionFloat.x, mousePositionFloat.y, false);

            if (pr == false)
            {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                {
                    //Menu UI Clicking 
                    if (as.button_newgame_s.getGlobalBounds().contains(mousePositionFloat))
                        as.CloseMenu();

                    if (as.button_exitgame_s.getGlobalBounds().contains(mousePositionFloat))
                        window.close();

                    if (as.pause_s.getGlobalBounds().contains(mousePositionFloat) && !as.back_s.getGlobalBounds().contains(mousePositionFloat)) {
                        as.setMenuUi();
                        as.button_newgame_txt.setString("Continue Farming");
                        as.button_newgame_txt.move(-40, 0);
                    }

                    //Game UI Clicking
                    for (int i = 0; i < std::size(GameUiButtons); i++) {
                        if (GameUiButtons[i].getGlobalBounds().contains(mousePositionFloat) && !as.back_s.getGlobalBounds().contains(mousePositionFloat)) {
                            switch (i)
                            {
                            //Shovel
                            case 0:
                                isShovel = !isShovel;
                                isBasket = false;
                                isTomato = false;
                                isBuild = false;
                                break;
                            //Basket
                            case 1:
                                isShovel = false;
                                isBasket = !isBasket;
                                isTomato = false;
                                isBuild = false;
                                break;
                            //Seed
                            case 2:
                                isShovel = false;
                                isBasket = false;
                                isTomato = !isTomato;
                                isBuild = false;
                                break;
                            //Build
                            case 3:
                                isShovel = false;
                                isBasket = false;
                                isTomato = false;
                                isBuild = !isBuild;
                                break;
                            }
                            pr = true;
                        }

                    }


                    //Editing Tiles
                    for (unsigned int i = 0; i < std::size(TileList); i++)
                    {
                        if (TileList[i].tile.getGlobalBounds().contains(mousePositionFloat) && !as.panel_s.getGlobalBounds().contains(mousePositionFloat) && pl.cur_worker != 0 && (TileList[i].color == "" || TileList[i].color == team))
                        {
                            //Dig bed
                            if (isShovel == true && (TileList[i].tile.getTexture() == &as.grass_t || TileList[i].tile.getTexture() == &as.tomato_bed_rot_t)) {
                                TileList[i].digtime = 3;
                                TileList[i].tile.setTexture(as.grass_clock_t);

                                TileList[i].color = team;
                                if (team == "r")
                                    TileList[i].tile.setColor(sf::Color::Red);

                                if (team == "b")
                                    TileList[i].tile.setColor(sf::Color::Blue);

                                float x = TileList[i].tile.getPosition().x;
                                float y = TileList[i].tile.getPosition().y;
                                string t = "grass_clock_t";
                                string c = team;

                                string n = nickname;
                                sf::Uint16 m = pl.money_p;

                                sf::Packet packet;
                                packet << x << y << t << c << n << m;

                                if (socket.send(packet, serverIp, serverPort) != sf::Socket::Done) {
                                    std::cerr << "Error sending message" << std::endl;
                                }

                                pl.cur_worker -= 1;
                                as.worker_txt.setString(std::to_string(pl.cur_worker) + "/" + std::to_string(pl.max_worker));

                                as.dig_sound_a.play();
                                isReady = true;
                            }

                            //Plant tomato
                            if (isTomato == true && TileList[i].tile.getTexture() == &as.dirt_t) {
                                Plant(TileList[i], as.tomato_bed_1_t, as.balance_txt, pl, sh.tomato_seed_buy_cost, TileList[i].tomato_grow_time, as.plant_sound_a);

                                string t = "tomato_bed_1_t";
                                float x = TileList[i].tile.getPosition().x;
                                float y = TileList[i].tile.getPosition().y;
                                string c = team;

                                string n = nickname;
                                sf::Uint16 m = pl.money_p;

                                TileList[i].color = team;
                                if (team == "r")
                                    TileList[i].tile.setColor(sf::Color::Red);

                                if (team == "b")
                                    TileList[i].tile.setColor(sf::Color::Blue);

                                sf::Packet packet;
                                packet << x << y << t << c << n << m;

                                if (socket.send(packet, serverIp, serverPort) != sf::Socket::Done) {
                                    std::cerr << "Error sending message" << std::endl;
                                }
                                isReady = true;
                            }

                            //Gather tomato
                            if (isBasket && TileList[i].tile.getTexture() == &as.tomato_bed_3_t) {
                                Gather(TileList[i], as.tomato_bed_1_t, pl, as.balance_txt, as.tomato_bed_rot_t, sh.tomato_sell_cost, TileList[i].tomato_grow_time, as.gather_sound_a);

                                string t;
                                float x = TileList[i].tile.getPosition().x;
                                float y = TileList[i].tile.getPosition().y;
                                string c = team;

                                string n = nickname;
                                sf::Uint16 m = pl.money_p;

                                TileList[i].color = team;
                                if (team == "r")
                                    TileList[i].tile.setColor(sf::Color::Red);

                                if (team == "b")
                                    TileList[i].tile.setColor(sf::Color::Blue);

                                if (TileList[i].lives > 0) {
                                    t = "tomato_bed_1_t";
                                }
                                else
                                    t = "tomato_bed_rot_t";

                                sf::Packet packet;
                                packet << x << y << t << c << n << m;

                                if (socket.send(packet, serverIp, serverPort) != sf::Socket::Done) {
                                    std::cerr << "Error sending message" << std::endl;
                                }
                                isReady = true;
                            }


                            //Build worker house
                            if (isBuild && TileList[i].tile.getTexture() == &as.grass_t && pl.money_p >= 50) {
                                pl.money_p -= 50;
                                as.balance_txt.setString(std::to_string(pl.money_p) + "$");

                                pl.max_worker += 1;
                                pl.cur_worker += 1;
                                as.worker_txt.setString(std::to_string(pl.cur_worker) + "/" + std::to_string(pl.max_worker));

                                TileList[i].tile.setTexture(as.grass_house_t);

                                string t = "grass_house_t";
                                float x = TileList[i].tile.getPosition().x;
                                float y = TileList[i].tile.getPosition().y;
                                string c = team;

                                string n = nickname;
                                sf::Uint16 m = pl.money_p;

                                TileList[i].color = team;
                                if (team == "r")
                                    TileList[i].tile.setColor(sf::Color::Red);

                                if (team == "b")
                                    TileList[i].tile.setColor(sf::Color::Blue);

                                sf::Packet packet;
                                packet << x << y << t << c << n << m;

                                if (socket.send(packet, serverIp, serverPort) != sf::Socket::Done) {
                                    std::cerr << "Error sending message" << std::endl;
                                }
                                isReady = true;
                            }
                        }
                    }
                    pr = true;
                }

                //Close game
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                    as.setMenuUi();
                    as.button_newgame_txt.setString("Continue Farming");
                    as.button_newgame_txt.move(-40, 0);
                }

                //Camera
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && side * 100 >= height) {
                    for (unsigned int i = 0; i < std::size(TileList); i++)
                        TileList[i].tile.move(0, 25);
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && side * 100 >= width) {
                    for (unsigned int i = 0; i < std::size(TileList); i++)
                        TileList[i].tile.move(-25, 0);
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && side * 100 >= height) {
                    for (unsigned int i = 0; i < std::size(TileList); i++)
                        TileList[i].tile.move(0, -25);
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && side * 100 >= width) {
                    for (unsigned int i = 0; i < std::size(TileList); i++)
                        TileList[i].tile.move(25, 0);
                }
            }
            else if (event.type == sf::Event::MouseButtonReleased || event.type == sf::Event::KeyReleased) pr = false;
        }


        //Render
        window.clear(sf::Color(127, 195, 255));

        //Tiles
        for (unsigned int i = 0; i < std::size(TileList); i++)
            window.draw(TileList[i].tile);

        //UI
        as.DrawUI(&window);

        window.display();
    }
    return 0;
}

