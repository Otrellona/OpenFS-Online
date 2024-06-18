#include <SFML/Graphics.hpp>
#include <string>
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

int width = 1280;
int height = 720;

using namespace std;
void CreateText(sf::Text& text, sf::Font& font, string txt, int size, sf::Color color) {
    text.setFont(font);
    text.setString(txt);
    text.setCharacterSize(size);
    text.setFillColor(color);
}

class AssetManager {
public:
    //Menu UI
    sf::Font MenuFont;
    sf::Texture logo_t, button_menu_t, back_t;
    sf::Text button_newgame_txt, button_exitgame_txt, nickname_txt, IPAddress_txt, button_redteam_txt, button_blueteam_txt, win_txt, input_txt;
    sf::Sprite logo_s, button_newgame_s, button_exitgame_s, back_s, nickname_s, IPAddress_s, button_redteam_s, button_blueteam_s;
    sf::Music back_music;

    void setMenuUi() {
        MenuFont.loadFromFile("textures/font/arial.ttf");

        CreateText(win_txt, font, "For win you need to earn 300$", 46, sf::Color::Black);
        win_txt.setOrigin(win_txt.getLocalBounds().width / 2, win_txt.getLocalBounds().height / 2);
        win_txt.setPosition(width / 2, 150);

        //Text
        CreateText(button_newgame_txt, MenuFont, "Connect to server", 24, sf::Color::Black);
        CreateText(button_exitgame_txt, MenuFont, "Exit", 24, sf::Color::Black);
        CreateText(nickname_txt, MenuFont, "Enter nickname", 24, sf::Color::Black);
        CreateText(IPAddress_txt, MenuFont, "Enter server IP", 24, sf::Color::Black);
        CreateText(button_redteam_txt, MenuFont, "Red team", 24, sf::Color::Black);
        CreateText(button_blueteam_txt, MenuFont, "Blue team", 24, sf::Color::Black);
        CreateText(input_txt, MenuFont, "Press Enter to confirm", 24, sf::Color::Black);
        input_txt.setPosition(-100, -100);

        //Textures
        logo_t.loadFromFile("textures/ui/logo.png");
        back_t.loadFromFile("textures/ui/back.png");
        button_menu_t.loadFromFile("textures/ui/button.png");

        //Sprites
        logo_s.setTexture(logo_t);
        logo_s.setPosition((width-393)/2, 50);

        back_s.setTexture(back_t);
        back_s.setPosition(0, 0);
        back_s.setScale(width / back_s.getLocalBounds().width, height / back_s.getLocalBounds().height);

        nickname_s.setTexture(button_menu_t);
        nickname_s.setPosition((width - 222) / 2, 200);

        IPAddress_s.setTexture(button_menu_t);
        IPAddress_s.setPosition((width - 222) / 2, 300);

        button_redteam_s.setTexture(button_menu_t);
        button_redteam_s.setColor(sf::Color::Red);
        button_redteam_s.setPosition((width - 672) / 2, 400);

        button_blueteam_s.setTexture(button_menu_t);
        button_blueteam_s.setColor(sf::Color::Blue);
        button_blueteam_s.setPosition((width + 312) / 2, 400);


        button_newgame_s.setTexture(button_menu_t);
        button_newgame_s.setPosition((width - 222)/2, 550);


        button_exitgame_s.setTexture(button_menu_t);
        button_exitgame_s.setPosition((width-222)/2, 650);

        //Text
        button_newgame_txt.setPosition((width-195)/2, 560);
        button_exitgame_txt.setPosition((width-40)/2, 660);
        button_redteam_txt.setPosition((width - 550) / 2, 410);
        button_blueteam_txt.setPosition((width + 420) / 2, 410);
        nickname_txt.setPosition((width - 190) / 2, 210);
        IPAddress_txt.setPosition((width - 190) / 2, 310);
    }

    //Game UI
    sf::Font font;

    sf::Texture seed_t, tomato_t, balance_icon_t, worker_icon_t, panel_t;
    sf::Sprite seed_s, tomato_s, balance_icon_s, worker_icon_s, panel_s;

