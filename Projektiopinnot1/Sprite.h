#ifndef SPRITE_H
#define SPRITE_H
//Forward declared dependencies
//Included dependencies
#include <string>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>



namespace sge
{
	class Sprite
	{
	public:
		Sprite();
		Sprite(sf::Texture&);
		~Sprite();

		virtual void draw(sf::RenderWindow&);
		void animationUpdate();

		//Misc
		void nextFrame();
		void previousFrame();
		void restart();

		//Setters
		void setTexture(sf::Texture&);
		void setPosition(double, double);
		void setColor(sf::Color&);
		void setRotation(double);
		void setOrigin(int, int);
		virtual void setScale(double, double);
		void setFrame(int, int);
		void setAutoUpdate(bool);
		void setTilesetSize(int, int);
		void setHorizontalFrames(int);
		void setVerticalFrames(int);
		void setFrameDuration(int);
		virtual void setFrameSize(int, int);
		void setFrameWidth(int);
		void setFrameHeight(int);
		void setVisibility(bool);
		void setLooping(bool);

		//Getters (update when needed)
		double getRotation();

	protected:
		sf::Sprite sprite;
		
		bool autoUpdate;
		bool visible;
		bool looping;

		int currentHorizontalFrame;
		int currentVerticalFrame;
		int horizontalFrames;
		int verticalFrames;

		int animationTimer; //Update cycles until the next frame
		int frameDuration;

		int frameWidth;
		int frameHeight;



	};
}
#endif