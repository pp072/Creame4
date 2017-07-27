#ifndef GAME_H
#define GAME_H

#include <QObject>

#include "GameEngine/GameEngine.h"
#include "GameEngine/GameLog.h"
#include "GameEngine/GameEngine_SceneGraph.h"

class Game : public QObject
{
	Q_OBJECT

public:
	Game(QObject *parent);
	~Game();
	bool init();
	void updateLog();
	void update();
	unsigned int m_camID;
private:
private slots:
	void initGame();
	
};

#endif // GAME_H
