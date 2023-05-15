/*!*****************************************************************************
\file ECS_components.h
\author Jazz Teoh Yu Jue
\par DP email: j.teoh\@digipen.edu
\par Group: Memory Leak Studios
\date 27-11-2022
\brief
This file contains all the structs of components used in ECS. 
ComponentType starts from 0.
*******************************************************************************/
#pragma once
#include <glm/glm.hpp>
#include "vec2.h"
#include "SpriteVariable.h"
#include "AudioVariable.h"
#include "TagVariable.h"
#include "../Physics/Force.h"
#include "../Physics/ColliderBody.h"
#include <variant>
#include "ScriptComponent.h"
#include "AnimationVariable.h"

class Prefab;

/*!*****************************************************************************
\brief
Struct that encapsulates general information most component will have, such as
name, tag and isActive flag
*******************************************************************************/
struct General {
	std::string name;
	TAG tag;
	SUBTAG	subtag = SUBTAG::NOSUBTAG;
	bool isActive;
	bool isPaused;
	Prefab* prefab = nullptr; // Nullptr if it's not linked to any prefab
	std::set<Entity> children;
	Entity parent = Entity(0);
};

/*!*****************************************************************************
\brief
This component encapsulate the entity's lifetime
*******************************************************************************/
struct Lifespan {
	float lifetime; // Duration of entity's existance
	float limit; // Maximum duration of entity's existance

	//Lifespan(float _lifetime, float _limit) : lifetime(_lifetime), limit(_limit) {}
};

/*!*****************************************************************************
\brief
This component encapsulates what is needed in a Transform Component.
*******************************************************************************/
struct Transform
{
	Math::Vec2		scale = {1.f,1.f};		//save, edit, see
	float					rotation = 0.f;			//save, edit, see
	Math::Vec2		translation = {0.f,0.f};//save, edit, see
};

/*!*****************************************************************************
\brief
This component encapsulates what is needed in a Sprite Component. 
The sprite variable describes the type of sprite that should be rendered.
The texture variable contains the texture id of the texture to be rendered.
The layer variable contains which layer the sprite is to be rendered in.
*******************************************************************************/
struct Sprite
{
	Color color = Color{ 255, 255,255,255 };	//save, edit, see
	SPRITE sprite;						//save, edit, see
	GLuint texture = 0;					//save, edit, see
	int layer = 0;						//save, edit, see
};

struct Button
{
	bool interactable = true;			//save, edit, see
	bool isHover = false;				//NO save, NO edit, see
	bool isClick = false;				//NO save, NO edit, see
	bool activated = false;				//NO save, NO edit, see
	bool renderFlag = true;				//NO save, edit, see
};

/*!*****************************************************************************
\brief
This component encapsulates what is needed in an Animation component. 
The images variable stores the texture id of images in the animation.
The currentImageIndex contains the index of the texture in images vector to be
used.
*******************************************************************************/
struct Animation
{
	std::vector<SpriteSheet> sheets;	//save, edit, see
	int currentImageIndex = 0;			//save, edit, see
	//float timeToImageSwap;				//to be deleted
	/*SpriteSheet::::
	GLuint sheet = 0;
	int frameCount = 1;
	float timePerFrame = 1.f;*/
};

/*!*****************************************************************************
\brief
This component encapsulates what is needed in a SheetAnimation. Which is 
animated using spritesheets.
The frameCount variable stores the number of frames in a sprite.
The currFrameIndex stores the index of the frame to be used.
*******************************************************************************/
struct SheetAnimation
{
	int frameCount = 1;					//save, edit, see
	int currFrameIndex = 0;				//save, edit, see
	float timePerFrame = 1.f;			//save, edit, see
	float timeToFrameSwap = 0.f;		//NO save, NO edit, NO see
};

