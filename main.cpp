#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include <windows.h>

#include "GenerationManager.h"
#include "PlayerManager.h"
#include "AssetManager.h"
#include "TileManager.h"
#include "ShopManager.h"


void HideConsole()
{
    HWND Hide;
    AllocConsole();
    Hide = FindWindowA("ConsoleWindowClass", NULL);
    ShowWindow(Hide, 1);
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

int main()
{
    //Set managers
    GenerationManager ge;
    PlayerManager pl{};
    AssetManager as;
    TileManager ti;
    ShopManager sh;

    //Player Values
    pl.NewGameSettings();

    //Window
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Open Farm Simulator", sf::Style::Close);
    window.setFramerateLimit(60);
    HideConsole();

    //Load Game Assets
    as.setGameUI(pl.money_p, pl.max_worker);
    as.setMenuUi();
    as.setAudio();
    as.setTiles();


    //Map generating
    TileManager TileList[100];

    int level[100];
    for (int i = 0; i < std::size(level); i++) {
        int start = 0;
        int end = 10;
        int x = rand() % (end - start + 1) + start;

        if (x < 3)
            level[i] = 1;
        else
            level[i] = 0;
    }

    unsigned int n = 0;
    for (unsigned int i = 0; i < 10; i++)
        for (unsigned int j = 0; j < 10; j++)
        {
            ti.tile.setScale(sf::Vector2f(1.f, 1.f));
            ti.tile.setTexture(as.grass_t);

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

            ti.tile.setPosition(140 + i * 100, j * 100);

            //Values of tile
            ti.growtime = -1;
            ti.digtime = -1;
            ti.planttime = -1;
            ti.gathertime = -1;
            TileList[n] = ti;

            n += 1;
        }

    //Update
    bool pr = false;

    bool isTomato = false;
    bool isShovel = false;
    bool isBasket = false;
    bool isBuild = false;

    sf::Sprite GameUiButtons[3]{as.shovel_s, as.basket_s, as.seed_s};

    static sf::Clock clockGrow;
    static sf::Clock clockAnim;
    static sf::Clock clockBuild;

    while (window.isOpen())
    {
        //GrowSystem
        if (clockGrow.getElapsedTime().asSeconds() > 1)
        {
            for (unsigned int i = 0; i < std::size(TileList); i++) {
                if (TileList[i].growtime != -1) {
                    TileList[i].growtime -= 1;
                }

                if (TileList[i].growtime == 5) {
                    TileList[i].tile.setTexture(as.tomato_bed_2_t);
                }

                if (TileList[i].growtime == 0) {
                    TileList[i].tile.setTexture(as.tomato_bed_3_t);
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

            if (pr == false) //ïåðåìåííàÿ, îïðåäåëÿþùàÿ "íàæàòîñòü" êëàâèøè
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
                                break;
                            //Basket
                            case 1:
                                isShovel = false;
                                isBasket = !isBasket;
                                isTomato = false;
                                break;
                                pr = true;
                            //Seed
                            case 2:
                                isShovel = false;
                                isBasket = false;
                                isTomato = !isTomato;
                                break;
                            }
                            pr = true;
                        }

                    }


                    //Editing Tiles
                    for (unsigned int i = 0; i < std::size(TileList); i++)
                    {
                        if (TileList[i].tile.getGlobalBounds().contains(mousePositionFloat) && !as.panel_s.getGlobalBounds().contains(mousePositionFloat) && pl.cur_worker != 0)
                        {
                            //Dig bed
                            if (isShovel == true && (TileList[i].tile.getTexture() == &as.grass_t || TileList[i].tile.getTexture() == &as.tomato_bed_rot_t)) {
                                TileList[i].digtime = 3;
                                TileList[i].tile.setTexture(as.grass_clock_t);

                                pl.cur_worker -= 1;
                                as.worker_txt.setString(std::to_string(pl.cur_worker) + "/" + std::to_string(pl.max_worker));

                                as.dig_sound_a.play();
                            }

                            //Plant tomato
                            if (isTomato == true && TileList[i].tile.getTexture() == &as.dirt_t)
                                Plant(TileList[i], as.tomato_bed_1_t, as.balance_txt, pl, sh.tomato_seed_buy_cost, TileList[i].tomato_grow_time, as.plant_sound_a);

                            //Gather tomato
                            if (isBasket && TileList[i].tile.getTexture() == &as.tomato_bed_3_t)
                                Gather(TileList[i], as.tomato_bed_1_t, pl, as.balance_txt, as.tomato_bed_rot_t, sh.tomato_sell_cost, TileList[i].tomato_grow_time, as.gather_sound_a);
                        }
                    }
                    pr = true;
                }

                //Close game
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                    window.close();

                //Camera
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                    for (unsigned int i = 0; i < std::size(TileList); i++) 
                        TileList[i].tile.move(0, 25);
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                    for (unsigned int i = 0; i < std::size(TileList); i++)
                        TileList[i].tile.move(-25, 0);
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                    for (unsigned int i = 0; i < std::size(TileList); i++) 
                        TileList[i].tile.move(0, -25);
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
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
