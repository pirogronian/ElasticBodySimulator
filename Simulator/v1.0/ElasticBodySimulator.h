#ifndef __PIROGRONIAN__ELASTIC_BODY_SIMULATOR__SIMULATOR__H__ 
#define __PIROGRONIAN__ELASTIC_BODY_SIMULATOR__SIMULATOR__H__

#include <QtCore/QtCore>
#include <QtGui/QVector3D>

namespace Pirogronian {
  
  
	class ElasticBodySimulator : public QObject {
		Q_OBJECT
	public:

		struct Link {
			qreal length;
		};
	  
		class Granule {
      public:
			QVector3D speed;
			QVector3D position;
			QVector3D prevpos;
			QVector3D force;
		
			Link xLink, yLink, zLink;
			
			bool movable;
		};
    
    ElasticBodySimulator(int, int, int, qreal, qreal, qreal, qreal, qreal, qreal);
    ~ElasticBodySimulator();
    
    int xSize() { return _xSize; }
    int ySize() { return _ySize; }
    int zSize() { return _zSize; }
    
    void setX(int);
    void setY(int);
    void setZ(int);
    
    qreal elasticCouple() { return _couple; }
    
    Granule *granule(int x, int y, int z) { return &_gMatrix[x][y][z]; }
    
    void sumForces();
    void doStep();
    
	 qreal granMass() { return _granMass; }
	 qreal linkMass() { return _linkMass; }
	 qreal timeSlice() { return _timeSlice; }
	 qreal eqSpace() { return _eqspace; }
	 
  private:
	  qreal _granMass;
	  qreal _linkMass;
    qreal _couple;
	 qreal _eqspace;
    qreal _timeSlice;
	 
	 qreal _totForce;
    
    int _xSize, _ySize, _zSize;
    
    QVector<QVector<QVector<Granule> > > _gMatrix;
  
    QVector3D inline distances(ElasticBodySimulator::Granule a, ElasticBodySimulator::Granule b) {
      QVector3D v;
    
      v.setX(b.position.x() - a.position.x());
      v.setY(b.position.y() - a.position.y());
      v.setZ(b.position.z() - a.position.z());
    
      return v;
    }
  
    QVector3D forces(ElasticBodySimulator::Granule a, ElasticBodySimulator::Granule b);
    
	 
    qreal calcForces();
	 void correctForces();
    
	 void updateLength(int, int, int);
    void updateForce(int, int, int);
    
    void updatePosition(Granule&);
    
  };
};

#endif

