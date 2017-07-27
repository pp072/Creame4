#include <QtGui>

#include "pythontexteditor.h"


#include "RopeProject.h"

#include "filestabwidget.h"
#include "PythonEditor.h"
#include "def_settings.h"
#define stop __asm nop
PythonWriter::PythonWriter() : _designer(NULL)
{
	//_text_edit = new PythonTextEditor(this);
    //setCentralWidget(_text_edit.data());
	ui.setupUi(this);

	loadSettingsFile();

	_files_tab_widget = new FilesTabWidget(this);
	_files_tab_widget->setMovable(true);
	connect(_files_tab_widget, SIGNAL(currentChanged (int)), SLOT(slot_cur_tab_sel_changed(int)));
	connect(_files_tab_widget, SIGNAL(tabCloseRequested (int)), SLOT(slot_cur_tab_close(int)));

	//connect(this,SIGNAL(tabCloseRequested(int)),this,SLOT(closeTab(int)));
	setCentralWidget(_files_tab_widget);

	_dir_model.setRootPath(_PythonEditorSettings->value(SET_ROOT_DIRECTORY).toString());
	_dir_tree_view.setModel(&_dir_model);

	_dir_tree_view.header()->setSectionHidden(1,_PythonEditorSettings->value(SET_HIDE_FILES_SIZE).toBool());
	_dir_tree_view.header()->setSectionHidden(2,_PythonEditorSettings->value(SET_HIDE_FILES_TYPE).toBool());
	_dir_tree_view.header()->setSectionHidden(3,_PythonEditorSettings->value(SET_HIDE_FILES_DATE).toBool());

	_dir_tree_view.setAnimated(true);
	_dir_tree_view.setIndentation(20);
	_dir_tree_view.setSortingEnabled(true);
	_dir_tree_view.show();
	ui.verticalLayoutDir->addWidget(&_dir_tree_view);
	_dir_tree_view.show();
	connect(&_dir_tree_view, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(slot_dir_tree_clicked(QModelIndex)));
    createActions();
    
	
//    connect(_text_edit.data(), SIGNAL(textChanged()),
//            this, SLOT(documentWasModified()));

	
 	_rope_proj = QSharedPointer<Plugins::RopeProject>(new Plugins::RopeProject(), &QObject::deleteLater);
 	ui.dockWidgetConsole->setWidget(_rope_proj->_console);
 	connect(_rope_proj.data(), SIGNAL(serverStarted()), SLOT(onCodeComplite()));
 	loadFile(_PythonEditorSettings->value(SET_FILE_DEFAULT).toString());

	//Autocomplite
	QAction *compl = new QAction(this);
	compl->setShortcut(Qt::CTRL + Qt::Key_Space);
	connect(compl, SIGNAL(triggered()), SLOT(onCodeComplite()));
	addAction(compl);
	
	//designer
	action_designer = new QAction(QIcon(":/Designer/designer.ico"), tr("&Designer"), this);
	action_designer->setShortcut(Qt::Key_F4);
	connect(action_designer, SIGNAL(triggered()), SLOT(showDesigner()));
	addAction(action_designer);

	//Run code
	run_code = new QAction(QIcon(":/images/run.png"), tr("&Run code"), this);
	run_code->setShortcut(Qt::Key_F5);
	connect(run_code, SIGNAL(triggered()), SLOT(onRunCode()));
	addAction(run_code);

 	run_code_im = new QAction(QIcon(":/images/run_imed.png"), tr("&Imediatly run code"), this);
 	run_code_im->setShortcut(Qt::Key_F6);
 	connect(run_code_im, SIGNAL(triggered()), SLOT(onRunImmediatlyCode()));
 	addAction(run_code_im);

	createMenus();
	createToolBars();
	createStatusBar();
	readSettings();

 	
}

void PythonWriter::showDesigner()
{
	if(_designer)
	{
		_designer->deleteLater();
		_designer = new Designer();
	}	
	else
		_designer = new Designer();

	_designer->show();
}
void PythonWriter::onRunCode()
{
	save();
	onRunCodeFlushing << curFile;

}

void PythonWriter::onRunImmediatlyCode()
{
	save();
 	if(_rope_proj->is_rope_running())
 	{
 		rope_stop();
 		QTime time;
 		time.start();
 		for(;time.elapsed() < 2000;){}
 	}
	QLOG_INFO() << "onRunImmediatlyCode START" ;
	QLOG_INFO() << "_rope_proj->main_python_context.evalFile( " << curFile << ")" ;
	_rope_proj->_main_python_module.evalFile(curFile);
	QLOG_INFO() << "onRunImmediatlyCode FINISH"; 
}

