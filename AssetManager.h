#include <SFML/Graphics.hpp>
#pragma once

/*
Value Names:
1) _txt - sf::Text
2) _s - sf::Sprite
3) _t - sf::Texture
4) _b - sf::Buffer
5) _a - sf::Audio
6) _p - Player Data
*/


class AssetManager {
public:
    //UI
	sf::Text balance_txt, tomato_storage_txt, tomato_seed_txt, header_txt, button_txt, amount_txt;
    sf::Font font;

    sf::Texture shovel_t, tomato_seed_t, basket_t, storage_t, market_t, shop_t, exit_but_t, back_panel_t, tomato_t;
    sf::Sprite shovel_s, tomato_seed_s, basket_s, storage_s, market_s, shop_s, exit_but_s, back_panel_s, tomato_s;

    sf::Sprite tomato_cursor_s, shovel_cursor_s, basket_cursor_s;

    sf::Texture l_arrow_t, r_arrow_t, button_t;
    sf::Sprite l_arrow_s, r_arrow_s, button_s;

    sf::Sprite tomato_shop_s;
    //Audio
    sf::SoundBuffer dig_sound_b;
    sf::Sound dig_sound_a;

    //Tiles
    sf::Texture grass_t, water_t, dirt_t, tomato_bed_1_t, tomato_bed_2_t, tomato_bed_3_t;
    sf::Texture water_0_t, water_1_t, water_2_t, water_3_t, water_4_t, water_5_t, water_6_t, water_7_t;

    //Game Load
    void setGameUI(int money_p, int tomato_p, int tomato_seed_p) {
        font.loadFromFile("textures/font/arial.ttf");

        //Balance
        balance_txt.setString(std::to_string(money_p) + "$");
        balance_txt.setFont(font);
        balance_txt.setCharacterSize(24);
        balance_txt.setFillColor(sf::Color::Black);
        balance_txt.setPosition(10, 50);

        //Tomato amount
        tomato_storage_txt.setFont(font);
        tomato_storage_txt.setString(std::to_string(tomato_p));
        tomato_storage_txt.setCharacterSize(24);
        tomato_storage_txt.setFillColor(sf::Color::Black);

        //Tomato Seed Amount
        tomato_seed_txt.setFont(font);
        tomato_seed_txt.setString(std::to_string(tomato_seed_p));
        tomato_seed_txt.setCharacterSize(24);
        tomato_seed_txt.setFillColor(sf::Color::Black);
        tomato_seed_txt.setPosition(550, 560);

        //Header
        header_txt.setFont(font);
        header_txt.setCharacterSize(24);
        header_txt.setFillColor(sf::Color::Black);

        //Button Text
        button_txt.setFont(font);
        button_txt.setCharacterSize(24);
        button_txt.setFillColor(sf::Color::Black);

        //Amount
        amount_txt.setFont(font);
        amount_txt.setCharacterSize(24);
        amount_txt.setFillColor(sf::Color::Black);

        //Textures
        shovel_t.loadFromFile("textures/ui/shovel.png");
        tomato_seed_t.loadFromFile("textures/ui/tomato_seed.png");
        basket_t.loadFromFile("textures/ui/basket.png");

        storage_t.loadFromFile("textures/ui/storage.png");
        market_t.loadFromFile("textures/ui/market.png");
        shop_t.loadFromFile("textures/ui/shop.png");

        exit_but_t.loadFromFile("textures/ui/exit_but.png");
        back_panel_t.loadFromFile("textures/ui/back_panel.png");

        tomato_t.loadFromFile("textures/ui/tomato.png");

        tomato_cursor_s.setTexture(tomato_t);
        tomato_cursor_s.setOrigin(50, 50);
        shovel_cursor_s.setTexture(shovel_t);
        shovel_cursor_s.setOrigin(50, 50);
        basket_cursor_s.setTexture(basket_t);
        basket_cursor_s.setOrigin(50, 50);

        l_arrow_t.loadFromFile("textures/ui/left_arrow.png");
        r_arrow_t.loadFromFile("textures/ui/right_arrow.png");
        button_t.loadFromFile("textures/ui/button.png");

        //Sprites
        shovel_s.setTexture(shovel_t);
        tomato_seed_s.setTexture(tomato_seed_t);
        basket_s.setTexture(basket_t);

        storage_s.setTexture(storage_t);
        market_s.setTexture(market_t);
        shop_s.setTexture(shop_t);

        exit_but_s.setTexture(exit_but_t);
        back_panel_s.setTexture(back_panel_t);

        l_arrow_s.setTexture(l_arrow_t);
        r_arrow_s.setTexture(r_arrow_t);
        button_s.setTexture(button_t);

        tomato_s.setTexture(tomato_t);
        tomato_shop_s.setTexture(tomato_seed_t);

        shovel_s.setPosition(400, 530);
        tomato_seed_s.setPosition(550, 530);
        basket_s.setPosition(650, 530);

        storage_s.setPosition(1160, 5);
        market_s.setPosition(1160, 155);
        shop_s.setPosition(1160, 305);
    }

