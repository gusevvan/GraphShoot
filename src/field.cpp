#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <time.h>
#include <formula.h>
#include <field.h>

namespace gm {
    void Axis::draw(sf::RenderTarget& render, sf::RenderStates states) const {
        render.draw(_line, states);
        for (sf::RectangleShape shape : _segments) {
            render.draw(shape, states);
        }
        sf::Font font;
        font.loadFromFile("my_font.ttf");
        for (sf::Text text : _values) {
            text.setFont(font);
            render.draw(text, states);
        }
    }
    Ox::Ox() {
        _line.setSize(sf::Vector2f(800.f, 3.f));
        _line.setPosition(0, 300);
        _line.setFillColor(sf::Color(255, 102, 0));
        sf::RectangleShape segment(sf::Vector2f(3.f, 10.f));
        segment.setFillColor(sf::Color(255, 102, 0));
        segment.setPosition(2, 297);
        sf::Text text;
        text.setFillColor(sf::Color(255, 102, 0));
        text.setPosition(5, 303);
        text.setCharacterSize(16);
        for (int i = 0; i < 10; ++i) {
            text.setString(static_cast<char>(i + '0'));
            _segments.emplace_back(segment);
            _values.emplace_back(text);
            text.move(sf::Vector2f(80.f, 0));
            segment.move(sf::Vector2f(80.f, 0));
        }
    }

    Oy::Oy() {
        _line.setSize(sf::Vector2f(3.f, 600.f));
        _line.setPosition(2, 0);
        _line.setFillColor(sf::Color(255, 102, 0));
        sf::RectangleShape segment(sf::Vector2f(10.f, 3.f));
        segment.setFillColor(sf::Color(255, 102, 0));
        segment.setPosition(0, 0);
        sf::Text text;
        text.setFillColor(sf::Color(255, 102, 0));
        text.setPosition(5, 3);
        text.setCharacterSize(16);
        text.setString("10");
        _values.emplace_back(text);
        std::string value;
        for (int i = 9; i >= -9; --i) {
            if (i < 0) {
                value += '-';
            }
            value += char(abs(i) + '0');
            text.setString(value);
            text.move(sf::Vector2f(0.f, 30.f));
            _segments.emplace_back(segment);
            _values.emplace_back(text);
            segment.move(sf::Vector2f(0.f, 30.f));
            value.clear();
        }
        _segments.emplace_back(segment);
        text.setString("-10");
        text.move(sf::Vector2f(0.f, 30.f));
        segment.move(sf::Vector2f(0.f, 30.f));
        _segments.emplace_back(segment);
        _values.emplace_back(text);
    }
    
    void Field::generate() {
        srand(time(NULL));

        _whites.clear();
        _blacks.clear();
        _backGround.setSize(sf::Vector2f(800.f, 602.f));
        _backGround.setFillColor(sf::Color::White);


        for (int i = 0; i < _rounds; ++i) {
            int radius = rand() % 100 + 10, Px = rand() % 801, Py = rand() % 601;
            sf::CircleShape shape(radius);
            shape.setOrigin(radius, radius);
            shape.setPosition(Px, Py);
            shape.setFillColor(sf::Color::Black);
            _blacks.push_back(shape);
        }

        for (int i = 0; i < _targets; ++i) {
            int Px = rand() % 601 + 200, Py = rand() % 601;
            sf::CircleShape shape1(_radius_reds);
            shape1.setOrigin(_radius_reds, _radius_reds);
            shape1.setPosition(Px, Py);
            shape1.setFillColor(sf::Color::Red);
            _reds.push_back(shape1);
            sf::CircleShape shape2(_radius_reds + 3);
            shape2.setOrigin(_radius_reds + 3, _radius_reds + 3);
            shape2.setPosition(Px, Py);
            _whites.push_back(shape2);
        }

        _grPoint.setRadius(2.f);
        _grPoint.setFillColor(sf::Color::Blue);
        _grPoint.setPosition(2, 300);

        sf::CircleShape shape(100);
        shape.setOrigin(sf::Vector2f(100.f, 100.f));
        shape.setPosition(2.f, 300.f);
        _whites.push_back(shape);
    }