void PythonWriter::flush_codes()
{
	if(onRunCodeFlushing.isEmpty())
		return;
 	if(_rope_proj->is_rope_running())
 	{
 		rope_stop();
 		QTime time;
 		time.start();
 		for(;time.elapsed() < 2000;){}
 	}
	foreach (const QString &curFile, onRunCodeFlushing)
	{
		_rope_proj->_main_python_module.evalFile(curFile);
		//if(!onRunCodeFlushing.isEmpty())
		onRunCodeFlushing.removeFirst();
	}
}

void PythonWriter::rope_stop()
{
	_rope_proj->makeRequest("stopServ", QVariantList(), this, SLOT(rope_stoped()));
	_rope_proj->_ip = "";
	_loop.exec();
}

void PythonWriter::rope_stoped()
{
	_loop.exit();
}

QString PythonWriter::getType(const QString & named)
{
	if (named == "function")
		return "3";
	if (named == "module")
		return "4";
	if (named == "class")
		return "2";
	if (named == "instance")
		return "5";
	return "1";
}

void PythonWriter::closeEvent(QCloseEvent *event)
{
	if(_designer){
		if(!_designer->close()){
			event->ignore();
			return;
		}
	}

	for (int i=0;i<_files_tab_widget->count();i++)
	{
		_text_edit=(PythonTextEditor*)_files_tab_widget->widget(i);
		
// 		if(!_files_tab_widget->tabWhatsThis(i).isEmpty())
// 			curFile = _files_tab_widget->tabWhatsThis(i);

		if(!mayBeSave())
		{
			event->ignore();
			return;
		}
	}

}

void PythonWriter::open()
{
   // if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this);
        if (!fileName.isEmpty())
            loadFile(fileName);
  //  }
}

bool PythonWriter::save()
{
    if (curFile.isEmpty()) {
        return saveAs();
    } else {
        return saveFile(curFile);
    }
}

bool PythonWriter::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this);
    if (fileName.isEmpty())
        return false;

    return saveFile(fileName);
}

void PythonWriter::about()
{
//    QMessageBox::about(this, tr("About Application"),
//             tr("The <b>Application</b> example demonstrates how to "
//                "write modern GUI applications using Qt, with a menu bar, "
//                "toolbars, and a status bar."));
}

void PythonWriter::documentWasModified()
{
	QString cfn  = curFile_name();
	_files_tab_widget->setTabText(_files_tab_widget->indexOf ( _text_edit ), curFile_name() + "*");
}

void PythonWriter::createActions()
{
     newAct = new QAction(QIcon(":/images/new.png"), tr("&New"), this);
     newAct->setShortcut(tr("Ctrl+N"));
     newAct->setStatusTip(tr("Create a new file"));
     connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));
 
     openAct = new QAction(QIcon(":/images/open.png"), tr("&Open..."), this);
     openAct->setShortcut(tr("Ctrl+O"));
     openAct->setStatusTip(tr("Open an existing file"));
     connect(openAct, SIGNAL(triggered()), this, SLOT(open()));
 
     saveAct = new QAction(QIcon(":/images/save.png"), tr("&Save"), this);
     saveAct->setShortcut(tr("Ctrl+2"));
     saveAct->setStatusTip(tr("Save the document to disk"));
     connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));
 
     saveAsAct = new QAction(tr("Save &As..."), this);
     saveAsAct->setStatusTip(tr("Save the document under a new name"));
     connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));
 
     exitAct = new QAction(tr("E&xit"), this);
     exitAct->setShortcut(tr("Ctrl+Q"));
     exitAct->setStatusTip(tr("Exit the application"));
     connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));
 
     cutAct = new QAction(QIcon(":/images/cut.png"), tr("Cu&t"), this);
     cutAct->setShortcut(tr("Ctrl+X"));
     cutAct->setStatusTip(tr("Cut the current selection's contents to the "
                             "clipboard"));

	 prefAct = new QAction(QIcon(":/images/setting_tools.png"), tr("Preferences"), this);
	 prefAct->setShortcut(tr("Ctrl+P"));
	 prefAct->setStatusTip(tr("Preferences"));
	 connect(prefAct, SIGNAL(triggered()), this, SLOT(openPreferences()));
	//create_text_edit_conn();
}