/*!*****************************************************************************
\brief
This component encapsulates information regarding dynamic movement of an entity.
> The gravityEnabled variable tells the physics manager if the entity should be
   affected by gravity
> The gravityScale variable tells the physics manager how much should the base 
   gravity force affect this object
> The dynamicsEnabled variable tells the physics manager if the entity is a 
   static, non-moving object that should not move
> The mass variable contains how heavy the object is
> The invMass variable contains the reciprocal of the mass mostly used in 
   calculations
> The inertia variable contains how much force is required to move the object
> The invInertia variable contains the reciprocal of the inertia mostly used in 
   calculations
> The restitution variable contains the restitution value which is used during 
   collision resolution to determine the amount of force conserved
> The friction variable contains the friction value 
> The damping variable contains the damping value used to create soft drag
> The accumulatedForce variable contains the sum of forces acting on the entity
   at the current step
> The velocity variable contains the current velocity of the object
> The acceleration variable contains the current acceleration value of the object
> The angularVelocity variable contains the current angular velocity of the object
> The angularTorque variable contains the current angular acceleration of the object
> The forceList variable contains the list of forces acting on the object
> The renderFlag variable contains the flag variable telling the render manager
   whether to render the velocity vector
*******************************************************************************/
struct Physics2D {
	bool dynamicsEnabled{true};				// save, edit, see

	float mass{1.f};						// save edit, see
	float inertia{1.f};						// save, NO edit, NO see (for now as unused)
	float restitution{0.0f};				// save, edit, see
	float friction{0.0f};					// save, NO edit, NO see (for now as unused)
	float damping{0.9f};					// save, edit, see 
	Math::Vec2 accumulatedForce{0.f, 0.f};	// save, NO edit, NO see
	Math::Vec2 velocity{0.f, 0.f};			// save, NO edit, NO see
	Math::Vec2 acceleration{0.f, 0.f};		// save, NO edit, NO see
	
	float angularVelocity{0.f};				// save, NO edit, NO see
	float angularTorque{0.f};				// save, NO edit, NO see
	
	std::vector<Force> forceList{};			// save, edit, see

	bool renderFlag{false};					// save, edit, see
};

struct LayerCollider {
	Math::Vec2 centerOffset = { 0.f, 0.f },	// save, edit, see
				scaleOffset = { 1.f, 1.f };	// save, edit, see
	//float yOffset;	
	// float rotationOffset,
	bool renderFlag = false;				// save, edit, see
};

/*!*****************************************************************************
\brief
This component encapsulates information regarding a rectangular collider for
collision detection
The centerOffset variable contains the offset from the entity's transform's 
 translation
The scaleOffset variable contains the offset from the entity's transform's scale
The renderFlag variable contains the flag variable telling the render manager
 whether to render the collider
*******************************************************************************/
struct RectCollider {
	Math::Vec2 centerOffset = { 0.f, 0.f },	// save, edit, see
			   scaleOffset = {1.f,1.f};		// save, edit, see
	// float rotationOffset,
	bool isTrigger{ false },				// save, edit, see
		 renderFlag{ false };				// save, edit, see
};

/*!*****************************************************************************
\brief
This component encapsulates information regarding a circular collider for
collision detection
The centerOffset variable contains the offset from the entity's transform's
 translation
The scaleOffset variable contains the offset from the entity's transform's scale
The renderFlag variable contains the flag variable telling the render manager
 whether to render the collider
*******************************************************************************/
struct CircleCollider {
	Math::Vec2 centerOffset = { 0.f, 0.f };	// save, edit, see
	float 	scaleOffset = { 1.f };			// save, edit, see
	// float rotationOffset,
	bool isTrigger{ false },				// save, edit, see
		 renderFlag{ false };				// save, edit, see
};


//struct Collider2D {
//	bool isTrigger;
//	bool renderFlag;
//	std::vector<ColliderBody> colliderList;
//};

/*!*****************************************************************************
\brief
This component encapsulates information regarding an edge/line collider for
collision detection
The p0Offset variable contains the offset from the entity's first point's 
 transform's translation
The rotationOffset variable contains the offset from the entity's first point's 
 transform's rotation
The scaleOffset variable contains the offset from the entity's transform's scale
The renderFlag variable contains the flag variable telling the render manager
 whether to render the collider
*******************************************************************************/
struct Edge2DCollider {
	Math::Vec2 p0Offset = { 0.f, 0.f };
	float rotationOffset = 0.f;
	float scaleOffset = 1.f;
	bool renderFlag = false;
};