    //Window
    sf::Text balance_txt, worker_txt;
    sf::Text op_info_txt;

    //Cursor
    sf::Texture shovel_t, basket_t, build_t;
    sf::Sprite shovel_s, basket_s, build_s;
    sf::Sprite tomato_cursor_s, shovel_cursor_s, basket_cursor_s, build_cursor_s;

    //Audio
    sf::SoundBuffer dig_sound_b, plant_sound_b, gather_sound_b, build_sound_b, click_sound_b;
    sf::Sound dig_sound_a, plant_sound_a, gather_sound_a, build_sound_a, click_sound_a;

    //Tiles
    sf::Texture dirt_t_r, tomato_bed_1_t_r, tomato_bed_2_t_r, tomato_bed_3_t_r, tomato_bed_rot_t_r, grass_house_t_r, grass_clock_t_r;
    sf::Texture dirt_t_b, tomato_bed_1_t_b, tomato_bed_2_t_b, tomato_bed_3_t_b, tomato_bed_rot_t_b, grass_house_t_b, grass_clock_t_b;
    sf::Texture grass_t, water_0_t, water_1_t;

    //Game Load
    void setGameUI(int money_p, int max_worker) {
        //Font
        font.loadFromFile("textures/font/arial.ttf");

        //Text
        CreateText(balance_txt, font, std::to_string(money_p) + "$", 36, sf::Color::Black);
        balance_txt.setPosition(121, height-60);

        CreateText(worker_txt, font, std::to_string(max_worker) + "/" + std::to_string(max_worker), 36, sf::Color::Black);
        worker_txt.setPosition(121, height-150);

        //Network
        CreateText(op_info_txt, font, "Wait player", 36, sf::Color::Black);
        op_info_txt.setPosition(60, height - 650);

        //Textures
        balance_icon_t.loadFromFile("textures/ui/balance_icon.png"); 
        worker_icon_t.loadFromFile("textures/ui/worker.png");
        panel_t.loadFromFile("textures/ui/panel.png");
        shovel_t.loadFromFile("textures/ui/shovel.png");
        seed_t.loadFromFile("textures/ui/seed.png");
        basket_t.loadFromFile("textures/ui/basket.png");
        build_t.loadFromFile("textures/ui/build.png");

        tomato_t.loadFromFile("textures/ui/tomato.png");

        tomato_cursor_s.setTexture(tomato_t);
        tomato_cursor_s.setOrigin(25, 25);

        shovel_cursor_s.setTexture(shovel_t);
        shovel_cursor_s.setOrigin(36, 36);
        basket_cursor_s.setTexture(basket_t);
        basket_cursor_s.setOrigin(36, 36);
        build_cursor_s.setTexture(build_t);
        build_cursor_s.setOrigin(36, 36);

        //Sprites
        balance_icon_s.setTexture(balance_icon_t);
        worker_icon_s.setTexture(worker_icon_t);
        panel_s.setTexture(panel_t);
        shovel_s.setTexture(shovel_t);
        seed_s.setTexture(seed_t);
        basket_s.setTexture(basket_t);
        build_s.setTexture(build_t);

        tomato_s.setTexture(tomato_t);
        tomato_s.setPosition(-100, -100);

        balance_icon_s.setPosition(6, height-83);
        worker_icon_s.setPosition(6, height-183);
        panel_s.setPosition((width-panel_s.getLocalBounds().width)/2, height-91);

        shovel_s.setPosition((width-72)/2-68, height-76);
        basket_s.setPosition((width-72)/3, height-76);
        seed_s.setPosition((width-72)/2+68, height-76);
        build_s.setPosition((width - 72) / 2 + 68 * 3, height - 76);
    }
    //Remove Menu
    void CloseMenu() {
        back_s.setPosition(-10000, -10000);
        win_txt.setString("");

        logo_s.setPosition(-100, -100);
        
        button_newgame_txt.setPosition(-100, -100);
        button_exitgame_txt.setPosition(-100, -100);
        button_newgame_s.setPosition(-100, -100);
        button_exitgame_s.setPosition(-100, -100);
        
        button_blueteam_s.setPosition(-100, -100);
        button_blueteam_txt.setPosition(-100, -100);
        button_redteam_s.setPosition(-100, -100);
        button_redteam_txt.setPosition(-100, -100);
        nickname_s.setPosition(-100, -100);
        nickname_txt.setPosition(-100, -100);
        IPAddress_s.setPosition(-100, -100);
        IPAddress_txt.setPosition(-100, -100);
    }

