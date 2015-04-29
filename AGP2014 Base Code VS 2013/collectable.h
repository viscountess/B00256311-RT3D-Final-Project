#ifndef COLLECTABLE_INCLUDE
#define COLLECTABLE_INCLUDE


//This will be the abstract base class for all collectables
//Using inheritance so if time permitted I can add in more collectables in the game

class Collectable
{
public:

	//coonstructor
	Collectable();

	//deconstructor
	virtual ~Collectable();

	virtual void initialise() = 0;
	virtual void render() = 0;
	virtual void update() = 0;
	
};

#endif //COLLECTABLE_INCLUDE