/*!*****************************************************************************
\brief
This component encapsulates information regarding a point collider for
collision detection
The centerOffset variable contains the offset from the entity's transform's
 translation
The renderFlag variable contains the flag variable telling the render manager
 whether to render the collider
*******************************************************************************/
struct Point2DCollider {
	Math::Vec2 centerOffset = { 0.f, 0.f };
	bool renderFlag = false;
};

/*!*****************************************************************************
\brief
This temporary component encapsulates information regarding the player, such as health
*******************************************************************************/
//struct PlayerTmp {
//	int HP = 1;
//};

/*!*****************************************************************************
\brief
	This temporary struct contains the data for AI component
*******************************************************************************/
struct AI {
	int	colorChange =0;
	int	movement = 0;
	float	speed = 1.f;
	float range = 10.f;
};
/*!*****************************************************************************
\brief
	This struct contains the data for MovementAI component
*******************************************************************************/
struct MovementAI {
	bool run = false;
	bool next = true;	//save
	bool loop = false; //save
	bool reverse = false;//save
	bool cycle = false;//save
	int step = 0;
	int nextStep = 1; //save
	int setStep = -1;
	int state = 0;
	float currtime;
	float acceleration = 0.f;//save
	std::vector<float> time; //save
	std::vector<Transform> targetTransforms; //save
	bool moveOnHover = false;
	bool moveOnCollide = false;
	bool renderFlag = false;

	bool moveScale = true;
	bool moveRotate = true;
	bool moveTranslation = true;
};
struct ColorAI
{
	bool run = false;
	bool next = true;	//save
	bool loop = false; //save
	bool reverse = false;//save
	bool cycle = false;//save
	int step = 0;
	int nextStep = 1; //save
	int setStep = -1;
	int state = 0;
	float currtime;
	float acceleration = 0.f;//save
	std::vector<float> time; //save
	std::vector<Color> targetColors; //save
	bool changeOnHover = false;
	bool changeOnCollide = false;
	int spriteORtext = 0;

	bool changeR = true;
	bool changeG = true;
	bool changeB = true;
	bool changeA = true;
};
/*!*****************************************************************************
\brief
	This struct contains the data for Audio component
*******************************************************************************/
struct Audio {
	//std::vector<Sound> sound;
	Sound sound;
	bool isSpacial = false;
	float spacialDistance = 1.0f;
	float spacialRatio = 1.f;
	/*SOUND::::
	std::string path;
	bool toPlay = false;
	float volume = 1.0f;
	float volumeMod = 1.0f;
	float pitch = 1.0f;
	bool isPaused = false;
	bool isMute = false;
	bool isLoop = false;
	bool isRandPitch = false;
	int channel;*/
};
/*!*****************************************************************************
\brief
	This struct contains the data for Text component
*******************************************************************************/
struct Text {
	std::string fontFile = "BPtypewrite";	//save, edit, see
	std::string text = "Hello";				//save, edit, see
	Math::Vec2 offset = Math::Vec2{0, 0};	//save, edit, see
	float scale = 1.f;						//save, edit, see
	Color color = Color{ 0, 0, 0, 255 };	//save, edit, see
	bool followCam = false;					//NO save, edit, see
	float width = 0.f;
};

/*!*****************************************************************************
\brief
	This struct contains the data for Dialogue component
*******************************************************************************/
struct Dialogue
{
	std::string filename;
	int speakerID;
	int selectedID;
	int textID;
	int nextTextID;
};

/*!*****************************************************************************
\brief
	This struct contains the data for Script component
*******************************************************************************/
struct Script {
	std::string name;
	//ScriptComponent* script;
};

/*!*****************************************************************************
\brief
This component encapsulates information regarding a lightsource, currently, it
only contains an offset. Later versions will include light intensity.
*******************************************************************************/
struct LightSource
{
	Math::Vec2 centerOffset;
	float radius;
	float intensity; //"whiteness" of the light
};

struct ShadowCaster
{
	std::vector<Math::Vec2> centerOffset;
	bool renderFlag{ false };				// save, edit, see
};

struct Viewport {
	int width;
	bool isUI;
};
struct PlayerData
{
	std::string name = "Tom";
	int levelAt = 0;
	std::vector<int> data1;
	std::vector<int> data2;
};

