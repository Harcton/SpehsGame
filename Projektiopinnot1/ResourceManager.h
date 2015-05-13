#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H
//Included dependencies
#include <string>
#include <cassert>
#include <map>
#include <functional>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Audio.hpp>


class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();


	void updateVolumeLevels();

//RESOURCES
	//Textures
	sf::Texture skeletonTex;
	sf::Texture coreTex;
	sf::Texture turretTex;
	sf::Texture engineTex;
	sf::Texture engineFireTex;
	sf::Texture engineFireAnimation;
	sf::Texture engineFireTier1Animation;
	sf::Texture engineFireTier4Animation;
	sf::Texture commanderTex;
	sf::Texture flierTex;
	sf::Texture enemyBasePurpleTex;
	sf::Texture enemyBaseGreenTex;
	sf::Texture enemyBaseTex;
	sf::Texture seekerTex;
	sf::Texture seekerActiveTex;
	sf::Texture seekerPassiveTex;
	sf::Texture seekerDodgeTex;
	sf::Texture sentinelTex;
	sf::Texture inheritanceArrowTex;
	sf::Texture circleSliderTex;
	sf::Texture slider1Tex;
	sf::Texture editorTurretTex;
	sf::Texture editorSkeletonTex;
	sf::Texture editorEngineTex;
	sf::Texture menuPlanetTex;
	sf::Texture menuSpace1Tex;
	sf::Texture menuLogoTex;
	sf::Texture sky1Tex;
	sf::Texture bullet1Tex;
	sf::Texture bullet2Tex;
	sf::Texture debris1Tex;
	sf::Texture ball1Tex;
	sf::Texture pointerArrowTex;
	//Textures-player ship
	sf::Texture turretPlatformTex;
	sf::Texture turretRotationEngineTex;
	sf::Texture turretFrameTex;
	sf::Texture turretBarrelTex;
	sf::Texture turretMagazineTex;
	sf::Texture turretRecoilAbsorberTex;
	sf::Texture turretReloaderTex;
	sf::Texture engineFuelTankTex;
	sf::Texture engineThrusterTex;
	sf::Texture engineRechargerTex;
	sf::Texture shipArmor1Tex;

	//Sounds
	sf::SoundBuffer menuButtonHoverBuffer;
	sf::Sound menuButtonHoverSound;
	sf::SoundBuffer menuUpgradeBuffer;
	sf::Sound menuUpgradeSound;
	sf::SoundBuffer menuClickBuffer;
	sf::Sound menuClickSound;
	sf::SoundBuffer turretFireBuffer;
	sf::Sound turretFireSound;
	sf::SoundBuffer explosionBuffer;
	sf::Sound explosionSound;
	sf::SoundBuffer engineBuffer;
	sf::Sound engineSound;


	//Music

	//Font
	sf::Font menuFont;






	//int hashPath(std::string);
	//std::string getFileName(std::string);
	//void loadTexture(std::string);
	//sf::Texture& getTexture(std::string);
	//void removeTexture(std::string);
	//void loadSound(std::string);
	//sf::SoundBuffer& getSound(std::string);
	//void removeSound(std::string);

private:
	//std::hash<std::string> hasher;
	//std::map<int, sf::Texture> textureMap;
	//std::map<int, sf::Texture>::iterator textureMapIt;
	//std::map<int, sf::SoundBuffer> soundMap;
	//std::map<int, sf::SoundBuffer>::iterator soundMapIt;


};

#endif