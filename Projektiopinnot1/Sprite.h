#ifndef SPRITE_H
#define SPRITE_H

namespace sge
{
	class Sprite
	{
	public:
		Sprite();
		Sprite(std::string);
		~Sprite();

		void draw(sf::RenderWindow&);
		void animationUpdate();

		//Misc
		void nextFrame();
		void previousFrame();

		//Setters
		void setTexture(sf::Texture&);
		void setPosition(double, double);
		void setColor(sf::Color&);
		void setRotation(double);
		void setOrigin(int, int);
		void setScale(double, double);
		void setFrame(int, int);
		void setAutoUpdate(bool);
		void setTilesetSize(int, int);
		void setHorizontalFrames(int);
		void setVerticalFrames(int);
		void setFrameDuration(int);
		void setFrameSize(int, int);
		void setFrameWidth(int);
		void setFrameHeight(int);
		void setVisibility(bool);

		//Getters (update when needed)
		double getRotation();

	private:
		sf::Sprite sprite;
		
		bool autoUpdate;
		bool visible;

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