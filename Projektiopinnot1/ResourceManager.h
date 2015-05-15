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
	sf::Texture menuSpace1Tex;
	sf::Texture menuLogoTex;
	sf::Texture sky1Tex;
	sf::Texture sky2Tex;
	sf::Texture sky3Tex;
	sf::Texture bullet1Tex;
	sf::Texture bullet2Tex;
	sf::Texture bullet3Tex;
	sf::Texture bullet4Tex;
	sf::Texture bullet5Tex;
	sf::Texture debris1Tex;
	sf::Texture stationMainTex;
	sf::Texture stationDocksTex;
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
	//Visual effects
	sf::Texture explosion1Tex;
	//in-game Gui
	sf::Texture compassTex;
	sf::Texture ball1Tex;
	sf::Texture pointerArrowTex;
	sf::Texture pressEnterToDockTex;
	sf::Texture displayMeterTex;

	//Sounds
	sf::SoundBuffer menuButtonHoverBuffer;
	sf::Sound menuButtonHoverSound;
	sf::SoundBuffer menuUpgradeBuffer;
	sf::Sound menuUpgradeSound;
	sf::SoundBuffer menuClickBuffer;
	sf::Sound menuClickSound;
	sf::SoundBuffer enterMenuBuffer;
	sf::Sound enterMenuSound;
	sf::SoundBuffer turretFireBuffer;
	sf::Sound turretFireSound;
	sf::SoundBuffer explosionBuffer;
	sf::Sound explosionSound;
	sf::SoundBuffer thrustingBuffer;
	sf::Sound thrustingSound;


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