    void Field::setDelta(double newDelta) {
        _delta = 30 * newDelta;
    }

    bool Field::checkTarget() {
        for (int i = 0; i < _reds.size(); ++i) {
            if ((_reds[i].getPosition().x - _grX) * (_reds[i].getPosition().x - _grX) +
                (_reds[i].getPosition().y - _grY) * (_reds[i].getPosition().y - _grY)
                <= _reds[i].getRadius() * _reds[i].getRadius()) {
                if (_reds[i].getFillColor() != sf::Color::White) {
                    _reds[i].setFillColor(sf::Color::White);
                    --_targets;
                    _grX = 2;
                    _grY = 300;
                    _grPoint.setPosition(2, 300);
                    return true;
                }
            }
        }
        return false;
    }

    std::string Field::getTargets() {
        std::ostringstream strs;
        strs << _targets;
        return strs.str();

    }

    void Field::setTargets(const int& targets) {
        _targets = targets;
    }

    void Field::setRounds(const int& level)
    {
        switch (level)
        {
        case 1:
            _rounds = rand() % 5 + 3;
            break;
        case 2:
            _rounds = rand() % 8 + 5;
            break;
        case 3:
            _rounds = rand() % 11 + 8;
            break;
        default:break;
        }
    }
    void Field::setRadiusReds(const int& radius)
    {
        _radius_reds = radius;
    }

    void Field::updateGraph() {
        _grX += 0.01 * _time * 10;
    }

    double Field::getGrX() {
        return _grX;
    }

    double Field::getGrY() {
        return _grY;
    }

    void Field::setGrY(double newGrY) {
        _grY = _delta + 300 - 30 * newGrY;
    }

    bool Field::checkCrash() {
        bool isCrashed = false;
        if (_grX > 800 || _grY > 600 || _grY < 0 || _grX < 0) {
            _grX = 2;
            _grY = 300;
            _grPoint.setPosition(2, 300);
            return true;
        }
        for (sf::CircleShape shape : _blacks) {

            if ((shape.getPosition().x - _grX) * (shape.getPosition().x - _grX) +
                (shape.getPosition().y - _grY) * (shape.getPosition().y - _grY)
                <= shape.getRadius() * shape.getRadius()) {
                isCrashed = true;
                break;
            }
        }
        if (isCrashed) {
            for (sf::CircleShape shape : _whites) {
                if ((shape.getPosition().x - _grX) * (shape.getPosition().x - _grX) +
                    (shape.getPosition().y - _grY) * (shape.getPosition().y - _grY)
                    <= (shape.getRadius() - 0.1) * (shape.getRadius() - 0.1)) {
                    isCrashed = false;
                    break;
                }
            }
            if (isCrashed) {
                sf::CircleShape shape(10);
                shape.setOrigin(5, 5);
                shape.setPosition(_grX, _grY);
                _whites.push_back(shape);
                _grX = 2;
                _grY = 300;
                _grPoint.setPosition(2, 300);
            }
        }
        return isCrashed;
    }

    void Field::changeFormulaStatus() {
        _isFormula = !_isFormula;
    }
    void Field::updateTime(const float& time)
    {
        _time = time;
    }
    void Field::draw(sf::RenderTarget& render, sf::RenderStates states) const
    {
        if (!_isFormula) {
            render.draw(_backGround, states);

            for (sf::CircleShape shape: _blacks) {
                render.draw(shape, states);
            }

            for (sf::CircleShape shape: _whites) {
                render.draw(shape, states);
            }

            for (sf::CircleShape shape : _reds) {
                if (shape.getFillColor() == sf::Color::White) {
                    render.draw(shape, states);
                }
            }

            for (sf::CircleShape shape : _reds) {
                if (shape.getFillColor() == sf::Color::Red) {
                    render.draw(shape, states);
                }
            }

            render.draw(_ox, states);
            render.draw(_oy, states);
        }
        

        if (_isFormula)
        {
            
            _grPoint.setPosition(_grX, _grY);
            render.draw(_grPoint, states);
        }

    }
}