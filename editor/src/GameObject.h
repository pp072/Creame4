#ifndef GameObject_h__
#define GameObject_h__

#include "Horde3D.h"
#include "Horde3DUtils.h"
#include <QObject>
#include <vector>
#include "QtGui"
static class GameAnimationManager * _gameAnimnManInstance = 0x0;

enum AnimMode
{
	PLAY,
	STOP,
	PAUSE
};

class GameObject
{
	friend GameAnimationManager;
public:
	GameObject(H3DNode node_id = 0);
	~GameObject(void);

	int getNodeID() const { return _node_ID; }
	void setFlags(int flags);
	int addAnimation(QString path);
	void setStage(H3DRes animation, int stage, bool additive);
	void autoSetupStages();
	void playAnimation(int stage);
	
private:

	int _node_ID;
	int _cur_anim_stage;
	AnimMode _anim_mode;
	std::vector <H3DRes> _animations;
};
class GameAnimationManager
{
public:
	GameAnimationManager(void);
	~GameAnimationManager(void);
	static GameAnimationManager * getInstance();
	void addObject( GameObject* animObjects);
	void update(float animTime);
	void reset();

private:
	std::vector <GameObject*> _animObjects;
};
//Python Wrapper
class GameObjectWrapper: public QObject 
{
	Q_OBJECT

public slots:
	GameObject* new_GameObject(  int node_ID ) { return new GameObject( node_ID ); }
	int getNodeID( GameObject* o) { return o->getNodeID(); }
	void setFlags(GameObject* o, int flags){o->setFlags(flags); } //	FLAGS NoDraw = 1, NoCastShadow = 2, NoRayQuery = 4,	Inactive = 7  
	int addAnimation( GameObject* o, QString path) { return o->addAnimation(path); }
	void setStage( GameObject* o,H3DRes animation, int stage, bool additive){o->setStage(animation,stage,additive); }
	void autoSetupStages(GameObject* o){o->autoSetupStages(); }
	void playAnimation(GameObject* o, int stage){o->playAnimation(stage); } 
	
};

class EngineHRD {
public:
	EngineHRD() {}

};
Q_DECLARE_METATYPE(EngineHRD);
class EngineHRDWrapper: public QObject
{
	Q_OBJECT
public slots:
	EngineHRD* new_EngineHRD() { return new EngineHRD(); }
	int findObjectByName(EngineHRD* o, QString name)
	{
		int find_num = h3dFindNodes(1,qPrintable(name), H3DNodeTypes::Model);
		return h3dGetNodeFindResult( 0 );
	}
};
#endif // GameObject_h__