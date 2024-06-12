#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include <windows.h>

#include "AssetManager.h"
#include "PlayerManager.h"
#include "TileManager.h"
#include "ShopManager.h"


void HideConsole()
{
    HWND Hide;
    AllocConsole();
    Hide = FindWindowA("ConsoleWindowClass", NULL);
    ShowWindow(Hide, 0);
}

//Game Funcs
void Buy(PlayerManager &pl, int &product_p, int &product_amount, int &cost, sf::Text &amount_txt, sf::Text &button_txt, sf::Text &product_storage_txt, sf::Text &balance_txt) {
    product_p += product_amount;
    product_storage_txt.setString(std::to_string(product_p));

    pl.money_p -= product_amount * cost;
    product_amount = 0;

    amount_txt.setString(std::to_string(0));
    button_txt.setString("Buy for " + std::to_string(product_amount * cost) + "$");
    balance_txt.setString(std::to_string(pl.money_p) + "$");
}

void Sell(PlayerManager &pl, int &product_p, int &product_amount, int &cost, sf::Text& amount_txt, sf::Text& button_txt, sf::Text& product_storage_txt, sf::Text& balance_txt) {
    pl.money_p += product_amount * cost;
    product_p -= product_amount;
    product_amount = 0;

    amount_txt.setString(std::to_string(0));
    button_txt.setString("Sell for " + std::to_string(product_amount * cost) + "$");
    balance_txt.setString(std::to_string(pl.money_p) + "$");
    product_storage_txt.setString(std::to_string(product_p));
}

void Gather(TileManager &tile, sf::Texture &tile_t, int &product_p, sf::Text &product_amount_txt) {
    tile.tile.setTexture(tile_t);
    product_p += 1;
    product_amount_txt.setString(std::to_string(product_p));
    tile.growtime = tile.tomato_grow_time;
}

