#include "game.h"
#include "editor.h"
Game::Game(QObject *parent)
	: QObject(parent)
{

}

Game::~Game()
{

}

void Game::initGame()
{
	init();
}

bool Game::init()
{
	if(!editor::instance()->getSceneFile())
		return false;
	if (!GameEngine::init()) 
		return false;

	QString file_path = editor::instance()->getSceneFile()->absoluteSceneFilePath();
	if(!GameEngine::loadScene(qPrintable(file_path)))
		return false;
		
	m_camID = GameEngine::entityWorldID("camera");
	updateLog();
	return true;
}
void Game::updateLog()
{
	int level;
	float time;
	QList<QListWidgetItem*> items;
	QString message;
	while(!(message = GameLog::getMessage(&level, &time)).isEmpty())
	{
		// Create List Widget Item (removing HTML Tags from message since Horde3D still adds some tags to the warnings)
		QListWidgetItem* item = new QListWidgetItem(QString::number(time) + "\t" + message.remove(QRegExp("(<(\\s*)?[a-z/]+(\\s*)?(/)?>)",Qt::CaseInsensitive, QRegExp::RegExp2)), 0, level);
		items.push_back(item);		
	}
	if (!items.isEmpty())
		editor::instance()->getLog_window()->updateLogWindow(items);
}

void Game::update()
{
	GameEngine::update();
	updateLog();
}