void PythonWriter::createMenus()
{
//     fileMenu = menuBar()->addMenu(tr("&File"));
//     fileMenu->addAction(newAct);
//     fileMenu->addAction(openAct);
//     fileMenu->addAction(saveAct);
//     fileMenu->addAction(saveAsAct);
//     fileMenu->addSeparator();
// 	fileMenu->addAction(prefAct);
//     fileMenu->addAction(exitAct);
// 
//     editMenu = menuBar()->addMenu(tr("&Edit"));
//     editMenu->addAction(cutAct);
//     editMenu->addAction(copyAct);
//     editMenu->addAction(pasteAct);
// 
//     menuBar()->addSeparator();
// 
//  	editCodeRunMenu = menuBar()->addMenu(tr("&Run"));
//  	editCodeRunMenu->addAction(run_code);
//  	editCodeRunMenu->addAction(run_code_im);

//    helpMenu = menuBar()->addMenu(tr("&Help"));
//     helpMenu->addAction(aboutAct);
//     helpMenu->addAction(aboutQtAct);
}

void PythonWriter::createToolBars()
{
    fileToolBar = addToolBar(tr("File"));
	fileToolBar->setMovable(false);
	fileToolBar->setIconSize(QSize(16,16));
    fileToolBar->addAction(newAct);
    fileToolBar->addAction(openAct);
    fileToolBar->addAction(saveAct);
	fileToolBar->addAction(prefAct);

    editToolBar = addToolBar(tr("Edit"));
	editToolBar->setMovable(false);
	editToolBar->setIconSize(QSize(16,16));
    editToolBar->addAction(cutAct);
    editToolBar->addAction(copyAct);
    editToolBar->addAction(pasteAct);

	editCodeRunBar = addToolBar(tr("Run"));
	editCodeRunBar->setMovable(false);
	editCodeRunBar->setIconSize(QSize(16,16));

	editCodeRunBar->addAction(action_designer);
	editCodeRunBar->addAction(run_code);
	editCodeRunBar->addAction(run_code_im);

}

void PythonWriter::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void PythonWriter::openPreferences()
{
	_settingUI = new SettingUI(this, SETTINGS_SCRIPT_EDITOR_FILE);
	_settingUI->show();
}


void PythonWriter::loadSettingsFile()
{
	_PythonEditorSettings = new QSettings(SETTINGS_SCRIPT_EDITOR_FILE,	QSettings::IniFormat ,this);
}

void PythonWriter::readSettings()
{
	this->restoreGeometry(_PythonEditorSettings->value("PythonEditor/geometry").toByteArray());
	this->restoreState(_PythonEditorSettings->value("PythonEditor/windowState").toByteArray());
}

void PythonWriter::writeSettings()
{
	_PythonEditorSettings->setValue("PythonEditor/geometry", this->saveGeometry());
	_PythonEditorSettings->setValue("PythonEditor/windowState", this->saveState());
}

