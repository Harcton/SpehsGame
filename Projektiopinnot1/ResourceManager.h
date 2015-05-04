#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H
//Included dependencies
#include <string>
#include <cassert>
#include <map>
#include <functional>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Audio/SoundBuffer.hpp>



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