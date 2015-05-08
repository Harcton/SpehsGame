#include "Main.h"
#include "ResourceManager.h"

ResourceManager::~ResourceManager() {}
ResourceManager::ResourceManager()
{
	//Load global resources
	menuFont.loadFromFile("Font/ORANGEKI.ttf");

}

int ResourceManager::hashPath(std::string fname)
{
	size_t hashedPath = hasher(fname);
	//std::cout << "\nHashed name for " << fname << ": " << hashedPath;
	return hashedPath;
}


/*
Removes file path and returns file name
for example: "Texture/Player/turret.png" would return "turret.png"
*/
std::string ResourceManager::getFileName(std::string path)
{
	std::string fileName;
	int fileNameLength = 0;
	for (int i = path.size() - 1; i >= 0; i--)
		if (path[i] != '/')
			fileNameLength++;
		else
			i = -1;//End loop

	for (int i = 0; i < fileNameLength; i++)
	{
		fileName += path[path.size() - fileNameLength + i];
	}
	//std::cout << "\n\t\t " << fileNameLength << ":" << fileName;

	return fileName;
}

//Texture
void ResourceManager::loadTexture(std::string path)
{
	//std::string fileName = getFileName(path);
	//std::cout << "\nLoading " << fileName;

	size_t hashedPath = hashPath(getFileName(path));

	textureMap.insert(std::make_pair(hashedPath, sf::Texture()));
	textureMap[hashedPath].loadFromFile(path);
}
sf::Texture& ResourceManager::getTexture(std::string path)
{
	std::string fileName = getFileName(path);
	if (textureMap.find(hashPath(fileName)) != textureMap.end())
		return textureMap[hashPath(fileName)];
	else
	{
		std::cout << "\nResourceManager: No texture found [" << fileName << "] ";
		return textureMap[hashPath(fileName)];
	}
}
void ResourceManager::removeTexture(std::string path)
{
	std::string fileName = getFileName(path);
	textureMap.erase(hashPath(fileName));
}



//Sound
void ResourceManager::loadSound(std::string path)
{
	std::string fileName = getFileName(path);

	size_t hashedPath = hashPath(fileName);

	soundMap.insert(std::make_pair(hashedPath, sf::SoundBuffer()));
	soundMap[hashedPath].loadFromFile(path);
}
sf::SoundBuffer& ResourceManager::getSound(std::string path)
{
	std::string fileName = getFileName(path);
	return soundMap[hashPath(fileName)];
}
void ResourceManager::removeSound(std::string path)
{
	std::string fileName = getFileName(path);
	soundMap.erase(hashPath(fileName));
}