#include "Main.h"
#include "Sprite.h"

namespace sge
{
	Sprite::Sprite(std::string path) : Sprite()
	{
		setTexture(RM.getTexture(path));
	}
	Sprite::Sprite()
	{
		autoUpdate = true;
		visible = true;
		currentHorizontalFrame = 0;
		currentVerticalFrame = 0;
		horizontalFrames = 1;
		verticalFrames = 1;
		animationTimer = 0;
		frameDuration = 1;
		frameWidth = 0;
		frameHeight = 0;
	}


	Sprite::~Sprite()
	{
	}

	void Sprite::animationUpdate()
	{
		if (animationTimer > 0)
		{
			animationTimer--;
			return;
		}

		animationTimer = frameDuration;
		if (++currentHorizontalFrame >= horizontalFrames)
		{
			currentHorizontalFrame = 0;
			if (++currentVerticalFrame >= verticalFrames)
				currentVerticalFrame = 0;
		}
		setFrame(currentHorizontalFrame, currentVerticalFrame);
	}
	void Sprite::draw(sf::RenderWindow& window)
	{
		if (autoUpdate)
			animationUpdate();
		if (visible == true)
			window.draw(sprite);
	}


	//Misc
	void Sprite::nextFrame()
	{
		animationTimer = 0;
		animationUpdate();
	}
	void Sprite::previousFrame()
	{
		animationTimer = frameDuration;
		if (currentHorizontalFrame > 0)
			currentHorizontalFrame--;
		else
		{
			currentHorizontalFrame = horizontalFrames - 1;

			if (currentVerticalFrame > 0)
				currentVerticalFrame--;
			else
				currentVerticalFrame = verticalFrames - 1;
		}
		setFrame(currentHorizontalFrame, currentVerticalFrame);
	}


	//Setters
	void Sprite::setPosition(double x, double y)
	{
		sprite.setPosition(x, y);
	}
	void Sprite::setOrigin(int xo, int yo)
	{
		sprite.setOrigin(xo, yo);
	}
	void Sprite::setScale(double xScale, double yScale)
	{
		sprite.setScale(xScale, yScale);
	}
	void Sprite::setFrame(int xIndex, int yIndex)
	{
		sprite.setTextureRect(sf::IntRect(frameWidth*xIndex, frameHeight*yIndex, frameWidth, frameHeight));
	}
	void Sprite::setAutoUpdate(bool setting)
	{
		autoUpdate = setting;
	}
	void Sprite::setTexture(sf::Texture& texture)
	{
		sprite.setTexture(texture);
	}
	void Sprite::setTilesetSize(int xFrameCount, int yFrameCount)
	{
		horizontalFrames = xFrameCount;
		verticalFrames = yFrameCount;
	}
	void Sprite::setHorizontalFrames(int frameCount)
	{
		horizontalFrames = frameCount;
	}
	void Sprite::setVerticalFrames(int frameCount)
	{
		verticalFrames = frameCount;
	}
	void Sprite::setFrameDuration(int duration)
	{
		frameDuration = duration;
	}
	void Sprite::setFrameSize(int width, int height)
	{
		frameWidth = width;
		frameHeight = height;
	}
	void Sprite::setFrameWidth(int width)
	{
		frameWidth = width;
	}
	void Sprite::setFrameHeight(int height)
	{
		frameHeight = height;
	}
	void Sprite::setColor(sf::Color& color)
	{
		sprite.setColor(color);
	}
	void Sprite::setRotation(double rotation)
	{
		sprite.setRotation(rotation);
	}
	void Sprite::setVisibility(bool setting)
	{
		visible = setting;
	}


	//Getters
	double Sprite::getRotation()
	{
		return sprite.getRotation();
	}

}