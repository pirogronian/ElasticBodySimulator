#ifndef __PIROGRONIAN__ELASTIC_BODY_SIMULATOR__PRESENTER__H__ 
#define __PIROGRONIAN__ELASTIC_BODY_SIMULATOR__PRESENTER__H__

#include <QtGui/QtGui>
#include <QtWidgets/QtWidgets>

#include "ElasticBodySimulator.h"

namespace Pirogronian {
	
	class Presenter;
	
	class GranuleItem : public QGraphicsEllipseItem {
		
	protected:
		virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *);
		
		Presenter *_presenter;
		
	public:
		ElasticBodySimulator::Granule *granule;
		
		GranuleItem(Presenter *, double x = 0, double y = 0);
		
		void setPresenter(Presenter *p) { _presenter = p; }
		Presenter *presenter() { return _presenter; }
		
		int posx, posy;
		
//		void udpatePosition();
//		void mapPosition();
	};
	
	class LinkItem : public QGraphicsLineItem {
	public:
		GranuleItem *g1, *g2;
		ElasticBodySimulator::Link *link;
		
		LinkItem();
	};
	
	class Presenter : public QWidget {
		Q_OBJECT
		
	public:
		enum Axis { X, Y, Z };
		
	private:
		Axis ax;
		int pos;
		bool _editable;
		
		QList<GranuleItem*> _itemList;
		QVector<QVector<GranuleItem*> > _itemMatrix;
		QList<LinkItem*> _linkList;
		
		QGraphicsScene *scene;
		QGraphicsView *view;
		
		ElasticBodySimulator *sim;
		
		double _scale;
		
		void resizeEvent(QResizeEvent *);
		
		LinkItem *createLink(GranuleItem *, GranuleItem *, Axis);
		
	public:
		
		void setAxis(Axis a) { ax = a; }
		Axis axis() { return ax; }
		
		void setPos(int i) { pos = i; }
		int axisPos() { return pos; }
		
		void setScale(double a) { _scale = a; }
		double scale() { return _scale; }
		
		void setSimulator(ElasticBodySimulator *s) { sim = s; }
		
		void setEditable();
		void setUneditable();
		bool isEditable();
		
		Presenter(ElasticBodySimulator *s =0, QWidget *parent = 0);
		~Presenter();
		
		void init();
		
		void putMatrix();
		
	public slots:
		void update();
	};
	
}

#endif