    void setAudio() {
        dig_sound_b.loadFromFile("audio/dig_sound.ogg");
        dig_sound_a.setBuffer(dig_sound_b);
    }

    void setTiles() {
        grass_t.loadFromFile("textures/tiles/tile_grass.png");
        water_t.loadFromFile("textures/tiles/tile_water.png");
        dirt_t.loadFromFile("textures/tiles/tile_dirt.png");

        tomato_bed_1_t.loadFromFile("textures/tiles/tomato_bed_1.png");
        tomato_bed_2_t.loadFromFile("textures/tiles/tomato_bed_2.png");
        tomato_bed_3_t.loadFromFile("textures/tiles/tomato_bed_3.png");

        water_0_t.loadFromFile("textures/tiles/w00.png");
        water_1_t.loadFromFile("textures/tiles/w01.png");
        water_2_t.loadFromFile("textures/tiles/w02.png");
        water_3_t.loadFromFile("textures/tiles/w03.png");
        water_4_t.loadFromFile("textures/tiles/w04.png");
        water_5_t.loadFromFile("textures/tiles/w05.png");
        water_6_t.loadFromFile("textures/tiles/w06.png");
        water_7_t.loadFromFile("textures/tiles/w07.png");
    }

    //Game Funcs
    void setTradePanel() {
        exit_but_s.setPosition(1200, 10);
        back_panel_s.setPosition(900, 5);
        header_txt.setPosition(1000, 20);

        storage_s.setPosition(-100, -100);
        market_s.setPosition(-100, -100);
        shop_s.setPosition(-100, -100);
    }

    void closeTradePanel() {
        exit_but_s.setPosition(-100, -100);
        back_panel_s.setPosition(-500, -100);

        header_txt.setPosition(-100, -100);

        storage_s.setPosition(1160, 5);
        market_s.setPosition(1160, 155);
        shop_s.setPosition(1160, 305);

        l_arrow_s.setPosition(-500, -500);
        r_arrow_s.setPosition(-500, -500);
        button_s.setPosition(-500, -500);
        amount_txt.setPosition(-500, -500);
        tomato_s.setPosition(-500, -500);
        button_txt.setPosition(-500, -500);
        tomato_shop_s.setPosition(-500, -500);
    }

    void setShop(int & tomato_seed_choose) {
        header_txt.setString("Shop");

        l_arrow_s.setPosition(950, 100);
        r_arrow_s.setPosition(1100, 100);
        button_s.setPosition(1000, 150);
        amount_txt.setPosition(1000, 100);
        tomato_shop_s.setPosition(1000, 50);
        button_txt.setPosition(1000, 150);
        button_txt.setString("Buy for " + std::to_string(tomato_seed_choose * 5) + "$");
        amount_txt.setString(std::to_string(tomato_seed_choose));
    }

    void DrawUI(sf::RenderWindow* window) {
        window->draw(balance_txt);
        window->draw(tomato_seed_txt);

        window->draw(tomato_cursor_s);
        window->draw(shovel_cursor_s);
        window->draw(basket_cursor_s);
              
        window->draw(shovel_s);
        window->draw(tomato_seed_s);
        window->draw(basket_s);

        window->draw(storage_s);
        window->draw(market_s);
        window->draw(shop_s);
              
        window->draw(back_panel_s);
        window->draw(exit_but_s);
              
        window->draw(tomato_s);
        window->draw(tomato_shop_s);
        window->draw(header_txt);

        window->draw(l_arrow_s);
        window->draw(r_arrow_s);
        window->draw(button_s);
        window->draw(button_txt);
        window->draw(amount_txt);
              
        window->draw(tomato_storage_txt);
    }
};