bool PythonWriter::mayBeSave()
{
    if (_text_edit && _text_edit->isModified()) {
        int ret = QMessageBox::warning(this, tr("Application"),
                     tr("The document \""+_files_tab_widget->tabWhatsThis(_files_tab_widget->indexOf(_text_edit)).toLatin1()
					 +"\" has been modified.\n"
                        "Do you want to save your changes?"),
                     QMessageBox::Yes | QMessageBox::Default,
                     QMessageBox::No,
                     QMessageBox::Cancel | QMessageBox::Escape);
        if (ret == QMessageBox::Yes)
            return save();
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}

void PythonWriter::newFile()
{
	tryCreateNewFile("untitled", ".py");
	_text_edit = new PythonTextEditor(this);
	QString cfn = curFile_name();
	_files_tab_widget->addTab(_text_edit,curFile_name());
	_files_tab_widget->setTabWhatsThis(_files_tab_widget->count()-1,curFile_abs_path()+"/"+curFile_name());
	_files_tab_widget->setTabToolTip(_files_tab_widget->count()-1,curFile_abs_path()+"/"+curFile_name());
	_files_tab_widget->setCurrentIndex(_files_tab_widget->count()-1);
	create_text_edit_conn();
	documentWasModified();
	QDateTime dateTime = QDateTime::currentDateTime();
	QString dateTimeString = dateTime.toString();
	_text_edit->setText("# Python module for CREAME4 \n");

	QString cur_file = curFile_name();
	setCurrentFile(cur_file);
	_file = _rope_proj->openFile(curFile_abs_path()+"/"+curFile_name());
	connect(_file, SIGNAL(findImplementationComplite(QString, int)), SLOT(onFindImplementationComplite(QString, int)));
	connect(_file, SIGNAL(autoComplite(Plugins::CompletitionList)),this, SLOT(onAutoComplite(Plugins::CompletitionList)));
}

bool PythonWriter::tryCreateNewFile( QString newName, QString exten, int number )
{
	curFile = newName+QString::number(number)+exten;

 	if(!is_file_already_open() && !QFile::exists(curFile)){
 		curFile = newName+QString::number(number)+exten;
		return true;
 	}else{
		if (tryCreateNewFile(newName, exten, number+1))
 			return true;
	}
}

void PythonWriter::loadFile(const QString &fileName)
{
    QFile file(fileName);
	
    if (!file.open(QFile::ReadOnly)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

    QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
   
    QApplication::restoreOverrideCursor();

	file.fileName();
	curFile = fileName;
	if(!is_file_already_open())
	{
		_text_edit = new PythonTextEditor(this);
		QString cfn = curFile_name();
		_files_tab_widget->addTab(_text_edit,curFile_name());
		_files_tab_widget->setTabWhatsThis(_files_tab_widget->count()-1,curFile_abs_path()+"/"+curFile_name());
		_files_tab_widget->setTabToolTip(_files_tab_widget->count()-1,curFile_abs_path()+"/"+curFile_name());
		_text_edit->setText(in.readAll());
		
		_files_tab_widget->setCurrentIndex(_files_tab_widget->count()-1);
		create_text_edit_conn();
	}
    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"), 2000);
	_file = _rope_proj->openFile(curFile_abs_path()+"/"+curFile_name());
	connect(_file, SIGNAL(findImplementationComplite(QString, int)), SLOT(onFindImplementationComplite(QString, int)));
	connect(_file, SIGNAL(autoComplite(Plugins::CompletitionList)),this, SLOT(onAutoComplite(Plugins::CompletitionList)));
}

bool PythonWriter::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << _text_edit->text();
    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"), 2000);
	_files_tab_widget->setTabText(_files_tab_widget->indexOf ( _text_edit ), curFile_name());
	_files_tab_widget->setTabWhatsThis(_files_tab_widget->indexOf ( _text_edit ),curFile_abs_path()+"/"+curFile_name());
    return true;
}

void PythonWriter::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    _text_edit->setModified(false);
    setWindowModified(false);

    QString shownName;
//     if (curFile.isEmpty())
//         shownName = "untitled.txt";
//     else
    shownName = strippedName(curFile);

    setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(tr("Application")));
}

QString PythonWriter::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

void PythonWriter::onCodeComplite()
{
	if(!_rope_proj->is_rope_running())
	{
		_rope_proj->rope_for_modules_path  = curFile_abs_path();
		_rope_proj->start_new_rope_server();
	}
	int pos = _text_edit->SendScintilla(QsciScintilla::SCI_GETCURRENTPOS);
	if(!pos)
		return;
	QString textINEdit = _text_edit->text();
	_file->autocompletitionList(textINEdit, pos);
}

void PythonWriter::onAutoComplite( const Plugins::CompletitionList & list )
{
	_text_edit->SendScintilla(QsciScintilla::SCI_AUTOCSETSEPARATOR, 46);
	_text_edit->SendScintilla(QsciScintilla::SCI_AUTOCSETTYPESEPARATOR, ':');
	QStringList typedList;
	foreach(QString it, list.items){
		QStringList spl = it.split('|');
		if (spl.length() > 1){
			QString t = getType(spl[1]);

			typedList.append(spl[0]+":"+t);
		}
	}

	_text_edit->SendScintilla(QsciScintilla::SCI_AUTOCSHOW, list.word.length(), qPrintable(typedList.join(".")));
}



QString PythonWriter::curFile_abs_path()
{
	QString abs_path = QDir().absoluteFilePath(curFile);
	QStringList parts = abs_path.split("/");
	parts.removeAt(parts.size()-1);
	return parts.join("/");
}
QString PythonWriter::curFile_name()
{
	QString abs_path = QDir().absoluteFilePath(curFile);
	
	QStringList parts = abs_path.split("/");
	return parts.at(parts.size()-1);
}

PythonWriter::~PythonWriter()
{
	
	writeSettings();
	delete _rope_proj.data();

	if(_designer)
		delete _designer;
}