    void setAudio() {
        dig_sound_b.loadFromFile("audio/dig_sound.ogg");
        dig_sound_a.setBuffer(dig_sound_b);

        plant_sound_b.loadFromFile("audio/plant_sound.ogg");
        plant_sound_a.setBuffer(plant_sound_b);

        gather_sound_b.loadFromFile("audio/gather_sound.ogg");
        gather_sound_a.setBuffer(gather_sound_b);

        build_sound_b.loadFromFile("audio/build_sound.ogg");
        build_sound_a.setBuffer(build_sound_b);

        click_sound_b.loadFromFile("audio/click_sound.ogg");
        click_sound_a.setBuffer(click_sound_b);

        back_music.openFromFile("audio/back_music.ogg");
        back_music.setLoop(true);
        back_music.play();
    }

    void setTiles() {
        grass_t.loadFromFile("textures/tiles/grass.png");
        water_0_t.loadFromFile("textures/tiles/water_0.png");
        water_1_t.loadFromFile("textures/tiles/water_1.png");

        dirt_t_r.loadFromFile("textures/tiles/dirt_r.png");
        tomato_bed_1_t_r.loadFromFile("textures/tiles/tomato_bed_1_r.png");
        tomato_bed_2_t_r.loadFromFile("textures/tiles/tomato_bed_2_r.png");
        tomato_bed_3_t_r.loadFromFile("textures/tiles/tomato_bed_3_r.png");
        tomato_bed_rot_t_r.loadFromFile("textures/tiles/tomato_bed_rot_r.png");
        grass_clock_t_r.loadFromFile("textures/tiles/grass_clock_r.png");
        grass_house_t_r.loadFromFile("textures/tiles/grass_house_r.png");

        dirt_t_b.loadFromFile("textures/tiles/dirt_b.png");
        tomato_bed_1_t_b.loadFromFile("textures/tiles/tomato_bed_1_b.png");
        tomato_bed_2_t_b.loadFromFile("textures/tiles/tomato_bed_2_b.png");
        tomato_bed_3_t_b.loadFromFile("textures/tiles/tomato_bed_3_b.png");
        tomato_bed_rot_t_b.loadFromFile("textures/tiles/tomato_bed_rot_b.png");
        grass_clock_t_b.loadFromFile("textures/tiles/grass_clock_b.png");
        grass_house_t_b.loadFromFile("textures/tiles/grass_house_b.png");
    }

    void DrawUI(sf::RenderWindow* window) {
        //Game
        window->draw(balance_txt);
        window->draw(balance_icon_s);
        window->draw(worker_txt);
        window->draw(worker_icon_s);
        window->draw(op_info_txt);

        window->draw(panel_s);
        window->draw(shovel_s);
        window->draw(basket_s);
        window->draw(seed_s);
        window->draw(build_s);

        window->draw(tomato_cursor_s);
        window->draw(shovel_cursor_s);
        window->draw(basket_cursor_s);
        window->draw(build_cursor_s);

        window->draw(tomato_s);

        //Menu
        window->draw(back_s);

        window->draw(button_redteam_s);
        window->draw(button_redteam_txt);
        window->draw(button_blueteam_s);
        window->draw(button_blueteam_txt);
        window->draw(nickname_s);
        window->draw(nickname_txt);
        window->draw(IPAddress_s);
        window->draw(IPAddress_txt);

        window->draw(button_newgame_s);
        window->draw(logo_s);
        window->draw(button_exitgame_s);
        window->draw(button_newgame_txt);
        window->draw(button_exitgame_txt);
        window->draw(win_txt);
        window->draw(input_txt);
    }
};
