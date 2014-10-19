
#include "Presenter.h" 

using namespace Pirogronian;

GranuleItem::GranuleItem(Presenter *p, double x, double y) : QGraphicsEllipseItem(x, y, 10, 10) {
	_presenter = p;
	setBrush(QBrush(Qt::black));
	setAcceptTouchEvents(true);
	setFlag(ItemIsMovable);
//	setFlag(ItemSendsGeometryChanges);
}

void GranuleItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *e) {
	Q_UNUSED(e)
	
	if (!presenter()->isEditable())  return;
	
	qreal x = pos().x() / _presenter->scale();
	qreal y = pos().y() / _presenter->scale();
	
	if (_presenter->axis() == Presenter::X) {
		granule->position.setY(x);
		granule->position.setZ(y);
		granule->prevpos.setY(x);
		granule->prevpos.setZ(y);
	} else if (_presenter->axis() == Presenter::Y) {
		granule->position.setX(x);
		granule->position.setZ(y);
		granule->prevpos.setX(x);
		granule->prevpos.setZ(y);
	} else if (_presenter->axis() == Presenter::Z) {
		granule->position.setX(x);
		granule->position.setY(y);
		granule->prevpos.setX(x);
		granule->prevpos.setY(y);
	}
	
}

LinkItem::LinkItem() {
	QPen pen;
	
	pen.setStyle(Qt::SolidLine);
	pen.setBrush(Qt::black);
	pen.setWidth(3);
	
	setPen(pen);
}

Presenter::Presenter(ElasticBodySimulator *s, QWidget *p) {
	setParent(p);
	sim = s;
	
	scene = new QGraphicsScene(this);
	view = new QGraphicsView(scene, this);
	
	view->setResizeAnchor(QGraphicsView::NoAnchor);
	view->setTransformationAnchor(QGraphicsView::NoAnchor);
	
	ax = X;
	pos = 0;
	_scale = 100;
}

Presenter::~Presenter () {
}

void Presenter::resizeEvent(QResizeEvent *e) {
	view->resize(size());
}

LinkItem *Presenter::createLink(GranuleItem *a, GranuleItem *b, Axis x) {
	LinkItem *item;
	
	item = new LinkItem;
	item->g1 = a;
	item->g2 = b;
	switch (x) {
		case X:
			item->link = &item->g1->granule->xLink;
			break;
		case Y:
			item->link = &item->g1->granule->yLink;
			break;
		case Z:
			item->link = &item->g1->granule->zLink;
	}
	
	return item;
}

void Presenter::init() {
	scene->clear();
	_itemList.clear();
	_itemMatrix.clear();
	_linkList.clear();
	
	int size1, size2;
	
	int x, y, z;
	
	Axis ax1, ax2;
	
	if (ax == X) { size1 = sim->ySize(); size2 = sim->zSize(); }
	else if (ax == Y) { size1 = sim->xSize(); size2 = sim->zSize(); }
	else { size1 = sim->xSize(); size2 = sim->ySize(); }
	
	for (int i = 0; i < size1; i++) {
		_itemMatrix.append(QVector<GranuleItem*>());
		for (int j = 0; j < size2; j++) {
			GranuleItem *item = new GranuleItem(this);
			
			switch (ax) {
				case X:
					x = pos;
					y = i;
					z = j;
					ax1 = Y;
					ax2 = Z;
					break;
				case Y:
					x = i;
					y = pos;
					z = j;
					ax1 = X;
					ax2 = Z;
					break;
				case Z:
					x = i;
					y = j;
					z = pos;
					ax1 = X;
					ax2 = Y;
			}
			
			item->posx = i;
			item->posy = j;
			item->granule = sim->granule(x, y, z);
//			item->setPresenter(this);
			scene->addItem(item);
			_itemList << item;
			_itemMatrix[i].append(item);
			
		}
	}
	
	if (size1 > 1 && size2 > 1) {
		LinkItem *item;
		for (int i = 0; i < size1; i++)
			for (int j = 0; j < size2; j++) {
				if (i < size1 - 1) {
					item = createLink(_itemMatrix[i][j], _itemMatrix[i+1][j], ax1);
					scene->addItem(item);
					_linkList << item;
				}
				if (j < size2 - 1) {
					item = createLink(_itemMatrix[i][j], _itemMatrix[i][j+1], ax2);
					scene->addItem(item);
					_linkList << item;
				}
			}
	}
	
//	view->resize((size1 + 1) * scale(), (size2 + 1) * scale());
	view->resize(size());
	view->setSceneRect(0, 0, (size1 + 1) * scale(), (size2 + 1) * scale());
	view->show();
	
	sim->sumForces();
	
	update();
	
/*	for (QList<LinkItem*>::iterator it = _linkList.begin(); it != _linkList.end(); it++)
		printf("Link((%f, %f),(%f, %f)) = ((%f, %f), (%f, %f))\n", 
				 (*it)->g1->pos().x(),  
				 (*it)->g1->pos().y(), 
				 (*it)->g2->pos().x(), 
				 (*it)->g2->pos().y(), 
				 (*it)->line().x1(),
				 (*it)->line().y1(),
				 (*it)->line().x2(),
				 (*it)->line().y2()
				);*/
}

void Presenter::update() {
//	int shiftX = view->size().width() / 2 - scale();
//	int shiftY = view->size().height() / 2 - scale();
	if (_itemList.isEmpty()) { puts("Empty!"); return; }
	
	for (QList<GranuleItem*>::iterator it = _itemList.begin(); it != _itemList.end(); it++) {
		ElasticBodySimulator::Granule *g = (*it)->granule;
		if (ax == X) (*it)->setPos((g->position.y() + 1) * scale(), (g->position.z() + 1) * scale());
		if (ax == Y) (*it)->setPos((g->position.x() + 1) * scale(), (g->position.z() + 1) * scale());
		if (ax == Z) (*it)->setPos((g->position.x() + 1) * scale(), (g->position.y() + 1) * scale());
	//	printf("Granule at [%f][%f][%f]\n", g->position.z, g->position.y, g->position.z);
	}
	
	for (QList<LinkItem*>::iterator it = _linkList.begin(); it != _linkList.end(); it++) {
		qreal b = sqrt(sim->eqSpace() + 6) + sim->eqSpace();
		qreal a = 510/b;
		qreal c = -255;
		int colint = a / ((*it)->link->length + b) + c;
		QColor col;
		if (colint < 0) col.setRgb(colint, 0, 0);
		else col.setRgb(0, colint, 0);
		QPen pen = (*it)->pen();
		pen.setBrush(col);
		
		(*it)->setLine(QLineF((*it)->g1->pos() + QPoint(5, 5), (*it)->g2->pos() + QPoint(5, 5)));
	}
}

void Presenter::setEditable() {
	for (QList<GranuleItem*>::iterator it = _itemList.begin(); it != _itemList.end(); it++)
		(*it)->setFlag(QGraphicsItem::ItemIsMovable);
	_editable = true;
}

void Presenter::setUneditable() {
	for (QList<GranuleItem*>::iterator it = _itemList.begin(); it != _itemList.end(); it++)
		(*it)->setFlag(QGraphicsItem::ItemIsMovable, false);
	_editable = false;
}

bool Presenter::isEditable() { return _editable; }

void putMatrix() {
	
}