struct ParticleSystem {
	// Information of particle to generate
	struct ParticleInfo {
		float mScale;					// Scale size of particle
		float mFacing;				// Direction which particle is facing (in degrees)
		float mLifespan;			// Lifespan of particle
		Sprite mSprite;				// Particle's sprite
		float mRotation;			// Particle's rotation per second (in degrees)
		float mExpanding = 0; // Speeed of particle expanding, in seconds. (Will shrink if negative) (The value is pixels expanding in x, y will be calculated accordingly to preserve aspect ratio)
		float mSpeed;					// Speed of particle
		bool mFading;					// Boolean value to determine if the color fades as it travels
		bool mFadeIn;					// Boolean value to determine if the color fades in from transparent. If this and mFading is true, it will fade in with half duration and fade out in half duration

		ParticleInfo() = default;
		ParticleInfo( float _Scale, float _Facing, float _Lifespan, Sprite _Sprite, float _Rotation, float _Speed, float _Expanding, bool _Fading, bool _FadeIn ) : 
			mScale(_Scale), mFacing(_Facing), mLifespan(_Lifespan), mSprite(_Sprite), mRotation(_Rotation), mSpeed(_Speed), mExpanding(_Expanding), mFading(_Fading), mFadeIn(_FadeIn)
		{}
	} mParticleInfo;

	int mDensity = 0;													// Amount of particles to generate
	Math::Vec2 mCenter = Math::Vec2();				// Center point of where particle generates from (Particles will be generated evently throughout the specified area)
	float mAreaWidth = 0;											// Half width of a square, away from the center point (Shape will be a square)
	float mDirection = 0;											// Direction of where the particles will travel
	float mSpread = 0;												// Angle in degrees, to spread the particles in a cone shape (360 degrees will spread all around evenly)
	float mDuration = 0;											// Duration to run the generator for. Particle system will stop generating once this number hits 0 or less
	bool mIsActive = false;										// Active state of the particle generator
	float mSlow = 0;													// Slow states that particle will only generate every indicated seconds instead of every frames

	float& SlowTracker() { return mSlowTracker; }
	
	const float& OriginalAlpha() const { return mOriginalAlpha; }
	float& OriginalAlpha() { return mOriginalAlpha; }

	ParticleSystem() = default;
	ParticleSystem( ParticleInfo _ParticleInfo, int _Density, Math::Vec2 _Center, float _AreaWidth,
									float _Direction, float _Spread, float _Duration, bool _IsActive, float _Slow ) :
		mParticleInfo( _ParticleInfo ), mDensity(_Density), mCenter(_Center), mAreaWidth(_AreaWidth), 
		mDirection(_Direction), mSpread(_Spread), mDuration(_Duration), mIsActive(_IsActive), mSlow(_Slow),
		mSlowTracker(), mOriginalAlpha(_ParticleInfo.mSprite.color.a)
	{}

private:
	// These variables should not be touched by anyone other than ParticleManager
	float mSlowTracker = 0;										// Track the duration for particle to spawn if slow
	float mOriginalAlpha = 0;
};

//use to index the variant data type, for ditor and serilization to determine type stored
enum class COMPONENTID
{
	GENERAL,		//0
	LIFESPAN,		//1
	TRANSFORM,		//2
	SPRITE,			//3
	ANIMATION,		//4
	SHEETANIMATION,	//5
	PHYSICS2D,		//6
	RECTCOLLIDER,	//7
	CIRCLECOLLIDER, //8
	EDGE2DCOLLIDER, //9
	POINT2DCOLLIDER,//10
	AUDIO,			//11
	TEXT,			//12
	AI,				//13
	SCRIPT,			//14
	DIALOGUE,		//15
	BUTTON,			//16
	LAYERCOLLIDER,	//17
	LIGHTSOURCE,	//18
	SHADOWCASTER,	//19
	Viewport,		//20
	MOVEMENTAI,      //21
	COLORAI,      //22
	PARTICLESYSTEM	//23
};
typedef std::variant<General, Lifespan, Transform, Sprite, Animation, SheetAnimation,
	Physics2D, RectCollider, CircleCollider, Edge2DCollider,
	Point2DCollider, Audio, Text, AI, Script, 
	Dialogue, Button, LayerCollider, 
	LightSource, ShadowCaster, Viewport, 
	MovementAI, ColorAI, ParticleSystem>  COMPONENT;



