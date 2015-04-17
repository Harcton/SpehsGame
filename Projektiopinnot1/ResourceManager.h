#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H
#include <cassert>
#include <map>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	int hashPath(std::string);
	std::string getFileName(std::string);

	void loadTexture(std::string);
	sf::Texture& getTexture(std::string);
	void removeTexture(std::string);

	void loadSound(std::string);
	sf::SoundBuffer& getSound(std::string);
	void removeSound(std::string);


private:
	std::hash<std::string> hasher;

	std::map<int, sf::Texture> textureMap;
	std::map<int, sf::Texture>::iterator textureMapIt;
	std::map<int, sf::SoundBuffer> soundMap;
	std::map<int, sf::SoundBuffer>::iterator soundMapIt;


};

#endif