#ifndef __PIROGRONIAN__ELASTIC_BODY_SIMULATOR__MAIN_WINDOW__H__ 
#define __PIROGRONIAN__ELASTIC_BODY_SIMULATOR__MAIN_WINDOW__H__

#include <QtCore/QtCore>
#include <QtGui/QtGui>
#include <QtWidgets/QtWidgets>

#include "ElasticBodySimulator.h"
#include "Presenter.h"

namespace Pirogronian {
	
	class MainWindow : public QMainWindow {
		Q_OBJECT
		
		QAction *newSimulationAction;
		QAction *startSimulationAction;
		QAction *stopSimulationAction;
		QTimer *timer;
		
		ElasticBodySimulator *sim;
		
		Presenter *pres;
	
		void createActions();
		void createMenus();
	public:
		MainWindow();
		~MainWindow();
		
	public slots:
		void newSimulation();
		void startSimulation();
		void stopSimulation();
		void stepSimulation();
	};
	
};

#endif
