#ifndef SPRITE_H
#define SPRITE_H

namespace sg
{
	class Sprite
	{
	public:
		Sprite();
		Sprite(std::string);
		~Sprite();

		void draw(sf::RenderWindow&);
	private:
		sf::Sprite sprite;
	};
}
#endif