void Plant(TileManager &tile, sf::Texture& tile_t, int &plant_product_p, sf::Text &product_storage_txt) {
    plant_product_p -= 1;
    product_storage_txt.setString(std::to_string(plant_product_p));

    tile.tile.setTexture(tile_t);
    tile.growtime = 10;
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
    AssetManager as;
    PlayerManager pl{};
    TileManager ti;
    ShopManager sh;

    //Player Values
    pl.NewGameSettings();

    //Window
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Open Farm Simulator", sf::Style::Close);
    window.setFramerateLimit(60);
    HideConsole();

    //Load Game Assets
    as.setGameUI(pl.money_p, pl.tomato_p, pl.tomato_seed_p);
    as.setAudio();
    as.setTiles();

    //Game
    const int level[] =
    {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        1, 0, 0, 0, 1, 0, 0, 0, 0, 0,
        0, 1, 1, 1, 0, 0, 0, 0, 0, 0,
        0, 1, 1, 1, 0, 0, 0, 0, 0, 0,
        0, 1, 1, 1, 0, 0, 0, 0, 0, 0,
        0, 1, 1, 1, 0, 0, 0, 0, 0, 0,
        0, 1, 1, 1, 0, 0, 0, 0, 0, 0,
        0, 1, 1, 1, 0, 0, 0, 0, 0, 0,
    };


    //Map generating
    TileManager TileList[100];

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
                ti.tile.setTexture(as.water_t);
                ti.isWater = true;
            }

            ti.tile.setPosition(380 + i * 100, j * 100);

            //Values of tile
            ti.growtime = -1;
            TileList[n] = ti;

            n += 1;
        }

    //Update
    bool pr = false;

    bool isTomato = false;
    bool isShovel = false;
    bool isBasket = false;

    bool isStorage = false;
    bool isMarket = false;
    bool isShop = false;

    int tomato_choose = 0;
    int tomato_seed_choose = 0;

    sf::Sprite GameUiButtons[3]{as.shovel_s, as.tomato_seed_s, as.basket_s};

    static sf::Clock clockGrow;
    static sf::Clock clockAnim;

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
        if (clockAnim.getElapsedTime().asMilliseconds() > 0)
        {
            for (unsigned int i = 0; i < std::size(TileList); i++)
                if (TileList[i].isWater) TileList[i].tile.setTexture(as.water_0_t);
        }

        if (clockAnim.getElapsedTime().asMilliseconds() > 300)
        {
            for (unsigned int i = 0; i < std::size(TileList); i++)
                if (TileList[i].isWater) TileList[i].tile.setTexture(as.water_1_t);
        }

        if (clockAnim.getElapsedTime().asMilliseconds() > 500)
        {
            for (unsigned int i = 0; i < std::size(TileList); i++)
                if (TileList[i].isWater) TileList[i].tile.setTexture(as.water_2_t);
        }

        if (clockAnim.getElapsedTime().asMilliseconds() > 600)
        {
            for (unsigned int i = 0; i < std::size(TileList); i++)
                if (TileList[i].isWater) TileList[i].tile.setTexture(as.water_3_t);
        }

        if (clockAnim.getElapsedTime().asMilliseconds() > 700)
        {
            for (unsigned int i = 0; i < std::size(TileList); i++)
                if (TileList[i].isWater) TileList[i].tile.setTexture(as.water_4_t);
        }

        if (clockAnim.getElapsedTime().asMilliseconds() > 800)
        {
            for (unsigned int i = 0; i < std::size(TileList); i++)
                if (TileList[i].isWater) TileList[i].tile.setTexture(as.water_3_t);
        }

        if (clockAnim.getElapsedTime().asMilliseconds() > 1000)
        {
            for (unsigned int i = 0; i < std::size(TileList); i++)
                if (TileList[i].isWater) TileList[i].tile.setTexture(as.water_2_t);
        }

        if (clockAnim.getElapsedTime().asMilliseconds() > 1400)
        {
            for (unsigned int i = 0; i < std::size(TileList); i++)
                if(TileList[i].isWater) TileList[i].tile.setTexture(as.water_1_t);

            clockAnim.restart();
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


            //Places
            if (isStorage == true || isMarket == true || isShop == true)
                as.setTradePanel();

            else
                as.closeTradePanel();


            //Storage
            if (isStorage == true)
            {
                as.tomato_storage_txt.setPosition(950, 40);
                as.tomato_s.setPosition(900, 40);

                as.header_txt.setString("Storage");
            }

            else {
                as.tomato_storage_txt.setPosition(-100, -100);
                as.tomato_s.setPosition(-100, -100);
            }

            //Market
            if (isMarket == true)
            {
                as.header_txt.setString("Market");

                as.l_arrow_s.setPosition(950, 100);
                as.r_arrow_s.setPosition(1100, 100);
                as.button_s.setPosition(1000, 150);
                as.amount_txt.setPosition(1000, 100);
                as.tomato_s.setPosition(1000, 50);
                as.button_txt.setPosition(1000, 150);
                as.tomato_s.setTexture(as.tomato_t);
                as.button_txt.setString("Sell for " + std::to_string(tomato_choose * sh.tomato_sell_cost) + "$");
                as.amount_txt.setString(std::to_string(tomato_choose));
            }

            //Shop
            if (isShop == true)
                as.setShop(tomato_seed_choose);




            
            if (pr == false) //переменная, определяющая "нажатость" клавиши
            {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                {
                    //UI Clicking
                    for (int i = 0; i < std::size(GameUiButtons); i++) {
                        if (GameUiButtons[i].getGlobalBounds().contains(mousePositionFloat)) {
                            switch (i)
                            {
                            //Shovel
                            case 0:
                                isShovel = !isShovel;
                                isTomato = false;
                                isBasket = false;
                                break;
                            //Tomato
                            case 1:
                                isTomato = !isTomato;
                                isShovel = false;
                                isBasket = false;
                                break;
                                pr = true;
                            //Basket
                            case 2:
                                isTomato = false;
                                isShovel = false;
                                isBasket = !isBasket;
                                break;
                            }
                            pr = true;
                        }

                    }

                    if (as.storage_s.getGlobalBounds().contains(mousePositionFloat)) {
                        isStorage = true;
                        isMarket = false;
                        isShop = false;

                        isTomato = false;
                        isShovel = false;
                        isBasket = false; 
                    }

                    if (as.market_s.getGlobalBounds().contains(mousePositionFloat)) {
                        isStorage = false;
                        isMarket = true;
                        isShop = false;

                        isTomato = false;
                        isShovel = false;
                        isBasket = false;
                    }

                    if (as.shop_s.getGlobalBounds().contains(mousePositionFloat)) {
                        isStorage = false;
                        isMarket = false;
                        isShop = true;

                        isTomato = false;
                        isShovel = false;
                        isBasket = false;
                    }

                    if (as.exit_but_s.getGlobalBounds().contains(mousePositionFloat)) {
                        isStorage = false;
                        isMarket = false;
                        isShop = false;
                    }

                    if (as.l_arrow_s.getGlobalBounds().contains(mousePositionFloat)) { 
                        if (isMarket && tomato_choose > 0) {
                            tomato_choose -= 1;
                            as.button_txt.setString("Sell for " + std::to_string(tomato_choose * 2.5f) + "$");
                            as.amount_txt.setString(std::to_string(tomato_choose));
                        }

                        if (isShop) {
                            tomato_seed_choose -= 1;
                            as.button_txt.setString("Buy for " + std::to_string(tomato_choose * 5) + "$");
                            as.amount_txt.setString(std::to_string(tomato_seed_choose));
                        }
                        pr = true;
                    }

                    if (as.r_arrow_s.getGlobalBounds().contains(mousePositionFloat)) {
                        if (isMarket && tomato_choose < pl.tomato_p) {
                            tomato_choose += 1;
                            as.button_txt.setString("Sell for " + std::to_string(tomato_choose * 2.5f) + "$");
                            as.amount_txt.setString(std::to_string(tomato_choose));
                        }

                        if (isShop) {
                            tomato_seed_choose += 1;
                            as.button_txt.setString("Buy for " + std::to_string(tomato_choose * 5) + "$");
                            as.amount_txt.setString(std::to_string(tomato_seed_choose));
                        }
                        pr = true;
                    }

                    //If press Buy/Sell Button
                    if (as.button_s.getGlobalBounds().contains(mousePositionFloat)) {
                        if (isMarket) 
                            Sell(pl, pl.tomato_p, tomato_choose, sh.tomato_sell_cost, as.amount_txt, as.button_txt, as.tomato_storage_txt, as.balance_txt);

                        if (isShop && pl.money_p >= tomato_seed_choose * sh.tomato_seed_buy_cost)
                            Buy(pl, pl.tomato_seed_p, tomato_seed_choose, sh.tomato_seed_buy_cost, as.amount_txt, as.button_txt, as.tomato_seed_txt, as.balance_txt);

                        pr = true;
                    }

                    //Editing Tiles
                    for (unsigned int i = 0; i < std::size(TileList); i++)
                    {
                        if (TileList[i].tile.getGlobalBounds().contains(mousePositionFloat))
                        {
                            //Dig bed
                            if (isShovel == true && TileList[i].tile.getTexture() != &as.water_t && TileList[i].tile.getTexture() == &as.grass_t) {
                                TileList[i].tile.setTexture(as.dirt_t);
                                as.dig_sound_a.play();
                            }

                            //Plant tomato
                            if (isTomato == true && TileList[i].tile.getTexture() != &as.water_t && TileList[i].tile.getTexture() == &as.dirt_t && pl.tomato_seed_p != 0)
                                Plant(TileList[i], as.tomato_bed_1_t, pl.tomato_seed_p, as.tomato_seed_txt);

                            //Gather tomato
                            if (isBasket && TileList[i].tile.getTexture() == &as.tomato_bed_3_t)
                                Gather(TileList[i], as.tomato_bed_1_t, pl.tomato_p, as.tomato_storage_txt);
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