void PythonWriter::create_text_edit_conn()
{
	connect(_text_edit, SIGNAL(textChanged()),
		             this, SLOT(documentWasModified()));

	connect(cutAct, SIGNAL(triggered()), _text_edit, SLOT(cut()));

	copyAct = new QAction(QIcon(":/images/copy.png"), tr("&Copy"), this);
	copyAct->setShortcut(tr("Ctrl+C"));
	copyAct->setStatusTip(tr("Copy the current selection's contents to the "
		"clipboard"));
	connect(copyAct, SIGNAL(triggered()), _text_edit, SLOT(copy()));

	pasteAct = new QAction(QIcon(":/images/paste.png"), tr("&Paste"), this);
	pasteAct->setShortcut(tr("Ctrl+V"));
	pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
		"selection"));
	connect(pasteAct, SIGNAL(triggered()), _text_edit, SLOT(paste()));
	cutAct->setEnabled(false);
	copyAct->setEnabled(false);
	connect(_text_edit, SIGNAL(copyAvailable(bool)),
		cutAct, SLOT(setEnabled(bool)));
	connect(_text_edit, SIGNAL(copyAvailable(bool)),
		copyAct, SLOT(setEnabled(bool)));
	connect(_text_edit, SIGNAL(findImpl(QString, int)), SLOT(findImpl(QString, int)));
}

bool PythonWriter::is_file_already_open()
{
	for (int i=0;i<_files_tab_widget->count();i++)
	{
		QString what_this = _files_tab_widget->tabWhatsThis(i);
		if(what_this == curFile_abs_path()+"/"+curFile_name())
		{
			_files_tab_widget->setCurrentIndex(i);
			return true;
		}
	}
	return false;
}

void PythonWriter::slot_cur_tab_sel_changed( int index )
{
	_text_edit=(PythonTextEditor*)_files_tab_widget->widget(index);
	if(!_files_tab_widget->tabWhatsThis(index).isEmpty())
		curFile = _files_tab_widget->tabWhatsThis(index);
	QDir::Filters kOriFilters = _dir_model.filter();
	_dir_tree_view.setRootIndex( _dir_model.setRootPath(  curFile_abs_path()  ));
	_dir_tree_view.setCurrentIndex( _dir_model.setRootPath(  curFile_abs_path()  ) );
	ui.lineEditPath->setText(curFile_abs_path());
	_dir_model.setFilter( kOriFilters );
	_file = (Plugins::IRopeFile*)(_rope_proj->get_rope_files()[curFile]);
	stop
}

void PythonWriter::slot_cur_tab_close( int index )
{
	_text_edit=(PythonTextEditor*)_files_tab_widget->widget(index);

	if (mayBeSave()) {
		if(!_files_tab_widget->tabWhatsThis(index).isEmpty())
			curFile = _files_tab_widget->tabWhatsThis(index);		
		//((Plugins::IRopeFile*)(_rope_proj->get_rope_files()[curFile]))->deleteLater();
		delete _files_tab_widget->widget(index);
		_files_tab_widget->removeTab(index);
	}
}

void PythonWriter::slot_dir_tree_clicked( QModelIndex index )
{
	if(!_dir_model.isDir(index))
	{
		QString openning_file = _dir_model.filePath(index);
		QFileInfo info = QFileInfo(openning_file);
		QString extension = info.completeSuffix();

		if (extension == "py")
			loadFile(openning_file);
		else if(extension =="ui")
			loadFile_UI(openning_file);
		else
			loadFile(openning_file);	
	}
}

void PythonWriter::loadFile_UI( QString openning_file )
{
	if(_designer)
	{
		if(_designer->close())
		{
			_designer->deleteLater();
			_designer = new Designer(0,0,openning_file);
		}
	}	
	else
		_designer = new Designer(0,0,openning_file);

	_designer->show();
}

PythonQtObjectPtr PythonWriter::getMain_python_context() const
{
	return _rope_proj->_main_python_module;
}

void PythonWriter::mousePressEvent( QMouseEvent *e )
{
//	stop
}

void PythonWriter::findImpl( QString str, int pos )
{
	_file->findImplementation(str, pos);
}

void PythonWriter::onFindImplementationComplite( const QString &file, int offset )
{
	if(file.isEmpty())
		return;
	QString filename = file;
	filename.replace("\\" , "/");
 	loadFile( filename );
 	_text_edit->jumpOffset(offset);
	stop
}
