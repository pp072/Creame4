#include "GameObject.h"

GameObject::GameObject( H3DNode node_id /*= 0*/ ) : _node_ID(node_id), _anim_mode(STOP)
{

}
GameObject::~GameObject(void)
{
}

void GameObject::setFlags(int flags)
{
	h3dSetNodeFlags(_node_ID,flags,true);
}

int GameObject::addAnimation( QString path )
{
	H3DRes animRes = h3dAddResource( H3DResTypes::Animation, qPrintable(path), 0 );
	_animations.push_back(animRes);
	h3dutLoadResourcesFromDisk(".");
	return animRes;
}

void GameObject::setStage( H3DRes animation, int stage, bool additive )
{
	h3dSetupModelAnimStage( _node_ID, stage, animation, 0, "", additive );
	_cur_anim_stage = stage;
}

void GameObject::autoSetupStages()
{
	for (int i = 0; i<_animations.size(); i++)
		h3dSetupModelAnimStage( _node_ID, i, _animations[i], 0, "", false );
}

void GameObject::playAnimation( int stage )
{
	_cur_anim_stage = stage;
	GameAnimationManager * anim_man = GameAnimationManager::getInstance();
	anim_man->addObject(this);
	_anim_mode = PLAY;
}




GameAnimationManager::GameAnimationManager( void )
{

}

GameAnimationManager::~GameAnimationManager( void )
{

}

GameAnimationManager * GameAnimationManager::getInstance()
{
	if(!_gameAnimnManInstance)
		_gameAnimnManInstance = new GameAnimationManager();
	return _gameAnimnManInstance;
}

void GameAnimationManager::addObject( GameObject* animObjects )
{
	bool isFound = false;
	for (int i = 0; i<_animObjects.size(); i++)
	{
		if(_animObjects[i]->getNodeID() == animObjects->getNodeID())
			isFound = true;
	}
	if(!isFound)
		_animObjects.push_back(animObjects);
}

void GameAnimationManager::update( float animTime )
{
	for (int i = 0; i<_animObjects.size(); i++)
	{
		h3dSetModelAnimParams( _animObjects[i]->getNodeID(),  _animObjects[i]->_cur_anim_stage, animTime * 30.0f, 1.0 );
		h3dUpdateModel( _animObjects[i]->getNodeID(), H3DModelUpdateFlags::Animation | H3DModelUpdateFlags::Geometry );
	}
}

void GameAnimationManager::reset()
{
	_animObjects.clear();
	_animObjects.resize(0);
}
